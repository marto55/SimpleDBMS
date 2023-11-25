
output: main.o sup.o
	g++ -idirafter ../Headers/ main.cpp sup.cpp -o output.exe

main.o: main.cpp
	g++ -idirafter ../Headers/ -c main.cpp

sup.o: sup.cpp ../Headers/sup.h
	g++ -idirafter ../Headers/ -c sup.cpp

clean:
	rm *.o *.exe
