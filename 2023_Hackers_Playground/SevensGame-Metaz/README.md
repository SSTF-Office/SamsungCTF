# Seven's Game - Metaz
Welcome to Fabulous Hacker's Playground!  
Earn 1,000,000,000 SCORE to get the flag.  

I believe we can control the randomness.

## Category
Reversing

## Deploy
Attachment in public : {seven3} binary
(The same file with `Seven's Game - Metal`)

### Build for local test
```
cd prob
make
./seven3 2000 100 0
```

### Docker
```
cp prob/seven3 deploy/server/src/challenge
cd deploy
docker-compose build --no-cache
docker-compose up -d --build
```

## Flag
`SCTF{stuck_1n_a_l0o0o0p}`

