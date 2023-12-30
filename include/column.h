#ifndef COL_H
#define COL_H

#include <cstdint>
#include "constants.h"

#define COLUMN_SIZE_IN_BYTES 21

class Column{
public:
    char name[STRING_SIZE];
	char type;
};

#endif // COL_H