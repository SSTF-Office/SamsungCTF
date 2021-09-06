[Info]
Title:bofsb
Description:Do you wanna be a hacker? Win the game!
Attach:bofsb.zip

[Deploy]
 env: sandbox, elf 32bit execution
 1. put server files into challenge directory
 2. socat with bofsb

[Solving Strategy]
 1. color selection is not important. select whatever
 2. magic code means stack address (address of v5)
 3. calculate v4's address (v5_addr - 4)
 4. make fsb attack payload: v4_addr || AAA... || %08x%n
    this will modify v4's value (number of 'A' means that)
 5. make bof attack payload: payload || BBB... || v5_addr
    this will modify 'foramt' variable points v5
 6. send payload as name
 7. fsb, bof works
 8. get the flag!
