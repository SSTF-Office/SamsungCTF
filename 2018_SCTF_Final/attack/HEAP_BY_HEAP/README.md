## Dear pwnabler.
#### Here is binary and system is ubuntu 16.04
#### Good luck.

## Solution 

#### 1. The point is that getting arbitary memory write  when you just have off by one vulnerability.

#### 2. You can change node size only one byte using setting password function.

#### 3. You can trick heap size when heap free.

#### 4. And than, next allocation is overlaped by other chunk.

#### 5. You should allocate make_hash function pointer to overlaped chunk.

#### 6. you can change make_hash function to system@plt address.





Here is off by one bug.
because "scanf" has null terminator

password size is 56 bytes. 
```
117 void SET_PASSWORD()
118 {
119 →   if(password == NULL)
120 →   {
121 →   →   printf("\n[+] Input password (length : 6~55)\n");
122 →   →   password = ( char *) malloc(56); // 56
123 →   →   if(password != NULL)
124 →   →   {
125 →   →   →   memset(password,0,56);
126 →   →   →   scanf("%56s",password);
127 →   →   →   printf("PW: %s\n",password);
128 →   →   }
129 →   }
130 }
```

There is set comment function 
 "void SET_COMMENT(NODE \*v1 , unsigned int v2)"

you can make heap what you want size using this function.




```
