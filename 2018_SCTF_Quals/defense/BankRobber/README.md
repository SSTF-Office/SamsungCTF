# BankRobber 

## Description
SCTFBank looks vulnerable...
Patch and protect it from Bankrobbers!

`nc bankrobber.eatpwnnosleep.com 4567`

[SCTFBank.sol](#)


## Flag
SCTF{sorry_this_transaction_was_sent_by_my_cat}


## How to solve
There are 4 vulnerabilities and 4 robbers (Alpha, Bravo, Charlie, Delta).
- Alpha: No verification of balance in donate function.
- Bravo: Integer overflow in multiTransfer function.
- Charlie: Re-entrancy in withdraw function.
- Delta: Use of tx.origin in sender verification.

To get the flag, all vulnerabilities should be patched without changing functionalities.
