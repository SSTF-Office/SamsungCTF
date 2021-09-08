CC=fsharpc
EXECUTABLE=F\#oker.exe

all: $(EXECUTABLE)

$(EXECUTABLE): yo.fs
	$(CC) -o $@ $<
	chmod +x $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
