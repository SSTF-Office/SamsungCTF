import hashlib
	
def ComputeSha256Hash(chars):
	value = hashlib.sha256(chars).hexdigest()
	return value

def HashAll(ans, l, pad):
	ans += " " * l
	i = 0
	r = ""
	while (i + l - 1 < len(ans)):
		tmp = ans[i:i+l]
		r += str(ComputeSha256Hash(tmp))
		print('part_hash=',r)
		i += pad
	return r

for i in range(32, 127):
	for j in range(32, 127):
		for k in range(32, 127):
			s = chr(i) + chr(j) + chr(k)
			h = ComputeSha256Hash(s)
			if h == "04befa704259028fe18c2763e1106e7d28104b1f6466010208e1c975b6b47d50":
				s1 = s
				print("hash1=",s, h ,s1)
			elif h == "de9001f3b15feb3dac45816f645f3aa42dfc7760e3fa09fbf2a8a3de85a9eede":
				s2 = s[1:]
				print("hash2=",s, h ,s2)
			elif h == "7b598a595430b4add2f939d8ff9a51ae30d2e9e2188d0e76eae70b3c7db13315":
				s3 = s[1:]
				print("hash3=",s, h ,s3)
			elif h == "a7052319cdb35ea5a1805529c71dcbc3547c2d638d11582388d5d8b46bba3f61":
				s4 = s[1:]
				print("hash4=",s, h ,s4)
			elif h == "679431ca4a677b9405974a4ed20ff43c636b87e3260d93503daf8d4090f310e5":
				s5 = s[1:]
				print("hash5=",s, h ,s5)
			elif h == "1ab36a6268e6fffd304e69dbc82b5cef9b89ae9e904d716e006494462ec982cc":
				s6 = s[1:1]
				print("hash6=",s, h ,s6)
				
concates = s1+s2+s3+s4+s5+s6
print("input data="+concates)		
ha = ComputeSha256Hash(s1+s2+s3+s4+s5+s6)
print("result=sctf{"+ha+"}")