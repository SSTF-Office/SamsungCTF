# Seven's Game - Low
Welcome to Fabulous Hacker's Playground!  
Earn 5,000,000 SCORE within 600 spins to get the flag.  

## Category
Misc, Reversing

## Deploy
The actual challenge gives remote host only. (No attatchment)  
Neither binary nor source code is given in public.  
### Build for local test
```
cd prob
make
./seven1
```

### Docker
```
cp prob/seven1 deploy/server/src/challenge
cd deploy
docker-compose up -d --build
```

## Flag
`SCTF{NegA7iVE_Be7_ReA77Y_HAppEnED}`

## Solution
Use both internal game data extraction and integer overflow.
### Phase 1. Extract the bonus game information
- Realize that the result of bonus game is related with its color combination.
- Try the bonus game several times to get all of combinations of bonus game.

### Phase 2. SCORE : 10,000 to 50,000
- Select the difficulty to 5000.
- Repeat playing the game until SCORE becomes 50,000.
	- Easily win by choosing the best choice in the bonus game using the bonus game information got in Phase 1.

### Phase 3. SCORE : 50,000 to 5,000,000
- Select the difficulty to 50000.
	- The difficulty is -15,536 in actual.
	- `(short)50000 == -15536`
- Repeat playing the game until ACTUAL SCORE becomes 5,000,000.
	- The displayed SCORE is not actual score. It may be about -16 million.
	- Easily LOSE by choosing the worst choice in the bonus game using the bonus game information got in Phase 1.

## Statistics
Both are necessary to get 4~6% success probability. Otherwise it succeeds with <0.00003% chance.
### Phase 1
- Extract whole data within few spins.
### Phase 2
#### Without extraction
Choose randomly out of 4 choices in the bonus game.
- The probability to reach 50,000 is <0.1%.
#### With extraction
Always choose the maximum reward.
- The probability to reach 50,000 within 50 spins is ~10%.
### Phase 3
#### Without integer overflow
Always choose any difficulty other than 50000.
- The probability to reach 5M within 600 spins is 0% in any cases.
	- Any difficulty cases : 500, 5000
	- Any bonus game choice cases : Worst, Random, Best
#### With integer overflow
Choose the difficulty as 50000.
##### Without extraction 
Choose randomly out of 4 choices in the bonus game.
- The probability to reach 5M within 600 spins is ~0.03%.
- The probability within 550 spins is 0%.
##### With extraction
Always choose the minimum reward. No hope for other choice.
- The probability to reach 5M within 600 spins is \~61%.
- The probability to reach 5M within 550 spins is \~39%.

