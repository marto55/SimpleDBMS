#include <bits/stdc++.h>
#include <cstring>

#include "Bitmap.h"
#include "constants.h"

using namespace std;

void Bitmap::init(char values[MEMORY_MAP_SIZE], int offs){
    offset = offs;
    for(int i =0; i<MEMORY_MAP_SIZE; i++){
        array[i] = values[i];
    }
}

void Bitmap::set(int index){
    array[index/NUMBER_OF_BITS] |= (1 << (index%NUMBER_OF_BITS));

    fstream file;
    file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
    if (!file.is_open()){
        // signal to the user if the file coudn't be opened
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{
        // update bitmap in memory too
        file.seekp(offset, ios::beg);
        file.write((char*)array, MEMORY_MAP_SIZE);
    }
    file.close();
}

void Bitmap::reset(int index){
    array[index/NUMBER_OF_BITS] &= ~(1 << (index%NUMBER_OF_BITS));

    fstream file;
    file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
    if (!file.is_open()){
        // signal to the user if the file coudn't be opened
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{
        // update bitmap in memory too
        file.seekp(offset, ios::beg);
        file.write((char*)array, MEMORY_MAP_SIZE);
    }
    file.close();
}

bool Bitmap::test(int index){
    return ( (array[index/NUMBER_OF_BITS] & (1<<(index %NUMBER_OF_BITS))) != 0 );
}

void Bitmap::flip(int index){
    array[index/NUMBER_OF_BITS] ^= (1 << (index%NUMBER_OF_BITS));

    fstream file;
    file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
    if (!file.is_open()){
        // signal to the user if the file coudn't be created
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{
        // update bitmap in memory too
        file.seekp(offset, ios::beg);
        file.write(array, sizeof(array));
    }
    file.close();
}

void Bitmap::print_bytes_as_ints(){
    for(int i =0; i<10; i++){
        cout<<"Number "<<i<<":"<<(int)array[i]<<"!"<<endl;
    }
}

Bitmap::~Bitmap(){
    //delete[] array;
}