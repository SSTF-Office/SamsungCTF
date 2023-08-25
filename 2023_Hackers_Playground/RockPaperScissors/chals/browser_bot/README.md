## How to run
```
$ docker build --tag xssbot .
$ docker run --cap-add=SYS_ADMIN -p 1338:1338 -e DOMAIN="DOMAIN" xssbot
```