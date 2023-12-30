#ifndef TBL_H
#define TBL_H

#include "column.h"

class Table{
public:
	char name[STRING_SIZE];
    char columns[ MAX_NUMBER_OF_COLUMNS * COLUMN_SIZE_IN_BYTES];
};

#endif // TBL_H