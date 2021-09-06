# buildingblocks

## Description
Get a flag.
(service address and port)

## Comment
Disassemble given code blocks and check their constraints to run correctly.

## How to run

In deploy folder
```
$ docker build -t buildingblocks .
$ docker run -d --name buildingblocks -v $PWD/log:/tmp/log -p 46115:46115 buildingblocks
```

OR

simply run
```
$ ./run.sh
```

## How to see log

In deploy folder

```
$ cat log/buildingblocks.log
```
