import os
import io
import re
import time
import string
import hashlib
from flask import Flask, render_template, request, session, redirect, flash, jsonify, send_file
from functools import wraps

from config import secret_key, admin_hash, course_data
from cert import make_certificate

app = Flask(__name__, static_url_path="/static")
app.secret_key = secret_key
app.config['SESSION_COOKIE_NAME'] = 'EduSession'

def login_required(f):
    @wraps(f)
    def decorator(*args, **kwargs):
        if 'name' not in session:
            flash("Login Required")
            return redirect('/')
        return f(*args, **kwargs)
    return decorator

def check_email(email):
    regex = '[A-Za-z0-9._+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}'
    if re.match(regex, email) == None:
        flash("Invalid Email")
        return False
    return True


def check_name(name):
    charset = string.ascii_letters + ' '
    for c in name:
        if c not in charset:
            flash("Invalid Name")
            return False
    return True


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/signin', methods=['POST'])
def signin():
    try:
        name = request.form['name']
        email = request.form['email']
        if check_name(name) and check_email(email):
            session.clear()
            session['name'] = name
            session['email'] = email
            session['is_admin'] = hashlib.md5(name.encode()).hexdigest() == admin_hash
            session['idx'] = 0
            return redirect('/main')
    except:
        pass
    return redirect('/')


@app.route('/main')
@login_required
def main():
    if session['idx'] >= len(course_data['vids']):
        flash('You finished the course! Get the certificate!')
    return render_template('main.html', data=course_data, idx=session['idx'])


@app.route('/status', methods=['POST'])
@login_required
def status():
    result = {}
    try:
        video = course_data['vids'][session['idx']]
        params = request.get_json()
        action = params['action']
        if action == 'start':
            session['start'] = time.time()
        elif action == 'finish':
            rate = float(params['rate'])
            if rate > 1.5:
                result['msg'] = 'Why are you so fast?'
            else:
                passed_time = time.time() - session['start']
                if (video['length'] / rate) < passed_time + 3:
                    session.pop('start', None)
                    session['idx'] += 1
                    result['msg'] = 'Good Job!'
                else:
                    result['msg'] = 'Do not skip video!'
    except:
        result['msg'] = 'error'
    return jsonify(result)


@app.route('/cert')
@login_required
def cert():
    if session['idx'] < len(course_data['vids']):
        return 'No!'

    pdf = make_certificate(session['name'], session['email'],
                           course_data['name'], course_data['author'])
    return send_file(
        io.BytesIO(pdf),
        mimetype='application/pdf')


@app.route('/flag')
@login_required
def flag():
    ### CTF stuff ###
    if session['is_admin']:
        flash(os.popen('cat flag*').read().strip())
    else:
        flash("you are not an admin, {}".format(session['name']))
    return redirect('/')


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=9999, debug=False)
