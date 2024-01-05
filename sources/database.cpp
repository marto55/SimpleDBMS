#include "database.h"
#include "table.h"
#include "hash_functions.h"

#include <iostream>
#include <string>
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

    // if the file exist just read the first 20 bytes and allocate the bitmaps in the program
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

    cout<<table_names;
}

void Database::drop_table(){

}

void Database::table_info(){

}