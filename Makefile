# Great Makefile
CC:= g++
FLAGS:= -Iinclude -Wall -Wextra -pedantic -std=c++17

SRC:= sources/
OBJ:= build/

OBJECTS:= $(OBJ)database.o $(OBJ)column.o $(OBJ)table.o $(OBJ)bitmap.o $(OBJ)input_reader.o $(OBJ)hash_functions.o
SOURCES:= $(SRC)*.cpp

$(shell mkdir -p $(OBJ))

all:output

output: $(OBJECTS)
	$(CC) $(FLAGS) $(SOURCES) -o $@.exe

$(OBJ)%.o: $(SRC)%.cpp
	$(CC) -c $(FLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(OBJ)*.o *.exe