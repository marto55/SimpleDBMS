#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#include "database.h"
#include "column.h"

using std::cout;
using std::endl;
using std::ios;
using std::vector;
using std::to_string;

void Database::insert_into(){
    // keep the index of the next character to read of the query
    // 12 is the index after "CreateTable "
    unsigned i = 12;

    // find and save the name ot the table
    string name;
    for(; i < query.size(); ++i) {
        if(name.size()>=DATABASE_STRING_SIZE){
            cout<<"table has too long name"<<endl;
        }
        if(query[i]==' '){
            break;
        }
        name += query[i];
    }
    // check the validity of the command
    if(query[i+1] != '(' || name.size()<=0){
        cout<<"Wrong command"<<endl;
        return;
    }
    // line of code to make i skip the " (" symbols
    ++i;
    ++i;

    vector<string> column_names;
    column_names.push_back("");
    for(int j=0; j < MAX_NUMBER_OF_COLUMNS; ++j) {
        for(; i < query.size(); ++i){
            if(column_names[j].size()>=DATABASE_STRING_SIZE){
                cout<<"column has too long name"<<endl;
            }
            if((query[i]==',' && query[i+1]==' ') || query[i]==')'){
                break;
            }
            column_names[j] += query[i];
        }
        if(query[i]==')'){
            break;
        }else{
            ++i;
            ++i;
        }
        column_names.push_back("");
    }
    if(column_names[0].size() <=0){
        cout<<"no columns given"<<endl;
        return;
    }

    vector<string> values;
    values.push_back("");
    if(query[i]==')' && query[i+1]==' ' && query[i+2]=='V' && query[i+3]=='A' && query[i+4]=='L' && 
    query[i+5]=='U' && query[i+6]=='E' && query[i+7]=='S' && query[i+8]==' ' && query[i+9]=='('){

        i+=10;
        for(int j=0; j < MAX_NUMBER_OF_COLUMNS; ++j) {
            for(; i < query.size(); ++i){
                if(values[j].size()>=DATABASE_STRING_SIZE){
                    cout<<"column has too long name"<<endl;
                }
                if((query[i]==',' && query[i+1]==' ') || query[i]==')'){
                    break;
                }
                if(query[i]=='"'){
                    continue;
                }
                values[j] += query[i];
            }
            if(query[i]==')'){
                break;
            }else{
                ++i;
                ++i;
            }
            values.push_back("");
        }

        if(values[0].size() <=0){
            cout<<"no values given"<<endl;
            return;
        }

        if(column_names.size() != values.size()){
            cout<<"number of columns given does not match number of values"<<endl;
            return;
        }
    }else{
        cout<<"Wrong command"<<endl;
        return;
    }

    bool found_table = false;
    vector<Column> columns;
    char next_chunk_number;
    file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
    // check if the file is open
    if (!file.is_open()){
        // signal to the user if the file coudn't be opened
        cout << "cannot open file" << DATA_FILE_NAME << endl;
    }else{
        // loop that goes through the bitmap
        // 8 is the number of bits in one byte
        for(int i=0; i<MEMORY_MAP_SIZE*8; ++i){
            if(tables_map.test(i)){
                // if there is a table description in this chunk read the name (the first 20 bytes of the chunk) and write it in 'buffer'
                char buffer[DATABASE_STRING_SIZE];
                file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE, ios::beg);
                file.read((char*) buffer, DATABASE_STRING_SIZE);

                // if names match read all columns frpm the table description
                if (name == string(buffer)){
                    found_table = true;

                    // go through the columns of the table
                    for(int j=0; j<MAX_NUMBER_OF_COLUMNS; ++j){

                        string column_name;
                        char type;
                        string column_default;

                        char buffer2[COLUMN_SIZE];
                        file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE + DATABASE_STRING_SIZE + j*COLUMN_SIZE, ios::beg);
                        file.read((char*) buffer2, COLUMN_SIZE);

                        if(buffer2[0]==0){
                            break;
                        }

                        for(int k=0; k<COLUMN_SIZE; ++k){
                            if(buffer2[k]==0){
                                if(k<DATABASE_STRING_SIZE){
                                    k=DATABASE_STRING_SIZE;
                                }else{
                                    break;
                                }
                                
                                if(buffer2[k]=='i'){
                                    type = 'i';
                                    if(buffer[k+5]=='0'){
                                        column_default += '\0';
                                        column_default += '\0';
                                        column_default += '\0';
                                        column_default += '\0';
                                        column_default += '0';
                                        break;
                                    }else{
                                        column_default += buffer[k+1];
                                        column_default += buffer[k+2];
                                        column_default += buffer[k+3];
                                        column_default += buffer[k+4];
                                        break;
                                    }
                                }
                                if(buffer2[k]=='s'){
                                    type = 's';
                                }
                                if(buffer2[k]=='d'){
                                    type = 'd';
                                }

                                if(buffer2[k+1]==0){
                                    break;
                                }else{
                                    ++k;
                                }
                            }
                            if(k<20){
                                column_name += buffer2[k];
                            }
                            if(k>=21){
                                column_default += buffer2[k];
                            }
                        }

                        Column c;
                        c.init(column_name, type, column_default);
                        columns.push_back(c);
                    }
                    file.seekg(2*MEMORY_MAP_SIZE + (i+1)*CHUNK_SIZE - 1, ios::beg);
                    char buffer3[1];
                    file.read((char*) buffer3, 1);
                    next_chunk_number = buffer3[0];
                    break;
                }
            }
        }
    }
    
    file.close();

    if(!found_table){
        cout<<"no table with such name"<<endl;
        return;
    }

    int size_of_record = 0;
    for(unsigned j=0; j<columns.size(); ++j){
        if(columns[j].get_type()=='i'){
            size_of_record += INT_SIZE;
        }
        if(columns[j].get_type()=='s'){
            size_of_record += DATABASE_STRING_SIZE;
        }
        if(columns[j].get_type()=='d'){
            size_of_record += DATE_SIZE;
        }
    }

    int record_index = 0;
    //char* record = new char[size_of_record];
    char record[100];
    memset(record, 0, size_of_record);
    // loop that goes through all columns of the table in the file
    for(unsigned j=0; j<columns.size(); ++j){
        bool this_column_is_ready = false;
        
        // loop that goes through all columns of the querry to see if any of them match with the column of the table
        for(unsigned k=0; k<column_names.size(); ++k){
            if(columns[j].get_name() == column_names[k]){
                int size = 0;
                if(columns[j].get_type()=='i'){
                    if(!check_default_int(values[k])){
                        cout<<"wrong value for column "<<columns[j].get_name()<<endl;
                        delete[] record;
                        return;
                    }

                    int num = stoi(values[k]);
                    char byte_array[4];
                    byte_array[0] = (char)(num >> 24);
                    byte_array[1] = (char)(num >> 16);
                    byte_array[2] = (char)(num >> 8);
                    byte_array[3] = (char)num;
                    values[k] = "";
                    values[k] += byte_array[0];
                    values[k] += byte_array[1];
                    values[k] += byte_array[2];
                    values[k] += byte_array[3];

                    size = INT_SIZE;
                }
                if(columns[j].get_type()=='s'){
                    values[k].resize(DATABASE_STRING_SIZE, 0);
                    size = DATABASE_STRING_SIZE;
                }
                if(columns[j].get_type()=='d'){
                    if(!check_date(values[k])){
                        cout<<"wrong date format"<<endl;
                        delete[] record;
                        return;
                    }
                    string new_value;
                    new_value +=values[k][0];
                    new_value +=values[k][1];
                    new_value +=values[k][3];
                    new_value +=values[k][4];
                    new_value +=values[k][6];
                    new_value +=values[k][7];
                    new_value +=values[k][8];
                    new_value +=values[k][9];
                    values[k] = new_value;
                
                    size = DATE_SIZE;
                }

                // loop that saves value in buffer
                for(int l=0; l<size && record_index<size_of_record; ++l, ++record_index){
                    record[record_index] = values[k][l];
                    cout<<values[k][l];
                }
                cout<<endl;
                this_column_is_ready = true;
            }else{
                if(columns[j].get_column_default()){
                    
                }
            }
        }
        if(!this_column_is_ready){
            cout<<"no value for column "<<columns[j].get_name()<<endl;
            delete[] record;
            return;
        }
    }
    if(next_chunk_number == 0){
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

        // next cunk = first empty chunk 

        // write i*SIZE CHUNK - 1 next chunk
    }else{
        // go to next chunk

        // find empty slot
    }
    
    delete[] record;
}