#! /usr/bin/python3

import sys
import logging
import os

logging.basicConfig(stream=sys.stderr)
sys.path.insert(0,"/var/www/pilcve")

from app import app as application

if __name__ == "__main__":
    print("app:", application)
