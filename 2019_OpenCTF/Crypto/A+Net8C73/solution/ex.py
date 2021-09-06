import hmac, hashlib

def HMAC_SHA1(key, msg, to_bytes=False):
    return hmac.new(key, msg, "sha1").digest() if to_bytes else hmac.new(key, msg, "sha1").hexdigest()

def PRF(key, label, data, length=384):
    n = length//160 if length%160 == 0 else length//160 + 1
    r = bytes()
    data = label.encode("ascii") + b'\x00' + data
    for i in range(0, n):
        r += HMAC_SHA1(key, data + i.to_bytes(1, "big"), to_bytes=True)
    return r

def ToBytesMinMax(hex1, hex2):
    assert len(hex1) == len(hex2) and len(hex1)%2 == 0
    n = len(hex1)//2
    int1 = int(hex1, 16)
    int2 = int(hex2, 16)
    byte1 = int1.to_bytes(n, "big")
    byte2 = int2.to_bytes(n, "big")
    return byte1+byte2 if int1 <= int2 else byte2+byte1

def XOR(byte1, byte2):
    assert len(byte1) == len(byte2)
    n = len(byte1)
    byte = bytes()
    for i in range(0, n):
        byte += (byte1[i] ^ byte2[i]).to_bytes(1, "big")
    return byte

def PBKDF_HMAC_SHA1(password, salt, count, length):
    if type(password) == str: password = password.encode()
    if type(salt) == str: salt = salt.encode()
    n = length//160 if length%160 == 0 else length//160 + 1
    r = bytes()
    for i in range(0, n):
        s = HMAC_SHA1(password, salt+(i+1).to_bytes(4, "big"), to_bytes=True)
        t = s
        for j in range(1, count):
            s = HMAC_SHA1(password, s, to_bytes=True)
            t = XOR(t, s)
        r += t
    return r[:(length//8)]


def AttackUsingPMKID(ssid_ap, addr_ap, addr_sp, pkmid):
    PMKID = bytes.fromhex(pmkid)
    for i in range(10000, 100000):
        password = "SCTF{" + str(i) + "}"
        salt = ssid_ap
        count = 4096
        length = 256
        #PMK = hashlib.pbkdf2_hmac("sha1", password, ssid, count, length//8)
        PMK = PBKDF_HMAC_SHA1(password, salt, count, length)
        data = b'PMK Name' + bytes.fromhex(addr_ap) + bytes.fromhex(addr_sp)
        x = HMAC_SHA1(PMK, data, to_bytes=True)[:16]
        if x == PMKID:
            print(">> Found Password: {}".format(password))
            return password
    return None

ssid_ap = "A+Net8C73"
addr_ap = "000136248c74"
addr_me = "ac2b6e086ca4"
pmkid = "1097e982e6c91c924e0b8d5aa23b9357"

password = AttackUsingPMKID(ssid_ap, addr_ap, addr_me, pmkid)

