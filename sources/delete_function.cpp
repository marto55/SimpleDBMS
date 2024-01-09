#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#include "database.h"
#include "column.h"

using std::cout;
using std::endl;
using std::ios;
using std::vector;
using std::to_string;

void Database::delete_function(){
    unsigned i=7;

    if(query[i] == '*'){
        ++i;

        if(query[i]==' ' && query[i+1]=='F' && query[i+2]=='R' && 
            query[i+3]=='O' && query[i+4]=='M' && query[i+5]==' '){
            i+=6;
        }else{
            cout<<"Wrong command"<<endl;
            return;
        }
        // find and save the name ot the table
        string name;
        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"table has too long name"<<endl;
            }
            name += query[i];
        }
        // check the validity of the command
        if(name.size()<=0){
            cout<<"no table given"<<endl;
            return;
        }

        file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
        // check if the file is open
        if (!file.is_open()){
            // signal to the user if the file coudn't be opened
            cout << "cannot open file" << DATA_FILE_NAME << endl;
        }else{

            bool found_table = false;
            // loop that goes through the bitmap
            // 8 is the number of bits in one byte
            for(int i=0; i<MEMORY_MAP_SIZE*8; ++i){
                if(tables_map.test(i)){
                    // if there is a table description in this chunk read the name (the first 20 bytes of the chunk) and write it in 'buffer'
                    char buffer[DATABASE_STRING_SIZE];

                    file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE, ios::beg);
                    file.read((char*) buffer, DATABASE_STRING_SIZE);

                    // if names match read all columns frpm the table description
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

                        memset(buffer2, 0, INT_SIZE);
                        file.seekp(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE + CHUNK_SIZE-INT_SIZE, ios::beg);
                        file.write((char *) buffer2, INT_SIZE);

                        char empty_chunk[CHUNK_SIZE];
                        memset(empty_chunk, 0, CHUNK_SIZE);
                        
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
    }else{

        vector<string> column_names;
        column_names.push_back("");
        for(int j=0; j < MAX_NUMBER_OF_COLUMNS; ++j) {
            for(; i < query.size(); ++i){
                if(column_names[j].size()>=DATABASE_STRING_SIZE){
                    cout<<"column has too long name"<<endl;
                }
                if(query[i]==',' && query[i+1]==' '){
                    break;
                }
                if(query[i]==' ' && query[i+1]=='F' && query[i+2]=='R' && 
                query[i+3]=='O' && query[i+4]=='M' && query[i+5]==' '){
                    break;
                }
                column_names[j] += query[i];
            }
            if(query[i]==' ' && query[i+1]=='F' && query[i+2]=='R' && 
                query[i+3]=='O' && query[i+4]=='M' && query[i+5]==' '){
                i+=6;
                break;
            }else{
                ++i;
                ++i;
            }
            column_names.push_back("");
        }
        if(column_names[0].size() <=0){
            cout<<"no columns given"<<endl;
            return;
        }

        // find and save the name ot the table
        string name;
        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"table has too long name"<<endl;
            }
            if(query[i]==' ' && query[i+1]=='W' && query[i+2]=='H' && 
                query[i+3]=='E' && query[i+4]=='R' && query[i+5]=='E' && query[i+6]==' '){
                    i+=7;
                    break;
                }
            name += query[i];
        }
        // check the validity of the command
        if(name.size()<=0){
            cout<<"no table given"<<endl;
            return;
        }

        // find and save the name ot the the column used 
        string column;
        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"table has too long name"<<endl;
            }
            if(query[i]==' ' && query[i+1]=='W' && query[i+2]=='H' && 
                query[i+3]=='E' && query[i+4]=='R' && query[i+5]=='E' && query[i+6]==' '){
                    i+=7;
                    break;
                }
            name += query[i];
        }
        // check the validity of the command
        if(name.size()<=0){
            cout<<"no table given"<<endl;
            return;
        }
    }    
}