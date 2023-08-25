#!/usr/bin/env python3
import os
from subprocess import check_output as exec

PROJECTS_PATH = "../handout/projects/"

repos = os.listdir(PROJECTS_PATH)

# Checking the last-commit time, you can find what is code repo of live server.
for repo in repos:
    time = exec("git show -s --format=%ci HEAD".split(" "), cwd=os.path.join(PROJECTS_PATH, repo)).strip().decode()
    year = int(time[:4])
    if year > 2016:
        print (f"Recent! {repo}: {year}")
    else:
        continue
        print (f"Repo: {repo} / Last Commit Year : {year} / Last Commit Time : {time}")