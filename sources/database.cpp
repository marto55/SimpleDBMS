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

}

void Database::drop_table(){

}

void Database::table_info(){

}

bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool Database::check_date(string date) {

    if (date.length() != 10 || date[2] != ':' || date[5] != ':')
        return false;

    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));

    if (year < 1583 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    if (month == 2) {
        if (isLeap(year))
            return (day <= 29);
        else
            return (day <= 28);
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
        return (day <= 30);

    return true;
}

bool Database::check_default_int(const std::string& s)
{
    for(char const &c : s) {
        if(c<'0' || '9'<c)
            return false;
    }
    return true;
}