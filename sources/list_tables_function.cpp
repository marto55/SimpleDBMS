#include <iostream>
#include <fstream>
#include <string>

#include "database.h"

using std::cout;
using std::endl;
using std::ios;

void Database::list_tables(){
    // create string to store the output
    string table_names;

    // open the file so the program can extract the names of the tables
    file.open(DATA_FILE_NAME, ios::in | ios::binary);
    
    // check if the file is open
    if (!file.is_open()){
        // signal to the user if the file coudn't be opened
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{
        // if its open then read all the tables' names and write in the string table_names
        // loop that goes through the bitmap
        for(int i=0; i<MEMORY_MAP_SIZE*8; ++i){
            if(tables_map.test(i)){

                // if there is a table description in this chunk read the name (the first 20 bytes of the chunk)
                char buffer[DATABASE_STRING_SIZE];
                file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE, ios::beg);
                file.read((char*) buffer, DATABASE_STRING_SIZE);
                for(int j=0; j<DATABASE_STRING_SIZE; j++){
                    table_names += buffer[j];
                }
                // add new line to make it readable in the console
                table_names += '\n';
            }
        }
    }
    file.close();

    // print the names of the tables in the console
    cout<<table_names;
}