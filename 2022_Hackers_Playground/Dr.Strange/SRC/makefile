CC=gcc -std=c99 -O0
ARMCC=aarch64-linux-gnu-gcc-10 -O0
BINARY1=AttackMe
BINARY2=exploit
LIBRARY=pd
OPTION=-lrt -L./ -lpd
default:
	$(ARMCC) -fPIC -c ${LIBRARY}.c
	$(ARMCC) -shared -Wl,-soname,libpd.so ${LIBRARY}.o -o lib${LIBRARY}.so
	$(ARMCC) ${BINARY1}.c -DARM -o ${BINARY1} ${OPTION}
	$(ARMCC) ${BINARY2}.c -DARM -o ${BINARY2} ${OPTION}  



clean:
	rm *.o ${BINARY1} ${BINARY2} lib${LIBRARY}.so
