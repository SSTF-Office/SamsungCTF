from flask import Flask, make_response,render_template,flash,request,url_for,redirect,send_from_directory,abort
from passlib.hash import sha256_crypt
from functools import wraps
from binascii import unhexlify, hexlify
from AESCipher import AESCipher
from base64 import b64decode, b64encode
from collections import OrderedDict
from os import urandom, path, environ
from database import User,init_db
from forms import LoginForm, RegistrationForm
import json
import string

def craft_cookie(user):
    js = user.generate_cookie()
    cjs = cipher.encrypt(str(js))
    return cjs

def check_cookie(cookie):
    cookie = cookie.replace("testcookie=","")
    #cookie format
    #{ "username": "", "email": "", "isAdmin" : 1/0, "sessionId" : "" }
    #check /cookietest endpoint if you want to decode your cookie
    js = cipher.decrypt(cookie)
    js = remNonAscii(js)
    j = json.loads(js,object_pairs_hook=OrderedDict)
    return j

def remNonAscii(data):
    output = [c for c in data if c in string.printable and not c.isspace()]
    return ''.join(output)

class myFlask(Flask):
    def process_response(self,response):
        response.headers['Server'] = "LaChupacabra v0.1"
        return response

app = myFlask(__name__)
from werkzeug.debug import DebuggedApplication
app.config['DEBUG'] = True
app.wsgi_app = DebuggedApplication(app.wsgi_app,evalex=False)
app.secret_key = environ['APP_KEY']
cipher = AESCipher(environ['AES_KEY'])

def verify_cookie(cookie):
    res = { "session": False, "admin": False }
    if cookie != None:
        cookie = cookie.replace("testcookie=","")
        js = remNonAscii(cipher.decrypt(cookie))
        j = json.loads(js,object_pairs_hook=OrderedDict)
        dbSession = init_db()
        user = dbSession.query(User).filter(User.username == j['username']).first()
        dbSession.close()
        if user != None:
            if user.sessionId == j['sessionId']:
                res['session'] = True
                if j['isAdmin'] == 1: res['admin'] = True
    return res

def login_required(f):
    @wraps(f)
    def wrap(*args,**kwargs):
        try:
            ses = verify_cookie(request.headers.get("Cookie"))
            if ses['session'] == True:
                        return f(*args,**kwargs)
            else:
                if '/hive' in request.path:
                    abort(404)
            return redirect(url_for('login_page'))
        except Exception:
            abort(404)
    return wrap


@app.route("/logout/")
@login_required
def logout():
    try:
        cook = request.headers.get('Cookie')
        c = check_cookie(cook)
        dbSession = init_db()
        user = dbSession.query(User).filter(User.username == c['username']).first()
        if user is not None:
            if user.sessionId != c['sessionId']:
                flash("Login first!")
            else:
                user.sessionId = None
                dbSession.commit()
                dbSession.close()
                flash("You have been logged out!")
                resp = make_response(render_template('index.html',verify_session=False,admin=False),200)
                resp.delete_cookie('testcookie')
                return resp
        return redirect(url_for('login_page'))
    except Exception:
        flash('Login first!')
        return redirect(url_for('login_page'))

@app.route('/',methods=["GET"])
def index():
    admin=False
    error=""
    cook = request.headers.get("Cookie")
    if cook != None:
        c = check_cookie(cook)
        dbSession = init_db()
        user = dbSession.query(User).filter(User.username == c['username']).first()
        dbSession.close()
        if user is not None:
            if c['sessionId'] == user.sessionId:
                if c['isAdmin'] == 1: admin=True
            else:
                error = 'Invalid cookie'
        else:
            error = "Invalid cookie"
    ses = verify_cookie(cook)
    resp = make_response(render_template("index.html",error=error,verify_session=ses['session'],admin=ses['admin']),200)
    return resp

@login_required
@app.route("/cookietest/")
def test():
    cook = request.headers.get("Cookie")
    if cook == None: return "Give me cookie!"
    c = cook.replace("testcookie=","")
    c = check_cookie(c)
    dbSession = init_db()
    user = dbSession.query(User).filter(User.username == c['username']).first()
    dbSession.close()
    return json.dumps(c)

@app.route('/login/',methods=["GET","POST"])
def login_page():
    error = ""
    try:
        form = LoginForm(request.form)
        if request.method == "POST":
            username = form.username.data
            password = str(form.password.data)
            dbSession = init_db()
            user = dbSession.query(User).filter(User.username == username).first()
            if user != None:
                if sha256_crypt.verify(password,user.password) == True:
                    cookie = craft_cookie(user)
                    dbSession.commit()
                    dbSession.close()
                    resp = make_response(redirect(url_for("hive")))
                    resp.set_cookie('testcookie',cookie)
                    return resp
            error = "Invalid credentials !"
        return render_template("login.html",error=error,form=form)
    except Exception:
        error = "Invalid credentials"
        return render_template("login.html",error=error,form=form)

@app.route("/register/",methods=["GET","POST"])
def register_page():
    try:
        form = RegistrationForm(request.form)
        if request.method == "POST" and form.validate():
            username = form.username.data
            email = form.email.data
            password = form.password.data
            dbSession = init_db()
            user = dbSession.query(User).filter(User.username == username).first()
            if user != None:
                return render_template('register.html',form=form,error='Username already taken!')
            newUser = User(username,password,email)
            dbSession.add(newUser)
            dbSession.commit()
            dbSession.close()
            flash("Thanks for registration. You can now sign in!")
            return render_template('index.html',verify_session=False,admin=False)
        else:
            return render_template('register.html',form=form)
    except Exception as e:
            return str(e)

@app.route("/hive/",methods=["GET"])
@login_required
def hive():
    ses =verify_cookie(request.headers.get("Cookie"))
    return render_template('hive.html',verify_session=ses['session'],admin=ses['admin'])

@app.route("/hive/secret/",methods=["GET"])
@login_required
def secret():
    ses = verify_cookie(request.headers.get("Cookie"))
    return render_template('secret.html',admin=ses['admin'],verify_session=ses['session'])

@app.route("/favicon.ico")
def favicon():
    return send_from_directory(path.join(app.root_path,'static'),'favicon.ico',mimetype='image/vnd.microsoft.icon')

@app.errorhandler(404)
def page_not_found(error):
    return "Not found",404

if __name__ == "__main__":
    app.run(host='0.0.0.0',port=80,debug=True,use_evalex=False)#,ssl_context='adhoc')
