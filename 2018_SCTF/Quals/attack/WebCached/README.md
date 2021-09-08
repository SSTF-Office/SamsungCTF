# WebCached

## Description

Cache Your Favorite Page @ WebCached

## How to Solve?

0. Get source

You can get source codes of service starting from `file:///proc/self/cmdline`.

1. CR-LF injection

Using CR-LF injection in urllib, you can poisoning redis.

Payload: `http://127.0.0.1\r\n SET <target> <value>\r\n :6379/`

2. RCE with redis session

By poisoning session, you can freely modify session.

Since session manager use pickle to serialize session with redis,
You can do RCE attack.
