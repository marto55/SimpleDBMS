#ifndef COL_H
#define COL_H

#include <cstdint>
#include <string>
#include "constants.h"

using std::string;

class Column{
    char name[DATABASE_STRING_SIZE];
	char type;
    char column_default[DATABASE_STRING_SIZE];
public:
    void init(string name, char type, string column_default);
    // Getter declarations
    char* get_name();
    char get_type();
    char* get_column_default();
};

#endif // COL_H