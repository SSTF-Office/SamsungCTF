# LCG

## Description

Here is a new PRNG which is a variant of LCG. Beat this and get the flag!

## How to run
`sh run.sh`

## How to solve
1. We have recurrence relation `A_n - X*A_{n-1} = Y(A_{n-1} - X*A_{n-2})`
1. Similar to [link](https://security.stackexchange.com/questions/4268/cracking-a-linear-congruential-generator), find remainder.
2. Find other secrets from remainder.

## Flag
`SCTF{LCG_is_too_simple_to_be_a_good_PRNG}`
