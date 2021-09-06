import os
os.environ['CLINT_FORCE_COLOR'] = '1'
del os

from clint.textui import colored
start = \
"""


.---.
  |         o
  |.  . .--..  .--. .-..
  ||  | |   |  |  |(   |
  '`--`-' -' `-'  `-`-`|
       .--.         ._.'         .       .
      :                         _|_   o _|_   o
      |    .-. .--.--. .,-.  .-. |    .  |    .  .-. .--.
      :   (   )|  |  | |   )(.-' |    |  |    | (   )|  |
       `--'`-' '  '  `-|`-'  `--'`-'-' `-`-'-' `-`-' '  `-
                       |
                       '
"""

stage1 = \
"""
==================== [{0}] =====================
Warm up! {0}.
""".format(colored.green("'0' * n + '1' * m where (n>=0 and m>=0)"))

stage2 = \
"""
==================== [{0}] =====================
something hard: {0}.
""".format(colored.green("'1'*x where (7 * x) % 13 == 1"))

stage3 = \
"""
==================== [{0}] =====================
from regular to PDA: {0}.
""".format(colored.green("'0'*n + '1'*n where (n>=0)"))

stage4 = \
"""
==================== [{0}] =====================
Not PDA!: {0}.
""".format(colored.green("'0'*p where p is prime"))


infos = [stage1, stage2, stage3, stage4]
