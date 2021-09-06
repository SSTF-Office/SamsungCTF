TITLE=Roughly_Secure_Algorithm
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist

all: deploy

deploy: challenge.py ../deploy/flag
	mkdir -p $(DIST_DIR)
	python challenge.py
	cp challenge.py $(DIST_DIR)/
	cp ciphertext.txt $(DIST_DIR)/
	mv ciphertext.txt $(EXPLOIT_DIR)/
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*

clean:
	rm -rf $(DIST_DIR)

allclean:
	rm -rf $(DIST_DIR) $(DEPLOY_DIR)/download/* $(EXPLOIT_DIR)/ciphertext.txt
