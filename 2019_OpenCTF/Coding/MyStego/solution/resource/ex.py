from PIL import Image
import numpy as np

image = Image.open('challenge.png')

r, g, b = map(np.array, image.split())

binstr = "0b"
for a, b, c in zip(r, g, b):
	for i in range(0, image.width):
		binstr += str(int(a[i] ^ b[i] ^ c[i]) & 1)

with open("out", "wb") as f:
	f.write(("%x"%eval(binstr)).decode("hex"))
