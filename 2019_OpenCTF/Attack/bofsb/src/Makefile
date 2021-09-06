TITLE=bofsb
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist
CC = gcc
CFLAGS = -m32 -mpreferred-stack-boundary=2 -O0 -W -Wall
TARGET = bofsb
OBJECTS = bofsb.o

all: deploy

deploy: $(TARGET)
	mkdir -p $(DIST_DIR)
	cp $(TARGET) $(DIST_DIR)/
	cp $(DEPLOY_DIR)/bofsb.rev.c $(DIST_DIR)/
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*
	cp $(TARGET) $(DEPLOY_DIR)/server/
	cp $(DEPLOY_DIR)/flag $(DEPLOY_DIR)/server/
	cp $(TARGET) $(EXPLOIT_DIR)/

$(target) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -rf $(DIST_DIR) $(TARGET) $(OBJECTS)

allclean:
	rm -rf $(DIST_DIR) $(TARGET) $(OBJECTS) $(DEPLOY_DIR)/server/* $(DEPLOY_DIR)/download/*
