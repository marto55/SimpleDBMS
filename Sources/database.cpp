#include "database.h"
#include "hash_functions.h"

#include <iostream>
#include <string>

using namespace std;

void Database::read_input(){
    while(true){

        // get input from user preferably in the form of valid query
        // string command is 12 symbols since no valid command is longer than 12 symbols
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

}

void Database::list_tables(){

}

void Database::drop_table(){

}

void Database::table_info(){

}