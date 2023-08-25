# `Seven's Game - Metaz` Solution

First, identify the argument of this challenge.

```
MAXIMUM_SPIN, MAXIMUM_SEED_CHANGE, USE_JACKPOT = 2000, 100, 0
```

It is equal to `./seven3 2000 100 0` in the command line.

Player has to find the loop giving the higher win.

## Analyzing the binary & Collecting training set & Training the model
Refer the `Seven's Game - Metal` solution.

Also, there is a solution without using machine learning.

## Analyzing the constraint
The price of flag is 100,000 times of the highest betting.

It means that the winning of each spin should be larger than 51 times (of the betting) within 2000 spins.

Because the `MAXIMUM_SEED_CHANGE` is 100, the player has to find the loop with the high win of above 51x on average.

If the loop gives (100, 0) wins, it fails to buy the flag. At least the loop of (100, 100, 0) wins is necessary.

Also the loop of (100, ) win could be the answer.

So now the problem is redefined as finding the fixed-point of the model, `x` such as `x = f(x)`, which gives 100x win.

## Solution I - with ML
Find the fixed-point input in the whole input space of `2**20` size and then check if it gives 100x win.

We know that the least significant bit of each reel's position should be 0, so we can reduce the search space into `2**17` size.

The expected number of spin is 216 because it is independent between the result of spin and the fixed-point property.

However, the model does not have 100% accuracy so there is a chacne that `SEED` the model found is not the actual fixed-point, and some more spin may be necessary.

At least 1011 spins are necessary after the correct answer is found, so about 900 spins may be used to collect the training data.

## Solution II - without ML
The target `SEED` is a fixed-point and it gives the 100x win (BBB).

Each connection (to the challenge server) has only thousands of candidates of `SEED`s not million.

Because if the connection has a correct answer of `SEED`, it should give 100x win, and the number of combination with BBB is at most thousands in each connection.

So in each connection, player just has to try checking 100 candidates of fixed-point, which are supposed to give BBB combination.

Finding the answer, just change the betting as the highest and spin 1011 times.

It is necessary to scan the whole reel strips to use this strategy. Turn on the spin animation and set frame interval into zero, then perhaps the whole reel strips could be scanned.

If the search space is `2**18`, the odd of fixed-point is about 0.8 million. So the player has to connect to the challenge 8000+ times. Though it needs a number of trials (of connection), it does not need to train the model so it could be more efficient if you do not have a GPU.

And we also have the chance to find `SEED` such as
```
SEED --> FIXED-POINT --> FIXED-POINT --> FIXED-POINT --> ...
```

`SEED` is not a fixed-point itself but the output what `SEED` makes is a fixed-point so `SEED` is also the correct answer. Therefore, the acutal chance to find the answer may be higher than considered.
