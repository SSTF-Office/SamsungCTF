#!/usr/bin/env python3

import plyvel
import os

fpath = os.path.expanduser("~/.config/maze-adventure/Local Storage/leveldb")
db = plyvel.DB(fpath)

# for key, value in db:
#     print (key, value)

db.put(b'_file://\x00\x01MONEY', b'\x019876543210')
db.put(b'_file://\x00\x01PATHFINDER_ENABLED', b'\x01true')
db.put(b'_file://\x00\x01STG_ACCESS_INFO', b'\x01[true,true,true,true]')

db.close()