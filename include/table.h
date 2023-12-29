#ifndef TBL_H
#define TBL_H

#include "column.h"

class Table{
public:
	char name[20];
    char columns[ 10 * COLUMN_SIZE_IN_BYTES];
};

#endif // TBL_H