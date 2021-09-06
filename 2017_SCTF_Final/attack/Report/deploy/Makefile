CPP = g++
CPPFLAGS = -Wl,-z,relro,-z,now
SRC = Score.cpp

Report : $(SRC) 
	$(CPP) $(CPPFLAGS) $(SRC)
	python p.py >/dev/null
	rm a.out
	chmod +x Report

