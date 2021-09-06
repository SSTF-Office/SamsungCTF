OPTION="-Ttext=0x011d2000"
default :
	arm-linux-gnueabihf-gcc-8 $(OPTION) -o ./prob ./prob.c -fno-stack-protector -no-pie
	arm-linux-gnueabihf-strip ./prob
	
