#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include "constants.h"

class Bitmap{
private:
    int offset;
    char array[MEMORY_MAP_SIZE_IN_BYTES];
public:
    void init(char values[MEMORY_MAP_SIZE_IN_BYTES], int offs);
    void set(int index);
    void reset(int index);
    bool test(int index);
    void flip(int index);
    void print_bytes_as_ints();

    ~Bitmap();
};

#endif // BMP_H