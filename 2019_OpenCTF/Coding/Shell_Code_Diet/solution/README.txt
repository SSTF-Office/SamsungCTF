1. open & read ctfToken

ctfToken file is located in same directory with server binary, so you just call with simple path to read the file.

example : open ( "ctfToken","r" ) 




1) push the ctfToken file's path parameter 

2) set the 'open' call parameter register

3) call open system call




4) get the open's parameter register

5) set the 'read' parameter

6) call read system call

you get the contents of "ctfToken" file -> "To be hashed text"

 
2. hash the read data (hash function is hand-made & located in somewhere)




First, you must find the hand-made digest function somewhere in the code. 

The digest function starts from 0x4025bd .

Carefully cut-off function's prologue/epillogue ¡æ only get the digest core logic




Digest function  is implemted with macro with sample parameters , so you have to analyze binary to get macro's parameter

belows are hints

...

4025fa:       83 f8 07                cmp    $0x7,%eax    (it is sample string length => loop starts from 0~7)

...

402604:       0f b6 80 92 c0 40 00    movzbl 0x40c092(%rax),%eax (it is sample input string "HashFunc")

 

7) set the digest function's parameter

8)  call digest core logic (copy & paste asm code)

9) you get the digest value. 

 

 

 
3. set the parameters -> call the decrypt function 

before do this, you have to find decrypt function's address & parameter's




10)  prepare parameters ( digest value .. other dummy parameters ...)

11) call the decrypt function
12)put together whole assembly codes of (1~11) 
 
4. Submit the bytes codes to server
