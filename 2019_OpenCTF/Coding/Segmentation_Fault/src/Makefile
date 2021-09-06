TITLE=Segmentation_Fault
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist
SRC=gen_pychal.py
TARGET=chal.py

all: deploy

deploy:
	mkdir -p $(DIST_DIR)
	cp arduino_test.py $(DIST_DIR)/
	python gen_pychal.py > $(DIST_DIR)/$(TARGET)
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*
	cp $(DIST_DIR)/$(TARGET) $(EXPLOIT_DIR)/
	cp arduino_test.py $(EXPLOIT_DIR)/

clean:
	rm -rf $(DIST_DIR)

allclean:
	rm -rf $(DIST_DIR) $(EXPLOIT_DIR)/$(TARGET)* $(EXPLOIT_DIR)/arduino_test.py* $(DEPLOY_DIR)/download/*
