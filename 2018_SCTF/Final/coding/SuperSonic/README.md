# SuperSonic

## Description

```
Solve the challenges!
Tip: Using given aws instances would be better because of network latency.
(service adderss)
(binary download link)
```

## How to run

in deploy folder,
```
# ./run.sh
```

## How to solve

This is a reversing & code optimization challenge.

When you analyze `dumb_solver`, you can figure out the given algorithm.

The algorithm performs factorizing [Lyndon word](https://en.wikipedia.org/wiki/Lyndon_word).

If given two consecutive chunks are lyndon words and first one is smaller than
second one, union of those chunks is also a lyndon word.

Through this fact, we can factorize given string from bottom to top.

To speed up, I used C/C++.

## Before launch

(!!!IMPORTANT) You need to set new time limit because of network latency.
