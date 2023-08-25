import json
a = []
for i in range(2 ** 20):
    if i & 0b000001000001000001:
        continue
    a.append(i)

fp = open("numbers.txt", "w")
json.dump(a, fp)
fp.close()
