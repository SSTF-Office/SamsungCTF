# Notes about CTF Challenge deployment

CTF challenge task is in `challenge.md`. 
Hints about:
- Fixed Window Exponentiation
- Chinese Reminder Theorem
- exact place where memory access pattern leakes during cache attack
- what exactly leaks: digits of d_p and d_q
- how the flag was encoded to the number (and decoded back)

can be found in `hint.py`. These hints can be disclosed during CTF if the task occurs to be too difficult.

## Quirks

[Here](https://blog.cryptohack.org/cryptoctf2021-hard#dorsa) we see some other method of encoding text to number:

```python
FLAG = int(FLAG.encode("utf-8").hex(), 16)
```
So in our CTF challenge it might be reasonable to synchronize flag encoding method with some CTF-standard way.

