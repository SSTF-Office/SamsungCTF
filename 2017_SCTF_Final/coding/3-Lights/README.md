# 3-Lights

## Description
Do you know lights-out game? [link](http://www.logicgamesonline.com/lightsout/)

It is quite fun, but too easy :P.

So, Here is a deep-dark version of lights-out.

There are 3 colors of light(R -> G -> B -> R) instead of 2(on / off).

And I also changed clicking operation slightly. lol

Enjoy it.

[source code will be given here]

(service address and port)

## Comment
Swiping down is a basic operation for solving these puzzle.

For each clicking on first row, we can make results of swiping down operation, and make them as a matrix.

Matrix is invertable on given environment, so every puzzle has a unique solution.

(result of swiping down for given puzzle) \times (inverse matrix made from just above) = number to click for first row

Now, click (inverse number) times on first row and swipe down. Then you will get a qrcode.

Decode and Send.

Given source code will helpful to understand what they do.

## How to run

In deploy folder
```
$ docker build -t lights .
$ docker run -d --name lights -v $PWD/log:/tmp/log -p 22341:22341 lights
```

OR

simply run
```
$ ./run.sh
```
