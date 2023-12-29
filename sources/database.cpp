#include "database.h"
#include "hash_functions.h"

#include <iostream>
#include <string>

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
            char zeros[20] = {0};
            file.write(zeros, sizeof(zeros));
        }
        file.close();
    }else{
        // if the file exist just read the first 20 bytes and allocate the bitmaps in the program
        file.open(DATA_FILE_NAME, ios::in | ios::binary);
        
        if (!file.is_open()){
            // signal to the user if the file coudn't be opened
            cout << "cannot open file" << DATA_FILE_NAME << endl;
        }else{
            char buffer[MEMORY_MAP_SIZE_IN_BYTES];
            file.read( (char*) buffer, sizeof(buffer));
            memory_chunks_map.init(buffer, 0);
            file.read( (char*) buffer, sizeof(buffer));
            tables_map.init(buffer, MEMORY_MAP_SIZE_IN_BYTES);
        }
        file.close();

        // int bit_number;
        // cout<<"which bit to set: "; cin>>bit_number;
        // memory_chunks_map.set(bit_number);

        // cout<<"which bit to reset: "; cin>>bit_number;
        // memory_chunks_map.reset(bit_number);

        // cout<<"which bit to flip: "; cin>>bit_number;
        // memory_chunks_map.flip(bit_number);

        // memory_chunks_map.print_bytes_as_ints();
    }

    

    // file.open(DATA_FILE_NAME, ios::out | ios::binary);
    // if (!file.is_open()){
    //     cout << "cannot open file" << DATA_FILE_NAME << endl;
    // }
    // file.close();

    // memory_chunks_map.init(MEMORY_MAP_SIZE_IN_BYTES);
    // tables_map.init(MEMORY_MAP_SIZE_IN_BYTES);
}

void Database::read_input(){
    while(true){

        // get input from user preferably in the form of valid query
        // string command is 11 symbols since no valid command is longer than 11 symbols
        getline (cin, query);
        string command = "-----------";

        // separate and save the first word of the input in the command variable
        for(unsigned i = 0; i < query.size(); ++i) {
            if(query[i]==' '){
                break;
            }
            command[i] = query[i];
        }

        // helper constants to compare to the first word of the query
        const string CreateTable = "CreateTable";
        const string ListTables  = "ListTables-";
        const string DropTable   = "DropTable--";
        const string TableInfo   = "TableInfo--";
        
        unsigned hash_of_command = hash_string(command);

        // comparing the hash codes of the constants to the hash code of the command
        // to determine the command and executing the apropriate function
        if(hash_of_command == hash_string(CreateTable)){
            create_table();
            continue;
        }
        if(hash_of_command == hash_string(ListTables)){
            list_tables();
            continue;
        }
        if(hash_of_command == hash_string(DropTable)){
            drop_table();
            continue;
        }
        if(hash_of_command == hash_string(TableInfo)){
            table_info();
            continue;
        }

        // message to the user if there was no match for his input
        cout<<"Wrong command !"<<endl;
    }
}

void Database::create_table(){

    string name;
    for(unsigned i = 11; i < query.size(); ++i) {
        if(query[i]==' '){
            break;
        }
        name[i] = query[i];
    }

    int first_empty_chunk = 0;
    while(true){
        if(!memory_chunks_map.test(first_empty_chunk)){
            break;
        }
        first_empty_chunk++;
        if(first_empty_chunk>80){
            cout<< "no more free chunks of memory"<<endl;
            return;
        }
    }
    memory_chunks_map.set(first_empty_chunk);
    tables_map.set(first_empty_chunk);

    cout<<first_empty_chunk<<endl;

    // ofstream fs;
    // fs.open(DATA_FILE_NAME, ios::out | ios::binary | ios::app);
    // if (!fs.is_open()){
    //     cout << "cannot open file" << DATA_FILE_NAME << endl;
    // }
    // else{
    //     //this->Save(fs);
    // }
    // fs.close();
}

void Database::list_tables(){

}

void Database::drop_table(){

}

void Database::table_info(){

}