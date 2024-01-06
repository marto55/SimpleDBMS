#include <iostream>
#include <fstream>
#include <string>

#include "database.h"

using std::cout;
using std::endl;
using std::ios;
using std::to_string;

void Database::table_info(){
    // keep the index of the next character to read of the query
    // 10 is the index after "TableInfo "
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
        cout<<"TableInfo requires name of table"<<endl;
        return;
    }

    file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
    // check if the file is open
    if (!file.is_open()){
        // signal to the user if the file coudn't be opened
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{
        string table_info;
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

                    for(unsigned j=0; j<name.size() + 4; ++j){
                        table_info += "-";
                    }
                    table_info += "\n";
                    table_info += "| " + name + " |";
                    table_info += "\n";
                    for(unsigned j=0; j<name.size() + 4; ++j){
                        table_info += "-";
                    }
                    table_info += "\n";

                    for(int j=0; j<MAX_NUMBER_OF_COLUMNS; ++j){
                        char buffer[COLUMN_SIZE];
                        file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE + DATABASE_STRING_SIZE + j*COLUMN_SIZE, ios::beg);
                        file.read((char*) buffer, COLUMN_SIZE);

                        if(buffer[0]==0){
                            break;
                        }

                        for(int k=0; k<COLUMN_SIZE; ++k){
                            if(buffer[k]==0){
                                if(k<DATABASE_STRING_SIZE){
                                    k=DATABASE_STRING_SIZE;
                                }else{
                                    break;
                                }
                                
                                table_info += string("\t:\t");
                                
                                if(buffer[k]=='i'){
                                    table_info += string("int");
                                    if(buffer[k+5]=='0'){
                                        break;
                                    }else{
                                        char bytes[4] = {buffer[k+1], buffer[k+2], buffer[k+3], buffer[k+4]};
                                        int value = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
                                        table_info += string("\t|\tdefault\t:\t");
                                        table_info += to_string(value);
                                        break;
                                    }
                                }
                                if(buffer[k]=='s'){
                                    table_info += string("string");
                                    if(buffer[k+1]==0){
                                        break;
                                    }else{
                                        ++k;
                                        table_info += string("\t|\tdefault\t:\t");
                                    }
                                }
                                if(buffer[k]=='d'){
                                    table_info += string("date");
                                    if(buffer[k+1]==0){
                                        break;
                                    }else{
                                        string value;
                                        value+=buffer[k+1];
                                        value+=buffer[k+2];
                                        value+='.';
                                        value+=buffer[k+3];
                                        value+=buffer[k+4];
                                        value+='.';
                                        value+=buffer[k+5];
                                        value+=buffer[k+6];
                                        value+=buffer[k+7];
                                        value+=buffer[k+8];

                                        table_info += string("\t|\tdefault\t:\t");
                                        table_info += value;
                                        break;
                                    }
                                }
                            }
                            table_info += buffer[k];
                        }
                        table_info += '\n';
                    }
                    cout<<table_info;
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