from chal import disp_vector
from arduino_test import char_vector
import itertools

leds = dict()
for k, v in char_vector.iteritems():
	val = []
	for i in range(17):
		if v[i] == 0:
			val.append(i)
	leds[tuple(val)] = k

#print leds

status = [0] * 17
flag_prefix = "SCTF{"
mapping = [0] * 17

vector = dict()
for c, v in zip(flag_prefix, disp_vector[:5]):
	for idx in v:
		status[idx] = 1 - status[idx]
	vector[c] = set()
	for i in range(17):
		if status[i] == 1:
			vector[c].add(i)

#print vector
forK = set(range(17))
flag_vector = []
for v in disp_vector[5:]:
	for idx in v:
		status[idx] = 1 - status[idx]
	s = set()
	for i in range(17):
		if status[i] == 1:
			s.add(i)
	flag_vector.append(s)
	forK -= s

#print flag_vector

forB = vector['C'] - vector['S']
forE = vector['F'] - vector['C']
forIQ = vector['T'] & vector['F']
forF = ((vector['C'] - vector['T']) & vector['F']) - forB
forCG = vector['{'] - vector['C'] - vector['F']
forDH = vector['T'] - vector['C']
forLN = (vector['S'] - vector['C']) - vector['F']
forAJ = vector['S'] - vector['F'] - forLN
forMOP = set(range(17)) - forB - forE - forIQ - forF - forCG - forDH - forLN - forK - forAJ

b, e, f, k = [e.pop() for e in [forB, forE, forF, forK]]

candidates = [forIQ, forCG, forDH, forLN, forAJ]
for i, c, d, l, a in itertools.product(*candidates):
	q = (forIQ - {i}).pop()
	g = (forCG - {c}).pop()
	h = (forDH - {d}).pop()
	n = (forLN - {l}).pop()
	j = (forAJ - {a}).pop()

	for m, o, p in itertools.permutations(forMOP):
		mapping = {eval(chr(ord('a') + idx)):idx for idx in list(range(17))}

		res = "SCTF{"
		for v in flag_vector:
			check = [mapping[el] for el in v]
			check.sort()

			if tuple(check) not in leds:
				break

			res += leds[tuple(check)]
		else:
			print "flag found! " + res
