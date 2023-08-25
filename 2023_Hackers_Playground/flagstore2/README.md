## Description

this is a simple web application that uses keycloak as an authentication server.<br />
the application is vulnerable to open redirect.<br />
then, you can manipulate the auth_server parameter in login.php.<br />
malicious auth_server will return the token which can manipulate HTTP request header. <br />

## Vulnerability

Open redirect in logout.php

## Deployment

1. chagne URL and PORT in `./tools/make_deploy.sh`

2. `make`

3. `docker-compose -f deploy/src/docker-compose.yml up --build --detach`

4. distribute the file `dist/src.tar.gz`
