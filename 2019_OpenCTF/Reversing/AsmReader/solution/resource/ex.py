'''
import angr

find = 0x400754
avoid = [0x400765]

p = angr.Project("./AsmReader.out", load_options={'auto_load_libs': False})
ex = p.surveyors.Explorer(find = (find,), avoid = avoid)
ex.run()

print ex.found[0].state.posix.dumps(0)
'''

import gmpy2

arr = "Reading bytecode is not easy"
mask = [16, 19, 89, 116, 98, 92, 109, 14, 7, 50, 53, 116, 24, 63, 108, 98, 121, 72, 12, 68, 119, 77, 9, 13, 67, 4, 81, 105]

#flag = "c4n_u_r34d_S7AGED_byt3c0d3??"

a = 31337
b = 79
m = 127

ainv = int(gmpy2.powmod(a, -1, m))
s = []
f = []
for i in range(len(arr)):
	s.append((ord(arr[i]) - b) * ainv % m)
	f.append(chr(s[-1] ^ mask[i]))

print "SCTF{" + "".join(f) + "}"

print "\nRight answers are:"
for i in range(0, len(arr), 4):
	t = map(chr, s[i:i + 4][::-1])
	print int("".join(t).encode("hex"), 16)

'''
725034867
1025442583
661280307
657161775
1030625085
1030389251
1450063655
'''
