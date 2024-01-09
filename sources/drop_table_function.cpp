#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "database.h"

using std::cout;
using std::endl;
using std::ios;

void Database::drop_table(){

    // keep the index of the next character to read from the query
    // 10 is the index after "DropTable "
    unsigned i = 10;

    // find and save the name ot the table
    string name;
    for(; i < query.size(); ++i) {
        if(name.size()>=DATABASE_STRING_SIZE){
            cout<<"table has too long name"<<endl;
        }
        name += query[i];
    }
    // message if no name was given after DropTable
    if(name.size()<=0){
        cout<<"DropTable requires name of table"<<endl;
        return;
    }

    file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
    
    // check if the file is open
    if (!file.is_open()){
        // signal to the user if the file coudn't be opened
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{

        bool found_table = false;

        // if its open then read all the tables' names and write in the string table_names
        // loop that goes through the bitmap
        // 8 is the number of bits in one byte
        for(int i=0; i<MEMORY_MAP_SIZE*8; ++i){
            if(tables_map.test(i)){

                // if there is a table description in this chunk read the name (the first 20 bytes of the chunk) and write it in 'buffer'
                char buffer[DATABASE_STRING_SIZE];
                file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE, ios::beg);
                file.read((char*) buffer, DATABASE_STRING_SIZE);

                // if names match delete current memory chunk (empty it / fill with '\0')
                if (name == string(buffer)){
                    found_table = true;

                    int next_chunk_index;
                    char buffer2[INT_SIZE];
                    file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE + CHUNK_SIZE-INT_SIZE, ios::beg);
                    file.read((char *) buffer2, INT_SIZE);
                    // convert bytes into integer
                    for(int k = 0; k < 4; ++k) {
                        next_chunk_index = (next_chunk_index << 8) + (buffer2[k] & 0xff);
                    }

                    char empty_chunk[CHUNK_SIZE];
                    memset(empty_chunk, 0, CHUNK_SIZE);
                    // write the chunk into the file
                    file.seekp(MEMORY_MAP_SIZE*2 + i*CHUNK_SIZE, ios::beg);
                    file.write( (char*) &empty_chunk, sizeof(empty_chunk));

                    memory_chunks_map.reset(i);
                    tables_map.reset(i);

                    while(next_chunk_index > 0){
                        file.seekg(2*MEMORY_MAP_SIZE + next_chunk_index*CHUNK_SIZE + CHUNK_SIZE-INT_SIZE, ios::beg);
                        file.read((char *) buffer2, INT_SIZE);

                        file.seekp(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE, ios::beg);
                        file.write( (char*) &empty_chunk, CHUNK_SIZE);

                        memory_chunks_map.reset(next_chunk_index);

                        // convert bytes into integer
                        for(int k = 0; k < 4; ++k) {
                            next_chunk_index = (next_chunk_index << 8) + (buffer2[k] & 0xff);
                        }
                    }
                    break;
                }
            }
        }
        if(!found_table){
            cout<<"no table with such name"<<endl;
        }
    }
    file.close();
}