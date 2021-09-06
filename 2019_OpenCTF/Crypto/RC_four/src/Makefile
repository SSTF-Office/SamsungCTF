TITLE=RC_four
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist

all: deploy

deploy: challenge.py ../deploy/flag
	mkdir -p $(DIST_DIR)
	echo `python -c 'print "key = \"Wow, this is a reeeeeeeeeeeeeeeeeeeeeally secret key, so nobody can guess this key :) with this tailing: bWF0dGEgaXMgbm90IGJhZA==\""'` > secret.py
	echo `python -c 'print "flag=\"" + open("../deploy/flag").read() + "\""'` >> secret.py
	python challenge.py > $(DIST_DIR)/output.txt
	cp challenge.py $(DIST_DIR)/
	cp $(DIST_DIR)/output.txt $(EXPLOIT_DIR)/output.txt
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*

clean:
	rm -rf $(DIST_DIR) secret.py*

allclean:
	rm -rf $(DIST_DIR) secret.py* $(DEPLOY_DIR)/download/* $(EXPLOIT_DIR)/output.txt
