#!/bin/sh
git init -q

git config user.name "admin"
git config user.email "admin2020@sctf.com"

EMPTY_TREE=$(git write-tree)

Tree32=$(git commit-tree --no-gpg-sign -m "[loop0] " $EMPTY_TREE)
Tree26=$(git commit-tree --no-gpg-sign -m "0 \"Fail..\n\" [loop6] " $EMPTY_TREE)
Tree23=$(git commit-tree --no-gpg-sign -m "write 1 left [loop6] " $EMPTY_TREE)
Tree21=$(git commit-tree --no-gpg-sign -m "put [loop7] " $EMPTY_TREE)
Tree20=$(git commit-tree --no-gpg-sign -m "quit " $EMPTY_TREE)
loop7=$(git commit-tree --no-gpg-sign -m "1 right read dup " -p $Tree20 -p $Tree21 $EMPTY_TREE)
git tag loop7 $loop7
loop6=$(git commit-tree --no-gpg-sign -m "dup " -p $loop7 -p $Tree23 $EMPTY_TREE)
git tag loop6 $loop6
Tree25=$(git commit-tree --no-gpg-sign -m "0 \"Congratz!\n\" " -p $loop6 $EMPTY_TREE)
Tree27=$(git commit-tree --no-gpg-sign -m "40 right cmp " -p $Tree25 -p $Tree26 $EMPTY_TREE)
Tree18=$(git commit-tree --no-gpg-sign -m "1 2 sub add 40 right [loop5] " $EMPTY_TREE)
Tree16=$(git commit-tree --no-gpg-sign -m "1 2 sub add 1 left [loop5] " $EMPTY_TREE)
Tree15=$(git commit-tree --no-gpg-sign -m "0 add 1 left [loop5] " $EMPTY_TREE)
Tree17=$(git commit-tree --no-gpg-sign -m "read 41 right read cmp " -p $Tree15 -p $Tree16 $EMPTY_TREE)
Tree19=$(git commit-tree --no-gpg-sign -m "read 41 left read cmp " -p $Tree17 -p $Tree18 $EMPTY_TREE)
loop5=$(git commit-tree --no-gpg-sign -m "1 read cmp " -p $Tree19 -p $Tree27 $EMPTY_TREE)
git tag loop5 $loop5
Tree29=$(git commit-tree --no-gpg-sign -m "1 right 11 write 1 right 37 write 1 right 99 write 1 right 39 write 1 right 62 write 1 right 126 write 1 right 64 write 1 right 114 write 1 right 103 write 1 right 98 write 1 right 3 write 1 right 75 write 1 right 16 write 1 right 18 write 1 right 96 write 1 right 74 write 1 right 124 write 1 right 85 write 1 right 3 write 1 right 14 write 1 right 45 write 1 right 42 write 1 right 29 write 1 right 105 write 1 right 65 write 1 right 83 write 1 right 5 write 1 right 121 write 1 right 100 write 1 right 21 write 1 right 47 write 1 right 124 write 1 right 101 write 1 right 73 write 1 right 21 write 1 right 13 write 1 right 25 write 1 right 9 write 1 right 17 write 1 right 62 write 0 " -p $loop5 $EMPTY_TREE)
Tree12=$(git commit-tree --no-gpg-sign -m "4 left read 42 left write 40 right [loop1] " $EMPTY_TREE)
goto2=$(git commit-tree --no-gpg-sign -m "1 left read 3 left write 5 right read 4 left write 2 left read dup add write 3 right read 1 sub write [loop2] " $EMPTY_TREE)
git tag goto2 $goto2
goto1=$(git commit-tree --no-gpg-sign -m "5 left read 1 left read add write 6 right " -p $goto2 $EMPTY_TREE)
git tag goto1 $goto1
Tree3=$(git commit-tree --no-gpg-sign -m "2 left [goto1] " $EMPTY_TREE)
Tree2=$(git commit-tree --no-gpg-sign -m "2 left [goto2] " $EMPTY_TREE)
Tree4=$(git commit-tree --no-gpg-sign -m "read 2 right read cmp " -p $Tree2 -p $Tree3 $EMPTY_TREE)
Tree7=$(git commit-tree --no-gpg-sign -m "read 2 left read cmp " -p $Tree4 -p $goto1 $EMPTY_TREE)
Tree1=$(git commit-tree --no-gpg-sign -m "1 left 1 read add write 1 right read 2 sub write [loop4] " $EMPTY_TREE)
loop4=$(git commit-tree --no-gpg-sign -m "2 read cmp " -p $Tree1 -p $Tree7 $EMPTY_TREE)
git tag loop4 $loop4
Tree9=$(git commit-tree --no-gpg-sign -m "1 right 0 write 4 left read 5 right write " -p $loop4 $EMPTY_TREE)
Tree0=$(git commit-tree --no-gpg-sign -m "1 left 1 read add write 1 right read 2 sub write [loop3] " $EMPTY_TREE)
loop3=$(git commit-tree --no-gpg-sign -m "2 read cmp " -p $Tree0 -p $Tree9 $EMPTY_TREE)
git tag loop3 $loop3
Tree11=$(git commit-tree --no-gpg-sign -m "1 right 0 write 3 left read 4 right write " -p $loop3 $EMPTY_TREE)
loop2=$(git commit-tree --no-gpg-sign -m "1 read cmp " -p $Tree11 -p $Tree12 $EMPTY_TREE)
git tag loop2 $loop2
Tree14=$(git commit-tree --no-gpg-sign -m "read 1 right 0 write 1 right 1 write 1 right write 1 right write 1 right 8 write " -p $loop2 $EMPTY_TREE)
loop1=$(git commit-tree --no-gpg-sign -m "1 read cmp " -p $Tree14 -p $Tree29 $EMPTY_TREE)
git tag loop1 $loop1
Tree31=$(git commit-tree --no-gpg-sign -m "pop 67 write 1 left 99 write 1 left 57 write 1 left 65 write 1 left 82 write 1 left 86 write 1 left 120 write 1 left 83 write 1 left 72 write 1 left 98 write 1 left 74 write 1 left 81 write 1 left 72 write 1 left 90 write 1 left 99 write 1 left 118 write 1 left 57 write 1 left 68 write 1 left 88 write 1 left 110 write 1 left 81 write 1 left 52 write 1 left 100 write 1 left 74 write 1 left 121 write 1 left 81 write 1 left 77 write 1 left 83 write 1 left 122 write 1 left 113 write 1 left 81 write 1 left 80 write 1 left 66 write 1 left 117 write 1 left 77 write 1 left 69 write 1 left 97 write 1 left 55 write 1 left 102 write 1 left 88 write 1 left 40 right " -p $loop1 $EMPTY_TREE)
loop0=$(git commit-tree --no-gpg-sign -m "get dup 10 cmp " -p $Tree31 -p $Tree32 $EMPTY_TREE)
git tag loop0 $loop0
git reset $loop0