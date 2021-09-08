#-*- coding: utf-8 -*-
from flask import Flask, render_template, abort, request, session, flash, url_for, redirect, Blueprint,send_from_directory
from gener import *
import datetime
import string
import md5
import json
import time

from random import choice

flag = "Samsung{FunWithImageProcessing}"
timeout = 80
frontend = Blueprint('frontend', __name__)

@frontend.route("/", methods=['GET', 'POST'])
def prob200():
    return render_template('main.html')

@frontend.route('/images/<path:path>')
def img(path):
    return send_from_directory('static/images', path)

@frontend.route("/start", methods=['POST'])
def start():
    # remove old files
    from os import listdir, remove
    from os.path import isfile, join, getctime
    for f in listdir('static/images'):
        if isfile(join('static/images', f)):
            if time.time() - getctime(join('static/images', f)) >= timeout:
                remove(join('static/images', f))

    session['val'] = ''.join([choice(string.lowercase) for i in xrange(16)])
    session['correct'] = 0
    session['start'] = int(time.time())
    image_target = md5.new('CAPTOTHECHA%s_%s'%(session['val'],session['correct'])).hexdigest()
    session['nxt'] = gen(1, 'static/images/'+image_target)
    return url_for('frontend.img', path=image_target+".png")

@frontend.route('/check', methods=['POST'])
def check():
    timer = int(time.time()) - session['start']
    if(session['correct'] == -1):
        return "-3"
    elif timer > timeout:
        return "-2"
    elif session['nxt'] == request.form['ans']:
        session['correct'] += 1
        if session['correct'] >= 100 and \
            timer <= timeout:
                return json.dumps({"flag": flag})

        image_target = md5.new('CAPTOTHECHA%s_%s'%(session['val'],session['correct'])).hexdigest()
        leng = session['correct'] + 1
        session['nxt'] = gen(leng if leng < 20 else 20, 'static/images/'+image_target)
        return json.dumps({"url" : url_for('frontend.img', path=image_target+".png"), "stage": session['correct'], "flag": ""})
    else:
        session['correct'] = -1
        return "-1"

