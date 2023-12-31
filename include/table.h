#ifndef TBL_H
#define TBL_H

#include <string>
#include "column.h"

using std::string;

class Table{
private:
    char name[DATABASE_STRING_SIZE];
    char columns[ MAX_NUMBER_OF_COLUMNS * COLUMN_SIZE_IN_BYTES];
public:
	void init(string name, Column columns[MAX_NUMBER_OF_COLUMNS]);
};

#endif // TBL_H