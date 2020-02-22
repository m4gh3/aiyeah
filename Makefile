xor_test: xor_test.cpp build/cells.o
	g++ xor_test.cpp build/cells.o build/desc.o build/diffbl.o -g -o xor_test
libaiyeah.so: build/cells.o build/desc.o build/diffbl.o
	g++ -shared build/*.o -o libaiyeah.so
build/cells.o: cells.cpp build/desc.o
	g++ cells.cpp build/desc.o build/diffbl.o -g -c -o build/cells.o
build/desc.o: desc.cpp build/diffbl.o
	g++ desc.cpp build/diffbl.o -g -c -o build/desc.o
build/diffbl.o: diffbl.cpp
	g++ diffbl.cpp -g -c -o build/diffbl.o
