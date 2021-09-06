# Knotted

> This whole reversing stuff is getting out of hand man. Now you can just run the file, take some tome for coffee and have it done.

> The flag is SAMSUNGCTF{output of python3 knotted.py 1586785745194037539296989450555036762962298359539455233716031661976595026090016806109443588280900030903881431427658355881525539279532875345980737296832990651884955460392766668800}

> Challenge files:
> knotted.py
> x.cpython-36m-x86_64-linux-gnu.so


We are given two files: `knotted.py` and `x.cpython-36m-x86_64-linux-gnu.so`. To get the flag, we need to know the output of 
```
python3 knotted.py 1586785745194037539296989450555036762962298359539455233716031661976595026090016806109443588280900030903881431427658355881525539279532875345980737296832990651884955460392766668800
``` 

Let's try to run it...
```
Your age: 46786745692
You are not underage and so, you need to verify your identity with your private keys
These are your private keys:
id_rsa	id_rsa.pub  known_hosts
-----BEGIN RSA PRIVATE KEY-----
[...]
Do you want to continue uploading them? [y/n]n
```
That is scary. Should we continue? Well, I guess no. Clicking `n` closes the program:
```
Ok, shutting down
```

If we rerun it, but we click `y`, the program simulates sending your private data, but as we can check, it doesn't actually read anything and it's only a "chochoł". But nonetheless, the computation can't seem to end. What happens with smaller arguments?

```
$ python3 knotted.py 4
1057
```
So it seems to be something really lengthy. Let's try to reverse it.

File `knotted.py` contains just two lines, first one with neccesary imports, second decompresses inlined marshaled object.
After the file is decompressed, we can proceed to analyzing the code object. We use the `uncompyle6` decompiler (specifically the `uncompyle6.main.decompile` function). This outputs following code:
```python3
# uncompyle6 version 3.3.3
# Python bytecode 3.6
# Decompiled from: Python 3.6.7 (default, Oct 22 2018, 11:32:17) 
# [GCC 8.2.0]
s = 0
K = True
b = int
G = getattr
import x as xx
try:
    xx.d(42)
except Exception as e:
    if isinstance(e, SystemExit):
        exit()

def x(c1):
    return '78b9w67rtygiuf%ndyiubvjxbtkuijmcv8cgirjesc5w4'[14::20] % c1


def m(x1):
    return x1 + d()


def B(x1):
    return x1[:-1]


def v(x1, y1):
    if y1 != h():
        return v(m(x1), B(y1))
    else:
        return x1


def h():
    return ''


def i(a1, b1):
    if b1 == h():
        return a1
    else:
        if a1 == h():
            x = xx.d(h)()
        return i(B(a1), B(b1))


def j(coefs, x1):
    if coefs:
        return coefs[0] + x1 * j(coefs[1:], x1)
    else:
        return 0


def d():
    global s
    s = (s ** 2 + 1) % 3943
    return x(s + j([2] * 4, 3))


def Y(x1):
    if not x1:
        return 0
    else:
        return 1 + Y(B(x1))


def M(a1, b1):
    if a1 == h():
        if b1 == h():
            pass
        return d()
    elif a1 == h() or b1 == h():
        return h()
    else:
        return M(B(a1), B(b1))


def z(a1, b1):
    if b1 == h():
        return h()
    elif a1 == h():
        return d()
    else:
        return z(B(a1), B(b1))


def n(a1, b1):
    if b1 == h():
        return h()
    else:
        return v(a1, n(a1, B(b1)))


def l(a1, b1, x1):
    return M(a1, n(b1, x1))


def L(a1, b1):
    if b1 == h():
        x = xx.d(h)()
        return x
    elif z(a1, b1):
        return h()
    n1 = h()
    while z(n(n1, b1), a1) or M(n1, a1):
        if not l(a1, b1, n1):
            n1 = m(n1)

    if l(a1, b1, n1):
        return n1
    else:
        return B(n1)


def q(a1, b1):
    x1 = n(L(a1, b1), b1)
    return i(a1, x1)


def R(a1, b1):
    return q(a1, b1) == h()


def X(x1):
    if not x1:
        return h()
    else:
        return v(d(), X(x1 - 1))


def D(n1):
    d1 = d()
    s1 = h()
    f = v(d1, d1)
    f = v(n(f, f), d1)
    while 1:
        if z(d1, n1) or M(d1, n1):
            if R(n1, d1):
                s1 = v(s1, n(d1, n(d1, n(d1, n(d1, d1)))))
            d1 = m(d1)

    return s1


def U(x1):
    x1 = X(x1)
    o1 = D(x1)
    print(Y(o1))


def H():
    import sys
    sys.setrecursionlimit(1000000000)
    try:
        U(b(G(sys, 'vgra'[::-1])[1]))
    except Exception as e:
        print("Something's not right")
H()
```

Let's start analyzing functions one by one. 

Function `x` looks like this. 
```
def x(c1):
    return '78b9w67rtygiuf%ndyiubvjxbtkuijmcv8cgirjesc5w4'[14::20] % c1
```
We see that the first part is not dependent on input, so we can inline it, and this function is just `'%c' % c1`, so `x` is the `chr` function.


There are two functions, `j` and `d`:
```python
def j(coefs, x1):
    if coefs:
        return coefs[0] + x1 * j(coefs[1:], x1)
    else:
        return 0


def d():
    global s
    s = (s ** 2 + 1) % 3943
    return x(s + j([2] * 4, 3))
```
We don't need to analyze what `j` does, because it's called with a constant argument, and its' result is `80`. `d` is just a pseudorandom generator, which spits out some characters with codes from range `80` to `80 + 3943`. Let's call it `randchar`.

```python
def m(x1):
    return x1 + d()


def B(x1):
    return x1[:-1]
```

`m` appends random character to the end of input, while `B` is its reverse - it removes last character form the string.

`h` is just an empty string.
```python
def h():
    return ''
```

This gives possibility to understand the function `v`:
```python
def v(x1, y1):
    if y1 != h():
        return v(m(x1), B(y1))
    else:
        return x1
```
After cleanup and unrolling recursion, this translates to
```python
def v(x1, y1):
    while y1 != '':
        x1 += randchar()
        y1 = y1[:-1]
    return x1
```
So basically, this produces a string that has length equal to sums of lengths of `x1` and `y1`. By now, we can suspect that the strings themselves don't play big role - what is important is their length. Indeed, `m` should be named `inc`, `B` should be called `dec`, whereas the proper name for `v` is `add`. `d` should be renamed to `one` and `h` to `zero`. Per analogy, we can suspect that `i` is just subtraction (by running it). There is this call to function `xx.d`, but let's ignore it for a while - it raises `IndexError` nonetheless, when strings can't be subtracted (`a1` is shorter than `b1`).
```
def i(a1, b1):
    if b1 == h():
        return a1
    else:
        if a1 == h():
            x = xx.d(h)()
        return i(B(a1), B(b1))
```

Function `M` tells whether both strings have the same length. `z` implements the "less than" operator (again, comparing lengths). `n` uses the rule `a*b = a*(b-1) + a` to compute the product. `L` is a really opaque integer division which is carried by finding the greatest such number `x`, so that `a <= b*x` (this checks every number less than correct `x`). `q` is the modulo operator and `R` tells whether `a1` divides `b1`. `X`, as can be seen, takes an integer and creates a string of length equal to this integer. Now comes the main function, `H` which sets astronimical limit on call stack size (hey, there are almost no loops in the code!), then decodes argument from the command line and calls `U`.
`U` is the function that produces the result we seek. Let's try to rewrite it to more readable form:
```
def D(n1):
 d1=d()
 s1=h()
 f=v(d1,d1)
 f=v(n(f,f),d1)
 while z(d1,n1)or M(d1,n1):
  if R(n1,d1):
   s1=v(s1,n(d1,n(d1,n(d1,n(d1,d1)))))
  d1=m(d1)
 return s1
```
is turned into
```
def D(n1):
    d1 = 1
    s1 = 0
    f = 1 + 1
    f = f*f + 1
    # now f equals 5
    while d1 < n1 or d1 == n1:
        if n1 % d1 == 0:
            s1 += d1*d1*d1*d1*d1
        d1 = d1 + 1
    return s1
```

This code computes the sum of fifth powers of number `n1`. We can verify that it's indeed the case. This code above produces the same results as `knotted.py` and does it blazingly fast, but unfortunately not fast enough for `n1 = 46786745692`. We can try to optimize it. Divisors of numbers come in pairs - if `x**2 != n`, but `x` divides `n`, then `n/x` is some other factor of `n`, therefore we can stop looking at factors after passing the square root of `n`. This code realizes this idea:
```
def D(n):
    s = 0
    i = 1
    while i**2 < n:
        if n % i == 0:
            s += i**5
            s += (n//i)**5
        i += 1
    if i**2 == n:
        s += i**5
    return s
```
It's reasonably fast for numbers less than `2**44` but later it slows down quite a lot. To get the answer, we need to dig deeper. It turns out that the function we're dealing with is multiplicative - for coprime integers `x`, `y`, it's true that `D(x*y) == D(x)*D(y)`. Let's plug the number to the factorization engine of choice, to get (these are pairs `prime: multiplicity`).
```
factors = {2: 49,
 3: 9,
 5: 2,
 7: 14,
 11: 4,
 13: 3,
 17: 2,
 19: 2,
 23: 3,
 29: 2,
 31: 1,
 41: 1,
 47: 1,
 53: 1,
 59: 2,
 61: 3,
 71: 3,
 83: 1,
 101: 1,
 109: 1,
 137: 1,
 139: 1,
 163: 2,
 173: 1,
 193: 1,
 199: 1,
 211: 1,
 227: 1,
 263: 1,
 269: 1,
 277: 1,
 307: 1,
 337: 1,
 349: 2,
 601: 1,
 641: 1,
 659: 2,
 733: 1,
 829: 1,
 1399: 1,
 1487: 1,
 1601: 1,
 1627: 1,
 1721: 1,
 1811: 1,
 1979: 1,
 2003: 1,
 2039: 1,
 2423: 1,
 5477: 1,
 5741: 1,
 7151: 1}
```
Then we notice that the factor contributing by any pair can be computing just by counting contributions given by subsequent powers of prime. Thus our counting function becomes
```
def f(prime, multiplicity):
    return sum(prime**(i*5) for i in range(multiplicity + 1))
import functools
print(functools.reduce(lambda x, y: x*y, [f(p, m) for (p, m) in factors]))
```
That gives out the result:
10431336152378891867561689393034395730190783857787201533568824973683968856835675395587772944624072318506287147486547094288692008428972593893646006808129520287832965522156460442459535098338850492720363683000111123089465329880058433400791661173955368869118813366570083506546672635356028198628239502411968907047652261931580390487624543107108678015343585676220464717198303170984127945434560261177313355651922722991782487393331877272343127637218982258864756433559815142459877203414667684196984587426502996777645730749308044308365117748329389727360377738678369720574888399445104445560167047204536093618271876392804149362368496487587467382294621122858686624913309783646989675760163837989803596025893513754965657320278120718431182747397832838156769171993802884517556042949967716235795637179142709972646187613420070603798848949038900115427468703886177211461156220330364108800000000000000000000000
