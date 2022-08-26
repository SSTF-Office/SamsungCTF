## Online Notepad

### Overview
| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | :star::star:        |
| Category          | Web                 |
| Tags              | Template Injection  |
| Author            | gogil               |

### Deployment
Download and extract: deploy/deploy_onlinenotepad.zip
```
docker-compose up -d
```

Open web browser and access to http://localhost:31547
- attachment required : prob/prob_onlinenotepad.zip

### Prob Description
 > Online Notepad is a free notepad!\
 > Please read the admin's memo.
 > 
 > http://x.x.x.x:31547/
 > 
 > Attachment: prob_onlinenotepad.zip
 > 

### Solving strategy
We can indicator possible SSTI(Server-Side Template Injection) by write `{%~%}` to the memo.\
Then, get shell by chain of templates.\
Please see the onlinenotepad_solve.py

### Flag
SCTF{156e5aaef5fa120f6c1cf3418e912e18}

### Reference
 - https://jinja.palletsprojects.com/en/2.10.x/templates/
