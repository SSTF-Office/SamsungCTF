## Yet Another Injection

### Description

SQL is not the only target of injection attacks.

Server: `http://localhost:1337`

If this challenge is too difficult, please revisit Web 101 and Web 102.
The principle is the same as SQLi.

### Deploy
```
cd deploy/server
docker-compose up -d --build
```

### Solution

We can see the source code.

When we request the paper list, it only retrieves papers that `published` is `yes`.

The detail information of a specific paper is provided by `paperdetail.php` and there's a XPATH Injection vulnerability.

We can get the flag by finding the abstract of a paper whose `published` property is `no`.


```
[URL]
http://yai.sstf.site/paperdetail.php?idx=A' or @published='no' or 'a'='b

[XPATH query]
/Papers/Paper[Idx/text()='A' or @published='no' or 'a'='b' and @published='no']
```
