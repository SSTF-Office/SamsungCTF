## PoWdle

### Overview

| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | Medium              |
| Category          | Misc                |
| Tags              | Misc, Attack        |
| Author            | hahah               |

### Description
 > Do you know [Wordle](https://www.nytimes.com/games/wordle/index.html) game?
 >
 > Try Proof-of-Workdle game!

### Deployment
 Provide Dockerfile & source code

### Solving strategy
 1. PoWdle game solver (by modifying wordle solvers, getting the required score is not difficult)
 Generate 3~4M hashes for the given challenge, and use them as wordlist.
 2. ReDoS attack on Email address
 The regular expression for email check, `^(\S*)@\S*\.\S*$`, is vulnerable to `"@."*2000`.
 It may takes longer than 5 seconds, triggering timeout.
 3. Command injection to read flag!

