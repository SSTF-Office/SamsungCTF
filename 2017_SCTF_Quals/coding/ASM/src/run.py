#-*- coding: utf-8 -*-
from front import *
from datetime import timedelta
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

app = Flask(__name__, static_url_path='/static')
app.secret_key = "xe9m?j\xa8\x84%D;\x7f5e\\\xea\xca\x1e\x96k\x05F\xaf\xe4:\xe6\xd7>sX\xa0\x9a\xdf\xb0"

app.register_blueprint(frontend)

@app.before_request
def make_session_permanent():
    app.permanent_session_lifetime = timedelta(seconds=timeout)

if __name__ == "__main__":
    app.run(host = '0.0.0.0', port=10000, debug=0)
