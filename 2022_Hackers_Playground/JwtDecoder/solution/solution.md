# JwtDecoder Solution

As used version ejs node module have unpatched rce vulnerability, you can leverage the vulnerability to get shell.

node cookie-parser module perform `JSON.parse` operation with `j:` prefixed cookie value. With this cookie payload, user can leverage ejs RCE vulnerability.

Exploit code is enclosed in `solve.py`.