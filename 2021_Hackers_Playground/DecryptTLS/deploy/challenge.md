# TLS 1.3 client

TLS 1.3 handshake과정을 통해, www.google.com 서버의 443포트에서 제공하고 있는 HTTPS 서비스에 접속하여 GET 요청을 하고 이에 대한 정상적인 응답을 받는 것을 목표로 한다.

### TCP 연결

* www.google.com 의 443 포트에 TCP 연결을 만든다.

```python
host = "www.google.com"
port = 443
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((host, port))
```

### client_hello 메시지 송신

* TLS 1.3이 TLS 1.2와 다른 점은 hello 메시지에 key_share를 넣어 hello 메시지의 교환만으로 키를 합의하고 그 후 남은 handshake 과정을 암호화해서 통신한다는 점이다.
* 아래와 같이 되도록 간결하게 만든 client_hello 메시지를 보내 서버와 TLS handshake를 시작한다.
* 랜덤하게 생성해야하는 client_random을 널바이트 32개로 고정한다.
* 랜덤하게 생성해야하는 key_share를 타원곡선 secp256r1의 generator인 `(48439561293906451759052585252797914202762949526041747995844080717082404635286, 36134250956749795798585127919587881956611106672985015071877198253568414405109)`로 고정한다. 이 값은 표준 [SEC 2](https://www.secg.org/sec2-v2.pdf)에 명시되어 있는 잘 알려져 있는 상수이다.
* 이렇게 하면 client의 secret을 1로 잡는 것과 마찬가지이므로 나중에 server가 주는 key_share가 그대로(정확하게는 x좌표 32바이트) ecdh_shared_secret이 된다.

```
version                         : TLS 1.2
random                          : 0000000000000000000000000000000000000000000000000000000000000000
session_ID_length               : 0
session_ID                      : 
cipher_suites_length            : 12
cipher_suites                   : ['TLS_AES_128_GCM_SHA256', 'TLS_AES_256_GCM_SHA384', 'TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256', 'TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384', 'TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256', 'TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384']
compression_methods_length      : 1
compression_methods             : ['NULL']
extensions_length               : 137
list_of_extensions
A list of 7 dictionaries
------------------------------------------------------------------------------------------------------------------------
    type                            : server_name
    length                          : 19
    data_of_type                    : ['www.google.com']
------------------------------------------------------------------------------------------------------------------------
    type                            : supported_groups
    length                          : 4
    data_of_type                    : ['secp256r1']
------------------------------------------------------------------------------------------------------------------------
    type                            : ec_point_formats
    length                          : 2
    data_of_type                    : ['uncompressed']
------------------------------------------------------------------------------------------------------------------------
    type                            : session_ticket
    length                          : 0
    data_of_type                    : 
------------------------------------------------------------------------------------------------------------------------
    type                            : signature_algorithms
    length                          : 8
    data_of_type                    : ['ecdsa_secp256r1_sha256', 'rsa_pss_rsae_sha256', 'rsa_pkcs1_sha256']
------------------------------------------------------------------------------------------------------------------------
    type                            : supported_versions
    length                          : 5
    data_of_type                    : ['TLS 1.3', 'TLS 1.2']
------------------------------------------------------------------------------------------------------------------------
    type                            : key_share
    length                          : 71
    data_of_type                    : [('secp256r1', (48439561293906451759052585252797914202762949526041747995844080717082404635286, 36134250956749795798585127919587881956611106672985015071877198253568414405109))]
```


### server_hello 메시지 수신

* 아래와 같은 server_hello 메시지를 서버로부터 받는다.

```
version                         : TLS 1.2
random                          : 34e2af2f7c45ae218aecc7dcee69147f7add13970df54bae0f8c5fe40c58bfb9
session_ID_length               : 0
session_ID                      : 
cipher_suite                    : TLS_AES_128_GCM_SHA256
compression_method              : NULL
extensions_length               : 79
list_of_extensions
A list of 2 dictionaries
------------------------------------------------------------------------------------------------------------------------
    type                            : key_share
    length                          : 69
    data_of_type                    : ('secp256r1', (34505705120791959922243167741731098125706790969398927040701653022736414067424, 46835911507207088295931903857901970333239020476632120018816323831955793938514))
------------------------------------------------------------------------------------------------------------------------
    type                            : supported_versions
    length                          : 2
    data_of_type                    : TLS 1.3
```

* client의 의도대로 키교환은 타원곡선 secp256r1을 이용한 Elliptic Curve Diffie-Hellman 알고리즘, 프로토콜 버전은 TLS 1.3, cipher_suite는 `TLS_AES_128_GCM_SHA256`으로 결정되었다.
* 따라서, 키유도에 사용할 해시함수는 `SHA256`이고 암호화에 사용할 알고리즘은 `AES-128-GCM`이다.

```python
hash_algo = "sha256"
hash_len = 32
key_len = 16
iv_len = 12
tag_len = 16
```

### 키 유도

* server_hello 메시지의 key_share를 이용해 ecdh_shared_secret를 구하고 [RFC 8446](https://datatracker.ietf.org/doc/html/rfc8446#section-7.1)를 참고하여 다음의 키들을 유도한다.

```python
import hashlib, hmac

def HKDF_Extract(salt, ikm, hash_algo):
    n = hashlib.new(hash_algo).digest_size
    if salt == b"": salt = b"\x00"*n
    return hmac.new(salt, ikm, hash_algo).digest()

def HKDF_Expand(prk, info, l, hash_algo):
    n = hashlib.new(hash_algo).digest_size
    assert len(prk) >= n and l <= 255*n
    if type(info) == str: info = info.encode("ascii")
    t = bytes(); cnt = 1
    while len(t) < l:
        s = t[-n:] + info + bytes([cnt]); cnt += 1
        t += hmac.new(prk, s, hash_algo).digest()
    return t[:l]

def TLS_HKDF_Expand_Label(secret, label, context, length, hash_algo):
    if type(label) == str: label = label.encode("ascii")
    label = b"tls13 " + label
    info = length.to_bytes(2, "big", signed=False) + bytes([len(label)]) + label + bytes([len(context)]) + context
    return HKDF_Expand(secret, info, length, hash_algo)

def TLS_DeriveSecret(secret, label, message, hash_algo):
    if type(label) == str: label = label.encode("ascii")
    digest = hashlib.new(hash_algo, message).digest()
    n = len(digest)
    return TLS_HKDF_Expand_Label(secret, label, digest, n, hash_algo)

zero_bytes = b"\x00"*hash_len
initial_salt = zero_bytes
pre_shared_secret = zero_bytes
server_key_share = (34505705120791959922243167741731098125706790969398927040701653022736414067424, 46835911507207088295931903857901970333239020476632120018816323831955793938514)
ecdh_shared_secret = server_key_share[0].to_bytes(32, "big", signed=False)
early_secret = HKDF_Extract(initial_salt, pre_shared_secret, hash_algo)
early_derived_salt = TLS_DeriveSecret(early_secret, "derived", b"", hash_algo)
handshake_secret = HKDF_Extract(early_derived_salt, ecdh_shared_secret, hash_algo)
handshake_derived_salt = TLS_DeriveSecret(handshake_secret, "derived", b"", hash_algo)
master_secret = HKDF_Extract(handshake_derived_salt, zero_bytes, hash_algo)

ecdh_shared_secret: 4c498978adb0dbdff8435373188f1dc1bb6cb00bd4ddc4e666bb6be71f0acae0 32 bytes
early_secret: 33ad0a1c607ec03b09e6cd9893680ce210adf300aa1f2660e1b22e10f170f92a 32 bytes
handshake_secret: 2cd8d04a632835ef369f36feeae02af68b1f4ed4ed8f3679cfc688faf7c8b131 32 bytes
master_secret: bc0c12e2a76512bd34429d928ee61b9469f0195831034021611fc2657afec8d2 32 bytes
```

* 아직 handshake가 완료되지 않았으므로 남은 handshake 과정에서 client와 server가 주고 받을 데이터를 암호화하는데 실제 사용될 키를 유도한다. 이 때 `message`는 헤더를 제외한 client_hello 메시지와 server_hello 메시지이다.

```python
client_handshake_traffic_secret = TLS_DeriveSecret(handshake_secret, "c hs traffic", message, hash_algo)
server_handshake_traffic_secret = TLS_DeriveSecret(handshake_secret, "s hs traffic", message, hash_algo)

client_handshake_enc_key = TLS_HKDF_Expand_Label(client_handshake_traffic_secret, "key", b"", key_len, hash_algo)
client_handshake_enc_iv = TLS_HKDF_Expand_Label(client_handshake_traffic_secret, "iv", b"", iv_len, hash_algo)
client_handshake_hmac_key = TLS_HKDF_Expand_Label(client_handshake_traffic_secret, "finished", b"", hash_len, hash_algo)
server_handshake_enc_key = TLS_HKDF_Expand_Label(server_handshake_traffic_secret, "key", b"", key_len, hash_algo)
server_handshake_enc_iv = TLS_HKDF_Expand_Label(server_handshake_traffic_secret, "iv", b"", iv_len, hash_algo)
server_handshake_hmac_key = TLS_HKDF_Expand_Label(server_handshake_traffic_secret, "finished", b"", hash_len, hash_algo)

client_handshake_enc_key: 1055a6c70c3216a737c56134bc6430ed 16 bytes
client_handshake_enc_iv: a391b0a0bfc077ba61ab35d6 12 bytes
client_handshake_hmac_key: 9eec25e544910f986f2b0b11db97559c17d4f9916ed1d5ce3cf98edc6c0a3cff 32 bytes
server_handshake_enc_key: 957a4d6431453260c019e7f67a1245d0 16 bytes
server_handshake_enc_iv: dd89adcd6740475cfef1f91c 12 bytes
server_handshake_hmac_key: 3a0cdfb2228a505b74b162d2a5126be716efc76c5afc06597318f59d5004a7db 32 bytes
```


### change_cipher_spec 메시지 수신

* server가 남은 handshake과정에서 사용할 키를 유도했으며 이후 server가 보내는 hasndshake 메시지는 암호화되어 있다는 것을 의미한다.

### 암호화된 handshake 메시지 수신

* server_handshake_enc_key와 server_handshake_enc_iv를 이용해 다음과 같이 암호화된 handshake 메시지를 복호화한다.

```python
from Cryptography.BlockCipher import AES

key = server_handshake_enc_key
recv_seq = 0
iv = XOR(server_handshake_enc_iv, recv_seq.to_bytes(iv_len, "big", signed=False)); recv_seq += 1
ad = encrypted_data[:5]
ct = encrypted_data[5:-tag_len]
tag = encrypted_data[-tag_len:]
pt = AES.Decrypt_GCM(key, iv, ct, ad, tag)
decrypted_data = pt[-1:] + b"\x03\x03" + (len(pt)-1).to_bytes(2, "big", signed=False) + pt[:-1]
```

* 복호화하면 다음과 같은 handshake 메시지들이 들어 있다. 여기서는 client가 server를 신뢰하기로 하고 검증은 생략한다. 하지만 복호화된 데이터 자체는 키 유도하는 과정에 사용된다.

#### ecrypted_extensions 메시지

#### certificate 메시지

#### certificate_verify 메시지

#### (server) finished 메시지

* 지금까지의 handshake 과정의 무결성을 확인하고 server 쪽 handshake가 완료되었으며 이후 server가 보내는 application_data는 암호화되어 있음을 의미한다.

### 키 유도

* server_finished 메시지를 받으면 handshake 완료 후 server와 client가 주고 받는 application_data를 암호화하는데 사용될 키를 유도할 수 있다. 이 때 `message`는 헤더를 제외한 client_hello, server_hello, encrypted_extension, certificate, certificate_verify, (server) finished 메시지이다.

```python
client_application_traffic_secret = TLS_DeriveSecret(master_secret, "c ap traffic", message, hash_algo)
server_application_traffic_secret = TLS_DeriveSecret(master_secret, "s ap traffic", message, hash_algo)

client_application_enc_key = TLS_HKDF_Expand_Label(client_application_traffic_secret, "key", b"", key_len, hash_algo)
client_application_enc_iv = TLS_HKDF_Expand_Label(client_application_traffic_secret, "iv", b"", iv_len, hash_algo)
server_application_enc_key = TLS_HKDF_Expand_Label(server_application_traffic_secret, "key", b"", key_len, hash_algo)
server_application_enc_iv = TLS_HKDF_Expand_Label(server_application_traffic_secret, "iv", b"", iv_len, hash_algo)

client_application_enc_key: 4912bdefed3aa8e94085c94e94d852f6 16 bytes
client_application_enc_iv: a858db0848c59dc3229e56e2 12 bytes
server_application_enc_key: 17c34dac5d8c6d1b515f6a757e9596cd 16 bytes
server_application_enc_iv: b917fde4be9e8d61e3014577 12 bytes
```


### change_cipher_spec 메시지 송신

* 이제 client가 키를 유도했으니 이후 client가 보내는 메시지는 암호화되어 있다는 것을 server에게 알린다.
* change_cipher_spec 메시지는 6바이트 `140303000101`로 고정되어 있다.

### (client) finished 메시지 송신

* 지금까지의 handshake 과정의 무결성을 증명하기 위해 다음과 같이 client_finished 메시지를 만든다. 이 때 `message`는 헤더를 제외한 client_hello, server_hello, encrypted_extension, certificate, certificate_verify, (server) finished 메시지이다.

```python 
hashed_message = hashlib.new(hash_algo, message).digest()
client_finished = hmac.new(client_handshake_hmac_key, hashed_message, hash_algo).digest()

client_finished: f02f2a5a05991dd6b9a9ab2eaf93925432c2a3476931415af03100a523a02010 32 bytes
```

* client_finished 메시지를 아래와 같이 암호화한 다음 server로 보내 handshake 과정을 완료한다.

```
key = client_handshake_enc_key
send_seq = 0
iv = XOR(client_handshake_enc_iv, send_seq.to_bytes(iv_len, "big", signed=False)); send_seq += 1
ad = b"\x17\x03\x03" + (len(client_finished_data)+tag_len-4).to_bytes(2, "big", signed=False)
pt = client_finished_data[5:] + client_finished_data[0:1]
(ct, tag) = AES.Encrypt_GCM(key, iv, pt, ad)
encrypted_client_finished_data = ad + ct + tag
```

### HTTP 요청 메시지 송신

* 이제 HTTP 요청 메시지 암호화해서 server에 보낸다.

```python
request = """GET / HTTP/1.1\r\n\
Host: {host}\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0\r\n\
Flag: SCTF{RFC8446:The_Transport_Layer_Security_(TLS)_Protocol_Version_1.3_63a3a9e1}\r\n\
\r\n"""
application_data = b"\x17\x03\x03" + len(request).to_bytes(2, "big", signed=False) + request
key = client_application_enc_key
send_seq = 0
iv = XOR(client_application_enc_iv, send_seq.to_bytes(iv_len, "big", signed=False)); send_seq += 1
ad = b"\x17\x03\x03" + (len(data)+tag_len-4).to_bytes(2, "big", signed=False)
pt = application_data[5:] + application_data[0:1]
(ct, tag) = AES.Encrypt_GCM(key, iv, pt, ad)
encrypted_application_data = ad + ct + tag
```


### HTTP 응답 메시시 수신

* server가 보내오는 applacation_data 메시지들을 복호화하면 client 보낸 요청에 대한 server의 응답을 얻을 수 있다.

```
HTTP/1.1 200 OK
Date: Fri, 23 Jul 2021 04:07:48 GMT
Expires: -1
Cache-Control: private, max-age=0
Content-Type: text/html; charset=UTF-8
Strict-Transport-Security: max-age=31536000
P3P: CP="This is not a P3P policy! See g.co/p3phelp for more info."
Server: gws
X-XSS-Protection: 0
X-Frame-Options: SAMEORIGIN
Set-Cookie: 1P_JAR=2021-07-23-04; expires=Sun, 22-Aug-2021 04:07:48 GMT; path=/; domain=.google.com; Secure; SameSite=none
Set-Cookie: NID=219=w4J2IBMlD67n_1OgU1pokbI_Z8JktNoxK3ORG36Drgv2lVqLd2lhi90gFnQQw-x2SpgeZ1XySidmlQHcPSZNYqgqH9HkiEy4k8U5ofNyOUODXW2UK7y3YBWp-O6rD0NIvcO9mIYN80BpOlaJpglmPwBzWNZ_ktDW0F7VyXqd4CY; expires=Sat, 22-Jan-2022 04:07:48 GMT; path=/; domain=.google.com; Secure; HttpOnly; SameSite=none
Alt-Svc: h3=":443"; ma=2592000,h3-29=":443"; ma=2592000,h3-T051=":443"; ma=2592000,h3-Q050=":443"; ma=2592000,h3-Q046=":443"; ma=2592000,h3-Q043=":443"; ma=2592000,quic=":443"; ma=2592000; v="46,43"
Accept-Ranges: none
Vary: Accept-Encoding
Transfer-Encoding: chunked

3874
<!doctype html><html itemscope="" itemtype="http://schema.org/WebPage" lang="ko"><head><meta charset="UTF-8"><meta content="origin" name="referrer"><meta content="/images/branding/googleg/1x/googleg_standard_color_128dp.png" itemprop="image"><link href="/manifest?pwa=webhp" crossorigin="use-credentials" rel="manifest"><title>Google</title>
...
```
