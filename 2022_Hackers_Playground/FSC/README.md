## Format String CrackMe

### Overview

| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | Low/Medium          |
| Category          | Reversing, Misc     |
| Tags              | Reversing           |
| Author            | hahah               |

### Description
 > Format String CrackMe!

### Deployment
 Provide the source code

### Solving strategy
 Reversing the format string and find out what it does.

 It simply checks flag bytes one by one.

 For instance, in `A(M(12))R(48,13)`,
 * `M(12)` reads value of 12th argument (`f[27]`), and prints a string with a length is equal to the value. (using "%.\*12d" format string)
 * A function repeats (`M(12)`) twice, so the number of printed bytes become `f[27]*2`.
 * `R(48,13)` prints a dummy string with a length of 48. As the 27th character of the correct flag is 'h' (ascii 107), if the input is correct, the total printed number of bytes becomes 107\*2 + 48 = 256 (0x100).
 * Then, `R(48,13)` stores the number of bytes to 13th argument (`f+100`) using "%hhn", which stores a one-byte value. Thus, if the input is correct, it stores a NULL byte (one byte of 0x100), otherwise, it stores non-NULL character.

 In this way, this format string checks the correctness of the input.

### Flag
 SCTF{just\_a\_printf\_is\_enough!}

### Reference
 For more info, see [printf-tac-toe](https://github.com/carlini/printf-tac-toe)

