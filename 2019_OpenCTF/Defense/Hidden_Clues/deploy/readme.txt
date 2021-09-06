[Info]
Title:Hidden Clues
Description:Police and system operators of A-Server, attacked by hackers, traced the hacker's penetration route and succeeded in capturing the hacker's account on the attack server.<br><br>The security vulnerabilities used in the attack is mitigated, but how they're used in the exploitation is still under investigation.<br><br>What can you find from the clues left in the hacker's account?
Attach:Hidden_Clues.zip

[Deploy]
 env: sandbox, python 2.7
 1. put server files into challenge directory
 2. chmod +x prs.py
 3. socat with prs.py

[Solving Strategy]
 1. find and analyze hidden files
 2. restore hacker's script
 3. make reverse connection using hacker's script
 4. get the flag!
