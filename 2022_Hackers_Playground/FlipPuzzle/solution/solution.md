# FlipPuzzle Solution
There are multiple solution for FlipPuzzle challenge.
1. Pre-compute all possible input cases and solution and response each prepared solution for input.
2. Solve puzzle with A* algorithm

## Pre-computing all possible cases
Puzzle shuffle operation is done at most 11 times. For each operation, 4 direction to shuffle, thus number of shuffle result is at most 4^11.
As maximum number of shuffle result is 4^11(=4194304), which is proper amount for computing machine to pre-compute all possible cases prior to connect to the server.
Just dump pre-computed answer for each test cases make you get the flag.

Implementation for this solution is up to you.

## A* search algorithm
If writing heuristic function is not so hard, A* algorithm can be one of good choice to solve some problem.
Solving 15 puzzle with A* algorithm is famous example. This flip puzzle challenge is similar to 15 puzzle.

A* search algorithm is very similar to BFS(Breadth-First Search). Although BFS uses queue to store next searchable state, A* search algorithm uses priority queue to store next searchable state.
Priority of each state is defined estimated distance. Estimated distance should not be smaller than real distance.
In this problem we can simply define estimated distance as sum of currently moved distance and number of out of place puzzle slots.

Utilizing A* search algorithm, you can solve each puzzle very quickly.

Implementation for this solution is enclosed on `solver.py`.