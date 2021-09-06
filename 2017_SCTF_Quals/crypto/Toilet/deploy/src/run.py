# My New Session Interface
from flask.sessions import SessionInterface, SecureCookieSession
from flask import Flask, request, session, Response, redirect, url_for
from config import flag
from Crypto.Util.number import bytes_to_long as s2i
from Crypto.Util.number import long_to_bytes as i2s
from Crypto.Cipher import AES
from Crypto.Hash import MD5
from base64 import b64encode, b64decode
import time
import json


def gf_2_128_mul(x, y):
    # GF(2^128) : 1 + a + a^2 + a^7 + a^128
    assert (x | y) < (1 << 128)
    res = 0
    for i in range(128):
        if y & (1 << i):
            res ^= x
        x = x << 1
        if x & 0x100000000000000000000000000000000L:
            x ^= 0x100000000000000000000000000000087L
    return res


class WegmanCarter():
    def __init__(self, key):
        self.key = MD5.new(key).digest()
        self.prp = AES.new(self.key).encrypt

    def uHash(self, message):
        res = 0
        plen = 16 - (len(message) % 16)
        message += chr(plen) * plen
        for i in xrange(len(message) / 16):
            blk = s2i(message[16 * i: 16 * i + 16])
            res = gf_2_128_mul(blk ^ res, s2i(self.key))
        return res

    def sign(self, message, nonce):
        assert len(nonce) == 16
        return i2s(self.uHash(message) ^ s2i(self.prp(nonce))).ljust(16, '\0')


class MySigningSerializer():
    def __init__(self, app):
        self.key = app.secret_key
        self.signer = WegmanCarter(self.key)

    def dumps(self, obj):
        nonce = time.asctime()[:16]
        res = json.dumps(obj)
        res = nonce + res + self.signer.sign(res, nonce)
        return b64encode(res)

    def loads(self, val):
        obj = b64decode(val)
        nonce, obj, tag = obj[:16], obj[16:-16], obj[-16:]
        assert self.signer.sign(obj, nonce) == tag
        return json.loads(obj)


class MySessionInterface(SessionInterface):
    session_class = SecureCookieSession

    def get_signing_serializer(self, app):
        if not app.secret_key:
            return None
        return MySigningSerializer(app)

    def open_session(self, app, request):
        s = self.get_signing_serializer(app)
        if s is None:
            return None
        val = request.cookies.get(app.session_cookie_name)
        if not val:
            return self.session_class()
        try:
            data = s.loads(val)
            return self.session_class(data)
        except:
            return self.session_class()

    def save_session(self, app, session, response):
        domain = self.get_cookie_domain(app)
        path = self.get_cookie_path(app)

        # Delete case.  If there is no session we bail early.
        # If the session was modified to be empty we remove the
        # whole cookie.
        if not session:
            if session.modified:
                response.delete_cookie(app.session_cookie_name,
                                       domain=domain, path=path)
            return

        # Modification case.  There are upsides and downsides to
        # emitting a set-cookie header each request.  The behavior
        # is controlled by the :meth:`should_set_cookie` method
        # which performs a quick check to figure out if the cookie
        # should be set or not.  This is controlled by the
        # SESSION_REFRESH_EACH_REQUEST config flag as well as
        # the permanent flag on the session itself.
        if not self.should_set_cookie(app, session):
            return

        httponly = self.get_cookie_httponly(app)
        secure = self.get_cookie_secure(app)
        expires = self.get_expiration_time(app, session)
        val = self.get_signing_serializer(app).dumps(dict(session))
        response.set_cookie(app.session_cookie_name, val,
                            expires=expires, httponly=httponly,
                            domain=domain, path=path, secure=secure)


# Start Web Application
app = Flask(__name__)
app.secret_key = flag
app.session_interface = MySessionInterface()


Image = open("image.gif").read().encode("base64")


@app.route("/")
def hi():
    name = session.get("name")
    isAdmin = session.get("is_admin")
    if isAdmin:
        return "Congratulation! Here is flag : %s" % (flag, )
    elif name is not None:
        return """hi %s<br><button onclick="location.href='./logout'"type="submit">logout</button>&nbsp;&nbsp;<button onclick="location.href='./get_source'"type="submit">source</button>        <img src="data:image/png;base64,%s" style="width:30%%;position:fixed;bottom: 100px; right:100px"></img>
""" % (name, Image)
    else:
        return redirect(url_for("login"))


@app.route("/login/")
def login():
    name = request.args.get("name")
    if session.get("name"):
        return redirect(url_for("hi"))
    elif name is not None:
        session["name"] = name
        session["is_admin"] = False
        return redirect(url_for("hi"))
    return """<form><span>Your id:</span>&nbsp;&nbsp;<input name="name"></input><button type="submit">submit</button></form>"""


@app.route("/logout")
def logout():
    for key in session.keys():
        session.pop(key)
    return redirect(url_for("login"))


@app.route("/get_source")
def get_source():
    with open(__file__, 'r') as f:
        res = f.read()
    return Response(res, mimetype="text/plain")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80, debug=False)

