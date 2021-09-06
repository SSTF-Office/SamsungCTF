# Xero Trust
## Description (in public)
I feel like someone eavesdrops on me.

So I made my own secure messenger service and now nobody can do it.

Have a secret talk with your flag-mate!

Recommended browser : Chrome

### Hint #1
In `xero.js`, the following two lines are not matched each other.
```
// line 87:
var plain = new Uint32Array([from, to]).toUint8Array().concat(JSON.stringify({"msg":msg}).encodeutf8());

//line 156:
       return decrypted.toUint8Array().decodeutf8();
```

There was a trap using data encoding.
Some points can be clarified by removing this trap.
```
xero.js line 156:
-       return decrypted.toUint8Array().decodeutf8();
+       return decrypted.slice(0, 8).toUint8Array().decode()+decrypted.slice(8).toUint8Array().decodeutf8();
```

## Caution
 - Instance only supports HTTPS not HTTP

## Flag
 - `SCTF{th3r3_1s_A_r34s0n_n0t_t0_u5e_ECB}`

## Run
 - `cd prob && sudo docker-compose up`

## Re-run
Do not re-run. Each running makes SALT changed, so the data will become waste.
```
cd prob
sudo docker-compose down
sudo docker volume rm $(sudo docker volume ls -q)
sudo docker-compose up -d
```
