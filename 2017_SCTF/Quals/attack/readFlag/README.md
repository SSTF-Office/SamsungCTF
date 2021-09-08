# readFlag

## Description
Read a flag.
[yo.tar](./prob/yo.tar)

## Comment
python pickle challenge.
Since file name is leaked, so you can open and read source code of test.py.

OR

There exist many other solutions.

## How to run

In deploy folder
```
$ docker build -t readflag .
$ docker run -d --name readflag -p 55402:55402 readflag
```

OR

simply run
```
$ ./run.sh
```

