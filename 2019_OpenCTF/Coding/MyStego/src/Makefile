TITLE=MyStego
DEPLOY_DIR=../deploy
EXPLOIT_DIR=../exploit
DIST_DIR=dist
SRC=challenge.ml
TARGET=stego
IN_FILE=sstf.png
OUT_FILE=challenge.png
EMBED=flag

all: deploy

deploy: $(TARGET)
	mkdir -p $(DIST_DIR)
	cp $(SRC) $(DIST_DIR)/
	./$(TARGET) $(DEPLOY_DIR)/$(IN_FILE) $(DIST_DIR)/$(OUT_FILE) $(DEPLOY_DIR)/$(EMBED)
	zip -j $(DEPLOY_DIR)/download/$(TITLE).zip $(DIST_DIR)/*
	cp $(DIST_DIR)/$(OUT_FILE) $(EXPLOIT_DIR)/

$(TARGET): 
	ocamlfind ocamlc -o $(TARGET) -package graphics -package unix \
	-package camlimages.png -package camlimages.graphics \
	-linkpkg $(SRC)

clean:
	rm -rf $(DIST_DIR) $(TARGET) *.cmi *.cmo

allclean:
	rm -rf $(DIST_DIR) $(TARGET) *.cmi *.cmo $(DEPLOY_DIR)/download/* $(EXPLOIT_DIR)/$(OUT_FILE)
