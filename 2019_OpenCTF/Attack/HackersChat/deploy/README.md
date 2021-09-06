
CTF TASK Hacking Chat

1. Description

The task is tie with exploitation of memory and it is divided in to stage. First stage is tie with memory leak where the participant needs to leak admin password by write to many data to username. if the participant have admin password he can log into service where he can create, delete and read messages. in this stage the participant must corrupt heap to get possibility to arbitrary code execution. by this stage he should have access to system and he can read key which is needed to decrypt messages in read messages function of the program. Also he can decrypt messages manually by read file message.s and use openssl to decrypt it.

1.1 Run environment with docker

To run it you need to have installed:
- docker
- docker-compose
Command used to run it:
- sudo docker-compose up --build

after download debian distro and run all you should have open port at 1337 address where you can connect and test ctf task.

1.2 Run binary locally

- LD_LIBRARY_PATH=. ./hackerChat

2. Exploitation

2.1 First stage - memory leak

We connect to the task and send as username random 32 bytes. The program return to as that we cannot log into as random 32 bytes plus password for admin account. In this situation we have function which check given credential with credential save in file cred_file.psw. The read credential and given are on the stack in 32 bytes buffers. When we write 32 bytes to the buffer we have overwrite because the null byte was wrote to the password read from file buffer. The password read from file buffer will be filled in stage wwhere the check stage is executed. By this we lost null byte at the end of username given buffer and we can leak admin password(the admin credential is the last in reading file).

2.2 Second stage - heap exploitation

2.2.1 Prepare memory

2.2.1-1 Create four chunks on heap

First of all we need to create 4 chunks where 2 are small chunks and 2 are fast chunks.
we can do that by choose option 2(create message) in program menu and sends:
- 0xf7 bytes to the server - create small chunk
- 0x67 bytes to the server - create fast chunk
- 0xf7 bytes to the server - create small chunk
- 0x10 bytes to the server - create fast chunk - use as protection against stack corruption

After that we need to remove first chunk and second by choose in menu option 3(delete message) and send 0(number of message) in both cases. Why? Because the program has implemented linked list to store messages, and if we delete first message, the next message change its position from 1 to 0. 

After that We create again fast chunk which will be stored in memory where the last fast chunk was, but in this situation we write  0x68 Bytes. This operation lets us clear flags in third chunk(flag in chunk are on the first byte and inform whether previous chunk was freed). Why? Because the algorithm which adds messages use strcpy to copy message from buffer to the allocate memory and on the end of message adds null byte where the null bute clear flags in next chunk.

Now we can create fake chunk which lets us on leak libc address. First we delete fast chunk by choose 3 option and send 2 because we want delete latest added message. Next we create fake chunk by send 'B'*0x66+'\x70\x01'. We repeat operation delete and add 7 times to insert null bytes to memory by reduce 'B' chars in each operation.

2.2.2 Leaking libc address

Before Leaks address we need to delete third chunk, this couses consolidation with the fakechunk and create large free chunk which will be use to leak libc address.

Now we are leaking libc address by create new small chunk with size 0xf6.

Reading data from fastchunk cause leak libc address. We can do that by print command in menu. 

2.2.3 Write arbitrary address to the _ _malloc__hook and spawn shell

This is the hardest stage in whole exploitation.

First you need clear up all change in heap by restore modifed fastchunk.

We can do that by delete and create 6 times latest added chunk and write to memory 'E'+i where i is 0xfd to 0xf7.

After that we can delete fastchunk and latest added small chunk.

Next we create smallchunk like 'F'*0x100 + address of __malloc_hook (you can get this address by run application in gdb and write command print /x &__malloc_hook. This is need to calculate offset of __malloc_hook from base_address of libc)

Ok, we have address __malloc_hook in memory, now we need restore freed fastchunk frame because we destroy it by wrote __malloc_hook address in Forward Pointer of Linked list of fastchunk). We can do that by delete and create message 'F'*i+'\x70' where i is from 0xfe to 0xf7.

After that we can create fast chunk which cause that next fast chunk will be create at __malloc_hook address ;). Now we can write address of one_gadget from libc(you can use one_gadget apps to find gadget from libc). Creating next fast chunk with address of gadget we modified __hook_malloc what cause that the next creation of chunk run our gadget. ;)	

2.2.4 decryption of comunication

To resolve challenge the participant must read key from k@q.k file and use it in print option in aplication to decrypt flag. Also he can read message.s file and decrypt it locally on his computer by use openssl application.
