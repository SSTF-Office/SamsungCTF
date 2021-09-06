# dfa : dfa service

## Description

simple finite automata..

has 1 vulnerability
(but found there exists multiple unintended vulnerabilities...)

(location of binary file and source file)

## Comment

participants can get source and bianry file.


## Patch (Original intention)

for add_node, it checks length of node name, but it checks fail when length is -1.

so participants have to change length check routine.
