TITLE=I_Love_Cubic
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist

CC = gcc
CFLAGS = -W -Wall
TARGET = cubic
OBJECTS = cubic.o

all: deploy

deploy: $(TARGET)
	mkdir -p $(DIST_DIR)
	cp $(TARGET) $(DIST_DIR)/
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*

$(target) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(DIST_DIR) $(TARGET) $(OBJECTS)

allclean:
	rm -rf $(DIST_DIR) $(TARGET) $(OBJECTS) $(DEPLOY_DIR)/download/*
