TITLE=SecureRunner2
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist
CC = gcc
CFLAGS = #-D_DEBUG_
LFLAGS = -lgmp
TARGET = $(TITLE)
OBJECTS = chal.o rsa.o

all: deploy

deploy: $(TARGET)
	mkdir -p $(DIST_DIR)
	mkdir -p $(DEPLOY_DIR)/server/src
	cp $(TARGET) $(DIST_DIR)/
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*
	cp $(TARGET) $(DEPLOY_DIR)/server/src/challenge
	cp $(DEPLOY_DIR)/flag.txt $(DEPLOY_DIR)/server/src/flag.txt
	cp $(TARGET) $(EXPLOIT_DIR)/

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)


clean:
	rm -rf $(DIST_DIR) $(TARGET) $(OBJECTS)

allclean:
	rm -rf $(DIST_DIR) $(TARGET) $(OBJECTS) $(DEPLOY_DIR)/server/challenge  $(DEPLOY_DIR)/download/*
