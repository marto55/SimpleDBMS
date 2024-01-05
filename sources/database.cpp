#include "database.h"
#include "table.h"
#include "hash_functions.h"

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

Database::Database(){

    // test to see if the file data.dat exists
    fstream test_file(DATA_FILE_NAME);
    if (!test_file.is_open()) {
        // if the file doesnt exist : try to create it and fill the first 20 bytes with zeroes since the bitmaps are empty
        file.open(DATA_FILE_NAME, ios::out | ios::binary);
        if (!file.is_open()){
            // signal to the user if the file coudn't be created
            cout << "cannot create file" << DATA_FILE_NAME << endl;
        }else{
            // fill the first 20 bytes ( the bitmaps ) with zeros
            char zeros[MEMORY_MAP_SIZE * 2] = {0};
            file.write(zeros, MEMORY_MAP_SIZE * 2);
            // init the memory maps in the program with 0s
            memory_chunks_map.init(zeros, 0);
            tables_map.init(zeros, MEMORY_MAP_SIZE);
        }
        file.close();
    }else{
        // if the file exist just read the first 20 bytes and allocate the bitmaps in the program
        file.open(DATA_FILE_NAME, ios::in | ios::binary);
        
        if (!file.is_open()){
            // signal to the user if the file coudn't be opened
            cout << "cannot open file" << DATA_FILE_NAME << endl;
        }else{
            // read first 20 bytes of the file and copy the bytes into the maps
            char buffer[MEMORY_MAP_SIZE];
            file.read( (char*) buffer, MEMORY_MAP_SIZE);
            memory_chunks_map.init(buffer, 0);

            char buffer2[MEMORY_MAP_SIZE];
            file.read( (char*) buffer2, MEMORY_MAP_SIZE);
            tables_map.init(buffer, MEMORY_MAP_SIZE);
        }
        file.close();
    }
}

void Database::read_input(){
    while(true){

        // get input from user preferably in the form of valid query
        getline (cin, query);
        string command;

        // separate and save the first word of the input query into the command variable
        for(unsigned i = 0; i < query.size() && i<sizeof("CreateTable"); ++i) {
            if(query[i]==' '){
                break;
            }
            command += query[i];
        }

        // comapring the input command variable to the valid options and executing the apropriate function
        if(command == "CreateTable"){
            create_table();
            continue;
        }
        if(command == "ListTables"){
            list_tables();
            continue;
        }
        if(command == "DropTable"){
            drop_table();
            continue;
        }
        if(command == "TableInfo"){
            table_info();
            continue;
        }

        // message to the user if there was no match for his input
        cout<<"Wrong command !"<<endl;
    }
}

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

void Database::drop_table(){

    // keep the index of the next character to read of the query
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
                    char empty_chunk[CHUNK_SIZE];
                    memset(empty_chunk, 0, CHUNK_SIZE);
                    // write the chunk into the file
                    file.seekp(MEMORY_MAP_SIZE*2 + i*CHUNK_SIZE, ios::beg);
                    file.write( (char*) &empty_chunk, sizeof(empty_chunk));

                    memory_chunks_map.reset(i);
                    tables_map.reset(i);
                }
            }
        }
        if(!found_table){
            cout<<"no table with such name"<<endl;
        }
    }
    file.close();

}

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
                                }
                                if(buffer[k]=='s'){
                                    table_info += string("string");
                                }
                                if(buffer[k]=='d'){
                                    table_info += string("date");
                                }

                                if(buffer[k+1]==0){
                                    break;
                                }else{
                                    ++k;
                                    table_info += string("\t|\tdefault\t:\t");
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