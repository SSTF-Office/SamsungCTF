import sys
import string
import random

fn_in = sys.argv[1]
fn_out = sys.argv[2]

data = open(fn_in).read().encode("base64")

base64char = string.letters + string.digits + "+/="

chars = [chr(i) for i in range(128) if chr(i) not in base64char]

idx = (random.sample(range(len(data)), len(data) // 2))
idx.sort()

for i in idx[::-1]:
	data = data[:i] + random.choice(chars) + data[i:]

for i in range(10):
	data += random.choice(chars)

with open(fn_out, "wb") as f:
	f.write(data)
