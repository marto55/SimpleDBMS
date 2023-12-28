#include "database.h"
#include "hash_functions.h"

#include <iostream>
#include <string>

using namespace std;

void Database::read_input(){
    while(true){

        getline (cin, query);
        string command = "-----------";

        for(unsigned i = 0; i < query.size(); ++i) {
            if(query[i]==' '){
                break;
            }
            command[i] = query[i];
        }

        string CreateTable = "CreateTable";
        string ListTables  = "ListTables-";
        string DropTable   = "DropTable--";
        string TableInfo   = "TableInfo--";
        
        unsigned hash_of_command = hash_string(command);

        if(hash_of_command == hash_string(CreateTable)){
            cout<<"Create Table option chosen"<<endl;
            continue;
        }
        if(hash_of_command == hash_string(ListTables)){
            cout<<"List Table option chosen"<<endl;
            continue;
        }
        if(hash_of_command == hash_string(DropTable)){
            cout<<"Drop Table option chosen"<<endl;
            continue;
        }
        if(hash_of_command == hash_string(TableInfo)){
            cout<<"TableInfo option chosen"<<endl;
            continue;
        }

        cout<<"Wrong command !"<<endl;
    }
}

    

    
