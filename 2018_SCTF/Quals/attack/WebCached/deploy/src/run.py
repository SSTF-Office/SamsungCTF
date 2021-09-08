#!/usr/bin/env python2
from redis import Redis
from flask import Flask, request, render_template
from flask import session, redirect, url_for, abort
from session_interface import RedisSessionInterface
import socket
import urllib


r = Redis()
app = Flask(__name__)
app.session_interface = RedisSessionInterface()
timeout = socket.getdefaulttimeout()


def cached(url):
    key = '{}:{}'.format(request.remote_addr, url)
    resp = r.get(key)
    if resp is None:
        resp = load_cache(url)
        r.setex(key, resp, 3)
    return resp


def load_cache(url):
    def get(url):
        return urllib.urlopen(url).read()
    socket.setdefaulttimeout(0.5)
    try:
        resp = get(url)
    except socket.timeout:
        resp = '{} may be dead...'.format(url)
    except Exception as e:
        resp = str(e)
    socket.setdefaulttimeout(timeout)
    return resp


@app.route('/view')
def view():
    url = session.get('url', None)
    if url is not None:
        session.pop('url')
        return cached(url)
    else:
        return redirect(url_for('main'))


@app.route('/', methods=['GET', 'POST'])
def main():
    if request.method == 'GET':
        return render_template('main.html')
    else:
        url = request.form.get('url', None) or abort(404)
        session['url'] = url
        return redirect(url_for('view'))


if __name__ == '__main__':
    app.run(port=12000, host='0.0.0.0', debug=True)
