demofile: main.o loader.o
	g++ main.o loader.o -o demo  -lbfd
	rm *.o

main.o : main.cpp
	g++ -c main.cpp


loader.o : loader.cpp loader.h
	g++ -c loader.cpp
