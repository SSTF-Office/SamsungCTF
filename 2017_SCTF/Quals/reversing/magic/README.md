# magic

## Description

Find the valid serial.
(service address and port)
(binary download link)

## Comment

Simple serial cracking challenge.
Figuring out remainder calculation (or division) is the point.
Chinese remainder theorem is required to solve last equation.

z3 cannot solve this problem.

## How to run

In deploy folder
```
$ docker build -t magic .
$ docker run -d -v $PWD/log:/tmp/log --name magic -p 23972:23972 magic
```

OR

simply run
```
$ ./run.sh
```
