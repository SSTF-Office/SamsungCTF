# Seven's Game - High
Welcome to Fabulous Hacker's Playground!  
This is for the high roller!  
Earn 1,000,000,000 SCORE to get the flag.  

## Category
Reversing

## Deploy
The actual challenge gives remote host and the binary `seven2` in this directory.
### Build for local test
```
cd prob
make
./seven2
```

### Docker
```
cp prob/seven2 deploy/server/src/challenge
cd deploy
docker-compose up -d --build
```

## Flag
`SCTF{A7WayS_MinD_CoNs7RaiN7}`

## Solution
There may be many ways to achieve the target score so this is just one of them.  

### Phase 0. Recover PAR sheet
- Analyzing the binary, a player can get game information of reel strip and free game.
- So the player can calculate the expecatation return of game and which option is the best.

### Phase 1. SCORE : 100,000 to 1,000,000
- Becasue of re-trigger in the free game, `10 Free Plays with Multiplier 1` in the Type B free game options gives the highest return to player.
- So the player can easily achieve the 1,000,000 SCORE with this option.
- The player can achieve 1,000,000 fast by choosing 100,000 difficulty.

### Phase 2. SCORE : 1,000,000 to 1,000,000,000
- Choosing the 4th or 5th option in Type A free game options, and changing the free game options into Type B, a player can access the out-of-bound of Type B free game options.
- Using this bug, the player can choose the free game option of `freespin_trigger` Free Plays with Multiplier `total_spin`, or `maximum_win` Free Plays with Multiplier `freespin_total`. (See the following code.)  
- `maximum_win` is at least 20,000 if it is not zero. So choosing this free game option makes the game never halted, because of the re-trigger.
- Having about 1,000,000 SCORE and the bet is `100000`, the player has to get 1000~2147 times of the bet in the single free game trigger.
	- Note that the `money` variable is 4-byte `int` type so the expectation has to be under 2147 times of the bet.
- The expectation of the single free game trigger can be calculated by `(total_spin) * (freespin_trigger) / (1 - 5.5% * freespin_trigger)`.
	- Note that 5.5% is the probability of re-triggering inside free game.
	- If the `freespin_trigger` is larger than 18, the expectation diverges, i.e. the game is never halted.
- So the player has to play with the lower bet until the expectation reaches the range between 1000 and 2147, and after it reaches, the bet would be changed into `100000`.

```
struct bonus_info bonusinfo1[] = {
	{12, 1},
	{6, 2},
	{4, 3},
	{3, 4},
	{2, 6}
};
struct bonus_info bonusinfo2[] = {
	{5, 2},
	{2, 5},
	{1, 10}
};
int total_spin = -1;
int freespin_trigger = -1;
int freespin_total = -1;
int maximum_win = -1;
```

## Less intended solution
Instead using OOB in the Phase 2, just choose the 3rd option in the combination B. (10 times free game with multiplier 1)  
Because of its high expectation return value, a player has a little chance to reach 1,000,000,000 from 1,000,000.  
Any bet could make it but the maximum bet is most efficient.  
The probability by this strategy is 0.1% after Phase 1, it is lower than that of the intented but reasonable.  

## Statistics
The following is the result of simulation for the above solution strategy.  
Total Simulation Count : 487K (486,957)  
Phase 1 : 17,991 (3.7%)  
Phase 2 : 1,153 (0.24%, or 6.4% after Phase 1)  

## Typo
- Change `multipler` into `multiplier` in the `help` menu.
