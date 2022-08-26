from ossaudiodev import control_labels
from selenium.webdriver.common.by import By
from selenium.common.exceptions import InvalidSessionIdException, NoSuchElementException
from selenium import webdriver
from time import sleep
from datetime import datetime
import requests
import logging
import os

HOST = os.getenv('JUPYTER_HOST')
ADMIN_ID = os.getenv('ADMIN_ID')
ADMIN_PASSWORD = os.getenv('ADMIN_PASSWORD')
WATCHER_ID = os.getenv('WATCHER_ID')
WATCHER_PASSWORD = os.getenv('WATCHER_PASSWORD')
SELENIUM_REMOTE = os.getenv('SELENIUM_REMOTE')

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(levelname)s %(message)s')

class URL:
    LOGIN = '/hub/login'
    SIGNUP = '/hub/signup'
    USER_LIST = '/hub/api/users'
    GET_TOKEN = '/hub/token'

class Tester:

    driver = None

    def __init__(self, url) -> None:
        logging.info(f"Initializing tester with {url}")
        self.url = url
        self.token = None
        self.user_list = []
        self.user_upt_time = None
        self.driver = webdriver.Chrome(executable_path="/usr/src/chrome/chromedriver",
                                       options=self.__get_chrome_options())
        self.driver.get(url)
        sleep(1)
        logging.info(f"Initializing tester Done")


    def __get_chrome_options(self):
        options = webdriver.ChromeOptions()
        options.add_argument('--headless')
        options.add_argument('--window-size=1920x1080')
        options.add_argument('--disable-gpu')
        options.add_argument('--no-sandbox')
        return options


    def do_login(self, username, password):
        logging.info(f"Do Login: [{username}, {password}]")
        self.driver.get(self.url + URL.LOGIN)
        sleep(2)
        id_input_box = self.driver.find_element(By.ID, 'username_input')
        password_input_box = self.driver.find_element(By.ID, 'password_input')

        id_input_box.send_keys(username)
        password_input_box.send_keys(password)

        login_button = self.driver.find_element(By.ID, 'login_submit')
        login_button.click()

        logging.info(f"Trying login")
        sleep(1)

        if 'login' in self.driver.current_url:
            logging.error('Login failed')
            logging.error(self.driver.find_element(By.CLASS_NAME, 'login_error'))
            logging.info('Try sign up with same value')
            ret = self.do_signup(username, password)
            if ret:
                logging.info('Retry login')
                return self.do_login(username, password)
            else:
                logging.error('Login failed')
                return False

        if f'/user/{username}' in self.driver.current_url:
            logging.info('Login succeed')
            return True
        else:
            logging.error('Failed with unknown error')
            return False


    def do_signup(self, username, password):
        self.driver.get(self.url + URL.SIGNUP)
        sleep(1)
        logging.info(f"Do sign up: [{username}, {password}]")
        id_input_box = self.driver.find_element(By.ID, 'username_input')
        password_input_box = self.driver.find_element(By.ID, 'pwd')

        id_input_box.send_keys(username)
        password_input_box.send_keys(password)

        login_button = self.driver.find_element(By.ID, 'signup_submit')
        login_button.click()

        sleep(1)

        success = self.driver.find_element(By.CLASS_NAME, 'alert-success')
        if success:
            logging.info(f"Sign up succeed")
            return True
        
        fail = self.driver.find_element(By.CLASS_NAME, 'alert-danger')
        if fail:
            logging.error(f"Sign up failed")
            logging.error(fail.text)
            return False

        logging.error(f"Sign up failed with unknown error")
        return False


    def get_user_list(self):
        logging.info("Try to get user list")
        if self.token == None:
            logging.error("Token is not setted")
            self.get_token()
        ret = requests.get(self.url + URL.USER_LIST, headers={'Authorization': f'token {self.token}'})
        try:
            ret.raise_for_status()
        except Exception as e:
            logging.error("Error request user list", exc_info=e)
        self.user_list = ret.json()
        self.user_upt_time = datetime.now()
        logging.info(f"Received total {str(len(self.user_list))} users")


    def get_token(self):
        logging.info("Try to request token")
        self.driver.get(self.url + URL.GET_TOKEN)
        sleep(1)
        request_token_btn = self.driver.find_element(By.CLASS_NAME, 'btn-jupyter')
        request_token_btn.click()
        sleep(0.3)
        self.token = self.driver.find_element(By.ID, 'token-result').text

        logging.info(f"New token {self.token}")
        return self.token


    def __authorize(self):
        logging.info("Try to authorize")
        authorize_btn = self.driver.find_element(By.CLASS_NAME, 'btn-jupyter')
        authorize_btn.click()
        sleep(0.5)
        logging.info("Authorize Done")

    def watch_test_file(self, username):
        logging.info(f"Try to check {username}'s work")
        self.driver.get(self.url + f'/user/{username}/notebooks/assignment.ipynb')
        sleep(0.3)
        logging.info(self.driver.current_url)
        if '/oauth2/authorize' in self.driver.current_url:
            logging.error(f"Not authorized to {username} yet")
            self.__authorize()
        sleep(3)
        for entry in self.driver.get_log('browser'):
            logging.info(entry)
        logging.info("Done")


    def __check_and_refresh_user_list(self):
        curret_time = datetime.now()
        diff = curret_time - self.user_upt_time
        logging.info(diff.total_seconds() // 60 > 0)
        if diff.total_seconds() // 60 > 1:
            self.get_user_list()


    def watch_all_user(self):
        logging.info("Watch all users' work")
        self.__check_and_refresh_user_list()
        for user in self.user_list:
            username = user['name']
            if user['admin']:
                continue
            try:
                self.watch_test_file(username)
            except InvalidSessionIdException as e:
                logging.error("Session Error", exc_info=e)
                self.re_run_driver()
                self.do_login('sub-admin', WATCHER_PASSWORD)
                self.get_user_list()
            except Exception as e:
                logging.error(f'Unexpected Error in {username}', exc_info=e)
            sleep(1)
        logging.info("Watching all users' work done")
        self.refresh_session_serverv()


    def re_run_driver(self):
        logging.info("Close Driver")
        try:
            self.driver.close()
        except InvalidSessionIdException as e:
            logging.error("Session not available")
        logging.info("Rerun Driver")
        self.driver = webdriver.Chrome(executable_path="/usr/src/chrome/chromedriver",
                                       options=self.__get_chrome_options())
        logging.info("Rerun Driver Done")


    def refresh_session_server(self):
        logging.info("Refreshing session server")
        control_panel_btn = self.driver.find_element(By.ID, 'jupyterhub-control-panel-link')
        control_panel_btn.click()
        sleep(1)
        try:
            stop_btn = self.driver.find_element(By.ID, 'stop')
            stop_btn.click()
            sleep(5)
            start_btn = self.driver.find_element(By.ID, 'start')
            start_btn.click()
        except NoSuchElementException as e:
            logging.warn("No stop button, seems server not running")
        logging.info("Refreshing session server Done")
        sleep(1)


if __name__ == '__main__':
    sleep(5)

    # Check for admin account
    admin_test = Tester(HOST)
    admin_test.do_login(ADMIN_ID, ADMIN_PASSWORD)

    test = Tester(HOST)
    test.do_login(WATCHER_ID, WATCHER_PASSWORD)
    test.get_user_list()

    while True:
        try:
            while True:
                test.watch_all_user()
                sleep(1)
        except Exception as e:
            logging.error("Error", exc_info=e)
        finally:
            test.re_run_driver()
            test.do_login(WATCHER_ID, WATCHER_PASSWORD)
            test.get_user_list()
