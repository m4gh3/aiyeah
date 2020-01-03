desc: cells.cpp build/desc.o
	g++ cells.cpp build/desc.o build/diffbl.o -O3 -std=c++2a -o cells
build/desc.o: desc.cpp build/diffbl.o
	g++ desc.cpp build/diffbl.o -std=c++2a -O3 -c -o build/desc.o
build/diffbl.o: diffbl.cpp
	g++ diffbl.cpp -O3 -c -o build/diffbl.o
