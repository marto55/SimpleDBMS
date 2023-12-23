#include "bitmap.h"
#include <bits/stdc++.h> 

using namespace std; 

#define NUMBER_OF_BITS (8 * sizeof(unsigned int))

void bitmap::init(int size){
    this->size = size;
    array = new int[size];
    memset(array, 0, size);
}

void bitmap::set(int index){
    array[index/NUMBER_OF_BITS] |= (1 << (index%NUMBER_OF_BITS));
}

void bitmap::reset(int index){
    array[index/NUMBER_OF_BITS] &= ~(1 << (index%NUMBER_OF_BITS));
}

bool bitmap::test(int index){
    return ( (array[index/NUMBER_OF_BITS] & (1<<(index %NUMBER_OF_BITS))) != 0 );
}

void bitmap::flip(int index){
    array[index/NUMBER_OF_BITS] ^= (1 << (index%NUMBER_OF_BITS));
}
