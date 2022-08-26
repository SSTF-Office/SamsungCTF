from flask import Flask, request, session, redirect, render_template, url_for
from flask_session import Session
from datetime import timedelta, datetime
from os import environ as env
from random import randint, choice
from pytz import UTC

timeout_seconds = 60
rounds_max = 30
rng = 0xfffff

app = Flask(__name__)
app.secret_key = env['FLASK_SECRET_KEY'] if 'FLASK_SECRET_KEY' in env else 'so_secret_key'
#app.permanent_session_lifetime = timedelta(minutes = 30)
app.config["SESSION_PERMANENT"] = timedelta(minutes = 10)
app.config["SESSION_TYPE"] = "filesystem"
app.config["SESSION_FILE_DIR"] = "/tmp/"

Session(app)

def now():
	return UTC.localize(datetime.now())

@app.route('/')
def index():
	#if 'timeout' not in session:
	#	return redirect(url_for('chal'))
	if 'timeout' in session:
		session.pop('timeout')

	return render_template('index.html', rounds=rounds_max, timeout=timeout_seconds)

@app.route('/chal', methods=["POST", "GET"])
def chal():
	msg = ''
	mn = request.form.get('min', 'A')
	mx = request.form.get('max', 'A')

	if 'timeout' not in session:
		session['challenge'] = genChallenge(rounds_max)
		session['round'] = 0
		session['timeout'] = now() + timedelta(seconds = timeout_seconds)
	elif now() > session['timeout']:
		return redirect(url_for('gameover'))
	elif (mn, mx) != ('A', 'A'):
		try:
			if (int(mn), int(mx)) == session['challenge'][session['round']][1]:
				session['round'] += 1
				if session['round'] == rounds_max:
					session.pop('timeout')
					return render_template('flag.html', flag=env['FLAG'] if 'FLAG' in env else 'SCTF{Sample_Flag}')
			else:
				msg = "Ooops, please think harder =)"
		except:
			msg = 'No hack.'

	ctxt = genChalStr(session['challenge'][session['round']][0])
	ctxt['round'] = session['round'] + 1
	ctxt['msg'] = msg

	return render_template('chal.html', **ctxt)

@app.route('/gameover')
def gameover():
	return render_template('gameover.html')

def f(c, x):
	return c[0] * (x ** 5) + c[1] * (x ** 4) + c[2] * (x ** 3) + c[3] * (x * x) + c[4] * x + c[5]

def genChalStr(chal):
	nums, mn, mx = chal[:-2], chal[-2], chal[-1]
	nums = ["{}x^{}".format(i, idx) if idx > 1 else "{}x".format(i) if idx == 1 else str(i) for (idx, i) in enumerate(nums[::-1])]
	ret = "y = " + " + ".join(nums[::-1]).replace("+ -", "- ")

	return {'challenge': ret, 'mn':mn, 'mx':mx}

def genChallenge(siz):
	chals = []
	while len(chals) < siz:
		a = randint(-rng, rng)
		b = randint(-rng, rng)
		c = randint(-rng, rng)
		d = randint(-rng, rng)

		if -(a + b + c + d) % 4 != 0 or (a*b + a*c + b*c + a*d + b*d + c*d) % 3 != 0 or -(a*b*c + a*b*d + a*c*d + b*c*d) % 2 != 0:
		 continue

		k = randint(-1000, 1000)
		n0 = k
		n1 = k * 5 * -(a + b + c + d) // 4
		n2 = k * 5 * (a*b + a*c + b*c + a*d + b*d + c*d) // 3
		n3 = k * 5 * -(a*b*c + a*b*d + a*c*d + b*c*d) // 2
		n4 = k * 5 * a*b*c*d
		n5 = randint(-rng, rng)

		while True:
			l = randint(min(a, b, c, d) - 100, max(a, b, c, d) + 100)
			m = randint(min(a, b, c, d) - 100, max(a, b, c, d) + 100)

			if l != m:
				break

		if l > m:
			l, m = m, l

		chal = [n0, n1, n2, n3, n4, n5]

		sol = [f(chal, x)  for x in [a, b, c, d, l, m] if l <= x <= m]

		chals.append((chal + [l, m], (min(sol), max(sol))))

	return chals
