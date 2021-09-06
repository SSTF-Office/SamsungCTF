TITLE=Hidden_Clues
DEPLOY_DIR=../deploy
DIST_DIR=home/badhacker
SERVER_IP=10.113.108.125

all: deploy

deploy: prs.py
	mkdir -p $(DIST_DIR)
	mkdir $(DIST_DIR)/Desktop $(DIST_DIR)/Documents $(DIST_DIR)/Downloads $(DIST_DIR)/Music
	mkdir $(DIST_DIR)/Pictures $(DIST_DIR)/Public $(DIST_DIR)/Templates $(DIST_DIR)/Videos
	python shuffle.py prs.py $(DIST_DIR)/payload
	python make_history.py $(SERVER_IP)
	mv .bash_history $(DIST_DIR)/
	cp bashrc $(DIST_DIR)/.bashrc
	rm -f $(DEPLOY_DIR)/download/$(TITLE).zip
	zip -r $(DEPLOY_DIR)/download/$(TITLE).zip home
	cp prs.py $(DEPLOY_DIR)/server/
	cp $(DEPLOY_DIR)/flag $(DEPLOY_DIR)/server/

clean:
	rm -rf $(DIST_DIR) home

allclean:
	rm -rf $(DIST_DIR) home $(DEPLOY_DIR)/download/* $(DEPLOY_DIR)/server/*
