# Great Makefile
CC:= g++
FLAGS:= -Iinclude -Wall -Wextra -pedantic -std=c++17

INC:= include/
SRC:= sources/
OBJ:= build/

OBJECTS:= $(OBJ)database.o $(OBJ)column.o $(OBJ)table.o
SOURCES:= $(SRC)database.cpp $(SRC)column.cpp $(SRC)table.cpp $(SRC)main.cpp

all:output

output: $(OBJECTS)
	$(CC) $(FLAGS) $(SOURCES) -o $@.exe

$(OBJ)%.o: $(SRC)%.cpp
	$(CC) -c $(FLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(OBJ)*.o *.exe