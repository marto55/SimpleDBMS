# -- Makefile --

CC:= g++
FLAGS:= -idirafter Headers/

HED:= Headers/
SRC:= Sources/
OBJ:= Object_files/

output: $(OBJ)main.o $(OBJ)sup.o
	$(CC) $(FLAGS) $(SRC)main.cpp $(SRC)sup.cpp -o output.exe

$(OBJ)main.o: $(SRC)main.cpp
	$(CC) $(FLAGS) -c $(SRC)main.cpp -o $(OBJ)main.o

$(OBJ)sup.o: $(SRC)sup.cpp $(HED)sup.h
	$(CC) $(FLAGS) -c $(SRC)sup.cpp -o $(OBJ)sup.o

clean:
	rm $(OBJ)*.o *.exe
