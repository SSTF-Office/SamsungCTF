1.execute a binary file and understand ASAN log

2.reverse the binary and find a reason

3.write a proper code and save as user.cpp

4.compile with main.cpp (g++ main.cpp user.cpp -o user_binary)

5.encode user_binary file with base64

5.upload output file



## compile option
g++ main.cpp user.cpp -o user_binary

## execute
user_binary < sample_input.txt

## base64 encoding
base64 -w 0 user_binary > encoded_user_binary

## upload your 'encoded_user_binary'
(cat encoded_user_binary; echo "") | nc codingmachine.sstf.site 9000

----------------------------------------------------------------------------------------------------



1.ASAN�α�

num_size = 70 �̰�

ptr�� unsigned long long (ull)�� ����Ű�� �����͵��� �迭�̴�.

==12657==ERROR: AddressSanitizer: global-buffer-overflow on address 0x000000e58250 at pc 0x0000004f94a1 bp 0x7fffffffdac0 sp 0x7fffffffdab8 READ of size 8 at 0x000000e58250 thread T0

0x000000e58250 is located 0 bytes to the right of global variable 'ptr' defined in 'main.cpp:7:6' (0xe58020) of size 560




0xe58250���� global-buffer-overflow�� ����Ų��. 0xe58250 - 0xe58020(address of ptr) = 0x230 =560 = 8(size of ull) * 70 �ΰ��� ���� ptr[69]���� ������ �������� �� �� �ִ�.




2. ������

main(){

operator>>((basic_istream<char,std--char_traits<char>> *)cin,(long_long *)ptr[0]);    

operator>>((basic_istream<char,std--char_traits<char>> *)cin,(long_long *)ptr[1]);

}

myfunc(i){

...����...

ppuVar4 = ptr + (long)i;

...����...

puVar1 = (ulonglong *)operator.new(8);

ptr[(long)i] = puVar1;    

uVar2 = myfunc(i + -1);    

 uVar3 = myfunc(i + -2);    

local_10 = uVar2 + uVar3 & 0xfffffff;    

ppuVar4 = ptr + (long)i;

puVar1 = *ppuVar4;

*puVar1 = local_10;

...����...  

}

������ ������ ���� �̿��Ͽ� ������ �ľ��ϸ� �Ǻ���ġ ������ ���ϴ� ������ �޸������̼� ����� ��� (https://en.wikipedia.org/wiki/Memoization, https://namu.wiki/w/%EB%A9%94%EB%AA%A8%EC%9D%B4%EC%A0%9C%EC%9D%B4%EC%85%98)�� ���Ǿ����� �� �� �ִ�.
?myfunc(i) = (myfunc(i-1) + myfunc(i-2))&0xfffffff
?ptr[69]�� ���� �ʰ� �ڵ�

3. user.cpp

#define ull unsigned long long

extern ull* ptr[100];

extern int num_size;

ull myfunc(int i){  

if(ptr[i]!=0) return *ptr[i];  

ptr[i] = new ull;  

return *ptr[i] = (myfunc(i-1)+myfunc(i-2))&0x0FFFFFFF;

}

void test(){  

for(int i=0; i<num_size; i++)    

myfunc(i);

}
