DX=~/.local/android-sdk-linux/build-tools/23.0.2/dx

all:
	javac com/example/egg/*.java
	jar cf egg.jar com/
	$(DX) --dex --output=egg.dex egg.jar
	python encrypt.py <egg.dex >egg
clean:
	rm -f com/example/egg/*.class egg egg.dex egg.jar
