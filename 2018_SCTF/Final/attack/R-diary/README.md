# R-diary

## Description

Try R-diary and get the secret of robots.

## Comments
Diary service using nodejs + react.js + restful api

1. Using Mongodb injection, Attacker can log in as some user.
2. There is one more function "Inspect", which is inspecting user's diary.
3. When viewing diary, if the diary id(/view/{id}) is not found in list, error prints some message with {id}.
4. Using this, We can do DOM XSS using iframe.
5. Leak the flag in the localStorage using 2, 3.
