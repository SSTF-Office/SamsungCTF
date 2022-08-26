l = 1024
p1 = random_prime(2**l)
p2 = random_prime(2**l)
n = p1*p2
print(p1, p2, n)

a1 = inverse_mod(p2, p1)*p2
a2 = inverse_mod(p1, p2)*p1
print(a1, a2)

# ?? how to choose r: the size of r
r = 6
result = list()
for i in range(0, r):
    x1 = ZZ.random_element(0, p1)
    x2 = ZZ.random_element(0, p2)
    y = a1*x1 + a2*x2
    assert y%p1 == x1 and y%p2 == x2
    result.append((x1, x2, y))
print(result)

## a1*u1 + a2*u2 = v
#u1 = list(item[0] for item in result)
#u2 = list(item[1] for item in result)
v = list(item[2] for item in result)
#print(u1)
#print(u2)
print(v)


##?? how to choose k1: the size of k1
k1 = 2**32
d1 = r+1
base1 = list()

for i in range(0, r):
    vector = list()
    for j in range(0, d1):
        if j == 0:
            vector.append(k1*v[i])
        else:
            if j == i+1:
                vector.append(1)
            else:
                vector.append(0)
    base1.append(vector)
    
base1 = Matrix(ZZ, r, d1, base1)
base1.transpose()
print("-"*100)
print(base1)

reduced1 = base1.LLL()
print("-"*100)
print(reduced1)


#?? how to choose k2: the size of k2
#!! roughly l
k2 = 2**l
d2 = r-2+r
base2 = list()
for i in range(0, r):
    vector = list()
    for j in range(0, d2):
        if j < r-2:
            vector.append(k2*reduced1[j][i+1])
        else:
            if j == i+r-2:
                vector.append(1)
            else:
                vector.append(0)
    base2.append(vector)
    
base2 = Matrix(ZZ, r, d2, base2)
base2.transpose()
print("-"*100)
print(base2)

reduced2 = base2.LLL()
print("-"*100)
print(reduced2)

w1 = list()
w2 = list()
for i in range(0, r):
    w1.append(reduced2[0][r-2+i])
    w2.append(reduced2[1][r-2+i])
print(w1, w2)
sol = [
    gcd(v[0]+w1[0], n), 
    gcd(v[0]+w2[0], n), 
    gcd(v[0]-w1[0], n), 
    gcd(v[0]-w2[0], n),
    gcd(v[0]+(w1[0]+w2[0]), n), 
    gcd(v[0]+(w1[0]-w2[0]), n), 
    gcd(v[0]+(-w1[0]+w2[0]), n), 
    gcd(v[0]+(-w1[0]-w2[0]), n), 
]
for s in sol:
    if s != 1 and s != n and n%s == 0:
        print("Found a factor of n:", s)

print(p1)
print(p2)
