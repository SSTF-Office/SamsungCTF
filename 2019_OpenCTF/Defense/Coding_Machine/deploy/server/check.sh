#!/bin/sh
NUM_ARGS=2
TEST_INPUT=$1
USER_OUTPUT=$2

if ! [ $# = $NUM_ARGS ]; then
  echo "[Useage] : ./check.sh ./test_input ./user_output"
  exit 1
fi

#OUT=comm -2 -3 $TEST_INPUT tmp0.txt | head -1
OUT="$(diff -Nurw $TEST_INPUT $USER_OUTPUT | head -1)"

if [ -z "$OUT" ]; then
  echo "true"
else
  echo "false"
fi
