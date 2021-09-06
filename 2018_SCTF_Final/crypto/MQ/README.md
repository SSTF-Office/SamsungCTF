# MQ

## Description
I heard solving multivariate quadratic polynomial is really hard.
Is it true??
[link](#)

## How to run
xinetd. `python3 mq.py`

## flag
`SCTF{Try_MQ_be_happy!}`

## How to solve
In general, searching solution of MQ system is known as NP-hard problem.
However, in MQ cryptosystem always have linearlity and this is why
we need to pad message with hash (e.g. c = p(m || sha256(m))).

1. Make matrix equation with form `A * x + B = C` from `f(x+d) - f(x)`
(note that plus is componentwise).

2. Find `A` from given `x` and `C`.

3. Find `d` from `A`, then that will be the flag.
