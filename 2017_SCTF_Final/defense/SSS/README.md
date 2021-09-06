# SSS : Simple Script Service

## Description
Find bug in SSS and fix it!

(location of source and binary file)

## Comment

participants can get source and binary file.
and they can change only 2 one "*.c" file and one "*.h" file.

submission server should replace these two files and execute it (after compile).


## Patch
there is NULL overflow in `new_VAL_env`, because maximum SYMBOL length is unbounded, but
variable environment structure has `char val_name[32]`.

So, we have to patch source code this to a valid one.
One also should change new_VAL_env and del_VAL_env, too.

