a, b = open("output.txt").read().strip().split("\n")
msg = "RC4 is a Stream Cipher, which is very simple and fast."

a = a.decode("hex")
b = b.decode("hex")

l = min(len(a), len(b))
r = ""
for (c1, c2, c3) in zip(a[:l], b[:l], msg[:l]):
	r += chr(ord(c1) ^ ord(c2) ^ ord(c3))
print r
