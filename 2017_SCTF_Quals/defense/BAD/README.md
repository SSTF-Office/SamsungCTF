# Buffer overflow Automatic Defense

## Description
Please make binaries safe automatically.

nc ip port

## Comment
There are 3 stages.
Each stage, it has 30 problems which has randomly generated buffer size and input size.

## Exploit
### Stage 1.

  There is stack based BOF in `get_int` function.
  Check buffer size and input size. And make input size smaller than buffer size.

### Stage 2.

  There is stack based BOF in `get_file` function.
  Check buffer size and input size. And make input size same as buffer size.
  Because the buffer size means the size of file name.

### Stage 3.

  There is heap overflow in `modify file` function.
  Check buffer size and input size. And make input size same as buffer size.
  Because the buffer size means the size of file name.

### Etc.
  Stage 2 includes the vulnerabilty of Stage 1, and Stage 3 also include the vulnerabilities of Stage 2.

