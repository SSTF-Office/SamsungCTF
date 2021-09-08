# SSS_binary : Simple Script Service

## Description
Find bug in SSS2 and fix it!

(location of binary file)
[Hint](https://github.com/kaishackgon/sctf2017/tree/master/defense/SSS)

 - The purpose of patch is prevent segmentation fault of certain input. Exploitability is not important.
 - It is not valid patch that halts program for ceratin input. REPL must be preserved after invalid input.

## Comment

participants can get binary file.

submission server should replace file and execute it.



## How to solve

For each instruction in AST_tree_eval, interpreter checks type for arguemnts.

There are 6 types (INT = 0, STRING = 1, SYMBOL = 2, LIST = 3, BOOL = 4, FUNC = 5). By checking type for all instructions, challengers can find that AST_tree_app filters `INT, STRING,  BOOL` for first argument, but `LIST` is still acceptable. a minimal program which crashes this interpreter is `((con (int 1) (int 2)) (int 3))`. to patch this, change the part for type checking to reject `LIST`.

## author's solution
```/bin/sh
$ objdump -d script > original
5 objdump -d script_patched > patched
$ diff original patched
3687c3687
<   403f3d:     0f 84 85 00 00 00       je     403fc8 <AST_tree_eval+0x728>
---
>   403f3d:     0f 86 85 00 00 00       jbe    403fc8 <AST_tree_eval+0x728>
4544c4544
<   404c78:     74 6e                   je     404ce8 <AST_tree_app+0x88>
---
>   404c78:     76 6e                   jbe    404ce8 <AST_tree_app+0x88>
```
