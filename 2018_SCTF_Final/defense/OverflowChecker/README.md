# OverflowChecker

## Description
Syntax of Language

```
B := S B
   | [empty]

S := V <- IE;
   | for( v1 <- 0; v1 < v2 ; v1 <- (v1 + 1) ) { B }
   | IE;
   | if(BE) {B}
   | if(BE) {B} else {B}

IE := Int
   | (IE + IE)
   | input()
   | V

BE := V = Int
   | V < Int
   | V <= Int
   | V > Int
   | V >= Int

V := [a-zA-Z_][a-zA-Z0-9_]*

```

All values should have range
`[0, 2147483647]`. If overflow occurs during interpretation, then it generates error and halt s.

Examples
```
x <- input();
x <- ( x + 1 );
```
overflow (maximum x = 2147483648)

```
x <- input();
if( x > 2137546063 ) {
  x <- 2137507655;
}
if( x <= 2137599644 ) {
  x <- ( x + 30868 );
}
```
safe (maximum x = 2137576931)

```
x <- input();
if( x > 1726640836 ) {
  x <- 1726606656;
}
if( x <= 1726696811 ) {
  x <- ( x + 13703 );
}
x2 <- 452245623;
x3 <- ( x2 + x );
```
overflow (maximum x3 = 2178900162)

```
x <- input();
if( x > 100 ) {
  x <- 17;
}
if( x <= 1148 ) {
  x <- ( x + 134 );
}
x2 <- 0;
for( x3 <- 0; x3 < x; x3 <- ( x3 + 1 )) {
  x4 <- input();
  if( x4 > 14294191 ) {
    x4 <- 14263305;
  }
  if( x4 <= 14354459 ) {
    x4 <- ( x4 + 2825 );
  }
  x2 <- ( x2 + x4 );
}
```
overflow (maximum x2 = 3345501744)

In the challenge, 100 programs are given. For each program, challengers should enter "n" if a program has expression that makes overflow. Else, enter "y". The flag will be given when at least 95 answers are correct.

(ip : port, description link)

## Challenge setup
```/bin/sh
$ cd deploy/
$ docker build -t ofd .
$ docker run -p $PORT:8888 -it ofd [$PORT : host port]
```
## Exploit

Challengers are supposed to make an analyzer for this program. First, making own parser
or using existing parser with small changing of syntax (for example, replace all "<-" to "=", and "=" to "==").

Second, analyze parsed program with several method. It has very restricted patterns in challenge, so lots of
method are suitable to analyze. Author's intended solution is building abstract interpreter with interval domain,
and apply special rule for `for` loop that runs body of the loop only once and apply the result of analysis multiple time.

In result, author's solution takes ~30s to solve with 100% accuracy. But many other method is available such as
dynamic testing with heuristic algorithm to input.
