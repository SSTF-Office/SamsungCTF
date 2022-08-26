import re
import requests
import os
import sys
import string
import random
import hashlib
from timeout_decorator import timeout

# PoWdle config
DIFFICULTY = 5
ROUNDS = 5
HIGHSCORE = 3000

green = "\033[42m {} \033[0m "
yellow = "\033[43m {} \033[0m "
gray = "\033[100m {} \033[0m "

# HoF server api
hof_server = 'http://hofserver/hof'

@timeout(30)
def gets(s=None):
    if s:
        print(s, end='', flush=True)
    data = sys.stdin.readline(0x1000)
    return data.strip('\n')


@timeout(5)
def hall_of_fame(email, score):
    email_re = "^(\S*)@\S*\.\S*$"
    m = re.match(email_re, email)
    if not m:
        print("Use a valid Email :(")
        return None
    else:
        name = m[1]
        print("Congratulations {}, you'll be inducted to the hall of fame!".format(name))
        r = requests.post(hof_server, json={'score': score, 'email': email})
        response = r.json()
        rank = response['rank']
        print("You are at rank #{}".format(rank))
        return rank


def clear(email, score):
    print("Good job!")
    print("Your score is {}".format(score))
    if score > HIGHSCORE:
        try:
            if hall_of_fame(email, score) == 1:
                print("What? Impossible!")
                os.system("cat flag")
        except:
            print("Sorry, HoF server seems busy, I'll handle it later.")
            os.system('echo "{},{}" >> ./HOF_TODO_LIST'.format(score, email))


def pow(s, difficulty=DIFFICULTY):
    m = hashlib.sha256()
    m.update(s.encode())
    h = m.hexdigest()[:difficulty]
    return h


@timeout(60)
def powdle(goal, challenge):
    board = ""
    trial = 1
    while trial <= 10:
        guess = gets("Guess #{}: ".format(trial))
        if not guess.startswith(challenge):
            print("Invalid Input!")
            continue
        guess = pow(guess)
        for i, c in enumerate(guess):
            if guess[i] == goal[i]:
                board += green.format(guess[i])
            elif guess[i] in [ c for j,c in enumerate(goal) if c != guess[j] ] :
                board += yellow.format(guess[i])
            else:
                board += gray.format(guess[i])
        board += '\n'
        print(board)
        if guess == goal:
            print("Correct!")
            return 2200-(200*trial)
        else:
            print("Try again!")
        trial += 1
    print("Wrong! The answer was {}".format(goal))
    return 0


def main():
    print("Welcome to Proof-of-Workdle!")
    print("You should be hurry!")
    email = gets("Give me your email: ")

    score = 0
    for i in range(ROUNDS):
        challenge = ''.join([ random.choice(string.ascii_letters) for i in range(10) ])
        goal = ''.join([ random.choice('0123456789abcdef') for i in range(DIFFICULTY)])
        print("Round {}/{}".format(i+1, ROUNDS))
        print("Challenge: {}\n".format(challenge))
        try:
            score += powdle(goal, challenge)
            print("Current score: {}\n".format(score))
        except:
            print("Too slow!")
            return

    if score != 0:
        clear(email, score)
    else:
        print("Workdle harder!")


if __name__ == "__main__":
    main()
