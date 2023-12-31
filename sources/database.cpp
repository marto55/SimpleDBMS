#include "database.h"
#include "table.h"
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
            char zeros[MEMORY_MAP_SIZE_IN_BYTES * 2] = {0};
            file.write(zeros, MEMORY_MAP_SIZE_IN_BYTES * 2);
            // init the memory maps in the program with 0s
            memory_chunks_map.init(zeros, 0);
            tables_map.init(zeros, MEMORY_MAP_SIZE_IN_BYTES);
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
            char buffer[MEMORY_MAP_SIZE_IN_BYTES];
            file.read( (char*) buffer, MEMORY_MAP_SIZE_IN_BYTES);
            memory_chunks_map.init(buffer, 0);

            char buffer2[MEMORY_MAP_SIZE_IN_BYTES];
            file.read( (char*) buffer2, MEMORY_MAP_SIZE_IN_BYTES);
            tables_map.init(buffer, MEMORY_MAP_SIZE_IN_BYTES);
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

void Database::create_table(){

    // keep the number of the next character to read of the query
    // 12 is the index after "CreateTable "
    unsigned i = 12;

    // find and save the name ot the table
    string name;
    for(; i < query.size(); ++i) {
        if(name.size()>=DATABASE_STRING_SIZE){
            cout<<"table has too long name"<<endl;
        }
        if(query[i]=='('){
            break;
        }
        name += query[i];
    }
    // check the validity of the command
    if(query[i] != '(' || name.size()<=0){
        cout<<"Wrong command"<<endl;
        return;
    }
    // line of code to make i skip the "(" symbol
    ++i;

    // check if the brackets are empty
    if(query[i] == ')'){
        cout<<"Wrong command"<<endl;
        return;
    }
    // determine and save the comumns' names and types
    //Column columns[10];
    for(int j=0; i < query.size() && query[i]!=')'; ++j){
        string column_name;
        string column_type;

        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"column has too long name"<<endl;
            }
            if(query[i]==':'){
                break;
            }
            column_name += query[i];
        }
        // check the validity of the command
        if(query[i] != ':' || column_name.size()<=0){
            cout<<"Wrong command"<<endl;
            return;
        }
        // line of code to make i skip the ":" symbol
        ++i;

        for(; i < query.size() && query[i]!=')'; ++i) {
            if(query[i]==',' && query[i+1]==' '){
                break;
            }
            column_type += query[i];
        }

        // check the validity of the command
        if( (query[i] != ',' && query[i] != ')') || column_type.size()<=0){
            cout<<"Wrong command"<<endl;
            return;
        }
        // line of code to make i skip the ":" symbol
        ++i;
        ++i;

        //cout<<"Column name: "<<column_name<<endl<<"Column type: "<<column_type<<endl;
        //columns[0].init();

        // if(column_type == "int"){
        //     columns[j].init(column_name, 'i');
        // }else{
        //     if(column_type == "string"){
        //         columns[j].init(column_name, 's');
        //     }else{
        //         if(column_type == "date"){
        //             columns[j].init(column_name, 'd');
        //         }else{
        //             cout<<"wrong data type for column: "<<column_name<<endl;
        //         }
        //     }
        // }
    }

    // find the first empty chunk of memory
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
    // flag the chunk as used in the maps
    memory_chunks_map.set(first_empty_chunk);
    tables_map.set(first_empty_chunk);


}

void Database::list_tables(){

}

void Database::drop_table(){

}

void Database::table_info(){

}