# my_diary

## Description
secret diary that only you can post and see
(service address and port)

## Comment
Users can compile filter function, so they can easily change memory or call functions.
However, the filter function cannot contain any library function and assembly language.
there exist many solutions; one would be overwriting the bss section.

## Exploit

In the filter function, get pie base by return address, and find bss buffer which used in system function.

Trigger command injection by appending string!


