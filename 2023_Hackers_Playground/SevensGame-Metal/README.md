# Seven's Game - Metal
Welcome to Fabulous Hacker's Playground!  
Earn 1,000,000,000 SCORE within 1000 spins to get the flag.  

I believe we can control the randomness.

## Category
Reversing

## Deploy
Attachment in public : {seven3} binary
### Build for local test
```
cd prob
make
./seven3 1000 1000 1
```

### Docker
```
cp prob/seven3 deploy/server/src/challenge
cd deploy
docker-compose build --no-cache
docker-compose up -d --build
```

## Flag
`SCTF{M4k3_y0ur_Own_1uck}`

