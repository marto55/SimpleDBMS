#ifndef COL_H
#define COL_H

#include <cstdint>
#include <string>
#include "constants.h"

using std::string;

#define COLUMN_SIZE_IN_BYTES 21

class Column{
    char name[DATABASE_STRING_SIZE];
	char type;
public:
    void init(string name, char type);
};

#endif // COL_H