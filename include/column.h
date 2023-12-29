#ifndef COL_H
#define COL_H

#include <cstdint>

#define COLUMN_SIZE_IN_BYTES 21

class Column{
public:
    char name[20];
	char type;
};

#endif // COL_H