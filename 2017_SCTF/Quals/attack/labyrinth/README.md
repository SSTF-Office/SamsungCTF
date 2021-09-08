# Labyrinth

## Description
  > Labyrinth management program!

  > `nc ip port`

## Instruction

  > binary will given

## exploit

* stage 1: memory leak;

* stage 2: get heap overflow

    Logical error while processing fwrite & fscanf.

    Using this bug, you can get heap overflow.

* stage 3: House of Force

    Use heap overflow in stage2, you can overwrite wilderness.

    Using House of Force, get shell.
