# Webpage as a Service

## Description
Webpage hosting service!

http://sctfwaas.com:8080

Make your webpage!

## Comment
Source files (backend.py, frontend.py, templates) are given.

Additional file (admin.py) is given as a hint.

Flag is in **config.py**, which is not provided.

<details>
  <summary> Vulnerability is ... (<b>SPOILER!</b>) </summary>
  <p>

The main idea of this challenge is from this [paper](http://www.icir.org/vern/papers/host-of-troubles.ccs16.pdf)

The front-end server is a simple HTTP server while the back-end server is based on the flask.

Since host header of HTTP protocol is handled differently in the front-end and back-end,

the attacker can bypass the firewall, and perform cache poisoning attack.
  </p>
</details>

## How to Run.
A domain is needed.

Any subdomain should be connected to the same server.

Modify domain name and port number (default: 8080) in **config.py** file.

in deploy directory,

```/bin/sh
$ docker build -t waas .
$ docker run -p 8080:8080 -d waas
```

## Requirements
python3

pip3 install flask

pip3 install robobrowser # for admin.py

