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
        if(command == "Insert"){
            // check if next word is 'INTO'
            command += ' ';
            for(unsigned i = command.size(); i < query.size() && i<sizeof("Insert INTO"); ++i) {
                if(query[i]==' '){
                    break;
                }
                command += query[i];
            }
            cout<<command<<endl;
            if(command == "Insert INTO"){
                insert_into();
                continue;
            }else{
                cout<<"Wrong command !"<<endl;
                continue;
            }
        }
        if(command == "Select"){
            select();
            continue;
        }
        if(command == "Delete"){
            delete_function();
            continue;
        }

        // message to the user if there was no match for his input
        cout<<"Wrong command !"<<endl;
    }
}