all:
	cd jni && python obfus.py
	ndk-build
	ant release
clean:
	ant clean
	rm -rf libs/ obj/
