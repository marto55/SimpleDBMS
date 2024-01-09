#include <iostream>
#include <fstream>
#include <vector>

#include "database.h"
#include "column.h"

using std::cout;
using std::endl;
using std::ios;
using std::vector;
using std::to_string;

void Database::select(){
    unsigned i=7;

    if(query[i] == '*'){
        ++i;

        if(query[i]==' ' && query[i+1]=='F' && query[i+2]=='R' && 
            query[i+3]=='O' && query[i+4]=='M' && query[i+5]==' '){
            i+=6;
        }else{
            cout<<"Wrong command"<<endl;
            return;
        }
        // find and save the name ot the table
        string name;
        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"table has too long name"<<endl;
            }
            name += query[i];
        }
        // check the validity of the command
        if(name.size()<=0){
            cout<<"no table given"<<endl;
            return;
        }

        file.open(DATA_FILE_NAME, ios::in | ios::out | ios::binary);
        // check if the file is open
        if (!file.is_open()){
            // signal to the user if the file coudn't be opened
            cout << "cannot open file" << DATA_FILE_NAME << endl;
        }else{

            vector<Column> columns;
            int next_chunk_index = 0;
            int size_of_record = 0;
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

                        char buffer3[INT_SIZE];
                        file.seekg(2*MEMORY_MAP_SIZE + (i+1)*CHUNK_SIZE - INT_SIZE, ios::beg);
                        file.read((char*) buffer3, INT_SIZE);
                        // convert bytes into integer
                        for(int k = 0; k < 4; ++k) {
                            next_chunk_index = (next_chunk_index << 8) + (buffer3[k] & 0xff);
                        }

                        // go through the columns of the table
                        for(int j=0; j<MAX_NUMBER_OF_COLUMNS; ++j){

                            string column_name;
                            char type;

                            char buffer2[COLUMN_SIZE];
                            file.seekg(2*MEMORY_MAP_SIZE + i*CHUNK_SIZE + DATABASE_STRING_SIZE + j*COLUMN_SIZE, ios::beg);
                            file.read((char*) buffer2, COLUMN_SIZE);

                            if(buffer2[0]==0){
                                break;
                            }

                            for(int k=0; k<=DATABASE_STRING_SIZE; ++k){
                                if(buffer2[k]==0){
                                    break;
                                }
                                column_name += buffer2[k];
                            }

                            if(buffer2[DATABASE_STRING_SIZE]=='i'){
                                type = 'i';
                                size_of_record += INT_SIZE;
                            }
                            if(buffer2[DATABASE_STRING_SIZE]=='s'){
                                type = 's';
                                size_of_record += DATABASE_STRING_SIZE;
                            }
                            if(buffer2[DATABASE_STRING_SIZE]=='d'){
                                type = 'd';
                                size_of_record +=DATE_SIZE;
                            }

                            Column c;
                            c.init(column_name, type, "");
                            columns.push_back(c);
                        }
                        break;
                    }
                }
            }

            int table_next_chunk_index = next_chunk_index;

            char next_buffer[INT_SIZE];
            char memory_chunk[CHUNK_SIZE];
            vector<unsigned> max_record_print_sizes;
            for(unsigned j=0; j<columns.size(); ++j){
                string temp = columns[j].get_name();
                max_record_print_sizes.push_back(temp.size());
            }
            // go through chunks to determine max record width in console
            while(next_chunk_index > 0){
                
                // read chunk
                file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE, ios::beg);
                file.read( (char*) &memory_chunk, CHUNK_SIZE);

                int inside_record_index = 0;
                // go through columns
                for(unsigned k=0; k<columns.size(); ++k){
                    
                    if(columns[k].get_type()=='i'){
                        // go through records
                        for(int j=0; j<(CHUNK_SIZE-4)/size_of_record; ++j){
                            char buffer[INT_SIZE];
                            file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE + j*size_of_record + inside_record_index, ios::beg);
                            file.read( (char*) &buffer, INT_SIZE);

                            int value =0;
                            for(int k = 0; k < 4; ++k) {
                                value = (value << 8) + (buffer[k] & 0xff);
                            }
                            string string_value = to_string(value);
                            if(max_record_print_sizes[k] < string_value.size()){
                                max_record_print_sizes[k] = string_value.size();
                            }
                        }
                        inside_record_index += INT_SIZE;
                    }
                    if(columns[k].get_type()=='s'){

                        // go through records
                        for(int j=0; j<(CHUNK_SIZE-4)/size_of_record; ++j){
                            char buffer[INT_SIZE];
                            file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE + j*size_of_record + inside_record_index, ios::beg);
                            file.read( (char*) &buffer, DATABASE_STRING_SIZE);

                            string string_value = buffer;
                            if(string_value.size() == 0){
                                break;
                            }
                            if(max_record_print_sizes[k] < string_value.size()){
                                max_record_print_sizes[k] = string_value.size();
                            }
                        }
                        inside_record_index += DATABASE_STRING_SIZE;
                    }
                    if(columns[k].get_type()=='d'){
                        max_record_print_sizes[k] = DATE_SIZE+2;
                    }
                }

                file.seekg(2*MEMORY_MAP_SIZE + next_chunk_index*CHUNK_SIZE + CHUNK_SIZE-INT_SIZE, ios::beg);
                file.read((char *) next_buffer, INT_SIZE);
                // convert bytes into integer
                for(int k = 0; k < 4; ++k) {
                    next_chunk_index = (next_chunk_index << 8) + (next_buffer[k] & 0xff);
                }
            }

            string result;

            for(unsigned j=0; j<max_record_print_sizes.size(); ++j){
                for(unsigned k=0; k<max_record_print_sizes[j]; ++k){
                    result += '-';
                }
                result+='-';
            }
            result += "-";
            result+='\n';
            result+='|';
            for(unsigned j=0; j<columns.size(); ++j){
                result+= columns[j].get_name();
                string temp = columns[j].get_name();
                for(unsigned k=0; k<max_record_print_sizes[j] - temp.size(); ++k){
                    result += ' ';
                }
                result+='|';
            }
            result+='\n';
            for(unsigned j=0; j<max_record_print_sizes.size(); ++j){
                for(unsigned k=0; k<max_record_print_sizes[j]; ++k){
                    result += '-';
                }
                result+='-';
            }
            result += "-";
            result+='\n';

            next_chunk_index = table_next_chunk_index;
            // go through chunks to determine max record width in console
            while(next_chunk_index > 0){
                
                // read chunk
                file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE, ios::beg);
                file.read( (char*) &memory_chunk, CHUNK_SIZE);
                
                // go through records
                for(int j=0; j<(CHUNK_SIZE-4)/size_of_record; ++j){
                    result += '|';
                    int inside_record_index = 0;
                    // go through columns
                    for(unsigned k=0; k<columns.size(); ++k){
                        if(columns[k].get_type()=='i'){

                            char buffer[INT_SIZE];
                            file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE + j*size_of_record + inside_record_index, ios::beg);
                            file.read( (char*) &buffer, INT_SIZE);

                            inside_record_index += INT_SIZE;

                            int value = 0;
                            for(int k = 0; k < 4; ++k) {
                                value = (value << 8) + (buffer[k] & 0xff);
                            }
                            string string_value = to_string(value);
                            result += string_value;
                            for(unsigned l=0; l<max_record_print_sizes[k] - string_value.size(); ++l){
                                result += ' ';
                            }
                            result+='|';

                        }
                        if(columns[k].get_type()=='s'){

                            char buffer[DATABASE_STRING_SIZE];
                            file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE + j*size_of_record + inside_record_index, ios::beg);
                            file.read( (char*) &buffer, DATABASE_STRING_SIZE);

                            inside_record_index += DATABASE_STRING_SIZE;

                            string string_value = (string)buffer;
                            result += string_value;
                            for(unsigned l=0; l<max_record_print_sizes[k] - string_value.size(); ++l){
                                result += ' ';
                            }
                            result+='|';
                        }
                        if(columns[k].get_type()=='d'){

                            char buffer[DATE_SIZE];
                            file.seekg(MEMORY_MAP_SIZE*2 + next_chunk_index*CHUNK_SIZE + j*size_of_record + inside_record_index, ios::beg);
                            file.read( (char*) &buffer, DATE_SIZE);

                            inside_record_index += DATE_SIZE;
                            result += buffer[0];
                            result += buffer[1];
                            result += '.';
                            result += buffer[2];
                            result += buffer[3];
                            result += '.';
                            result += buffer[4];
                            result += buffer[5];
                            result += buffer[6];
                            result += buffer[7];
                            result+='|';
                        }
                    }
                    result += '\n';
                }

                file.seekg(2*MEMORY_MAP_SIZE + next_chunk_index*CHUNK_SIZE + CHUNK_SIZE-INT_SIZE, ios::beg);
                file.read((char *) next_buffer, INT_SIZE);
                // convert bytes into integer
                for(int k = 0; k < 4; ++k) {
                    next_chunk_index = (next_chunk_index << 8) + (next_buffer[k] & 0xff);
                }
            }
            for(unsigned j=0; j<max_record_print_sizes.size(); ++j){
                for(unsigned k=0; k<max_record_print_sizes[j]; ++k){
                    result += '-';
                }
                result+='-';
            }
            result += "-";
            result+='\n';

            cout<<result;
        }
        file.close();
        
    }else{

        vector<string> column_names;
        column_names.push_back("");
        for(int j=0; j < MAX_NUMBER_OF_COLUMNS; ++j) {
            for(; i < query.size(); ++i){
                if(column_names[j].size()>=DATABASE_STRING_SIZE){
                    cout<<"column has too long name"<<endl;
                }
                if(query[i]==',' && query[i+1]==' '){
                    break;
                }
                if(query[i]==' ' && query[i+1]=='F' && query[i+2]=='R' && 
                query[i+3]=='O' && query[i+4]=='M' && query[i+5]==' '){
                    break;
                }
                column_names[j] += query[i];
            }
            if(query[i]==' ' && query[i+1]=='F' && query[i+2]=='R' && 
                query[i+3]=='O' && query[i+4]=='M' && query[i+5]==' '){
                i+=6;
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

        // find and save the name ot the table
        string name;
        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"table has too long name"<<endl;
            }
            if(query[i]==' ' && query[i+1]=='W' && query[i+2]=='H' && 
                query[i+3]=='E' && query[i+4]=='R' && query[i+5]=='E' && query[i+6]==' '){
                    i+=7;
                    break;
                }
            name += query[i];
        }
        // check the validity of the command
        if(name.size()<=0){
            cout<<"no table given"<<endl;
            return;
        }

        // find and save the name ot the the column used 
        string column;
        for(; i < query.size(); ++i) {
            if(name.size()>=DATABASE_STRING_SIZE){
                cout<<"table has too long name"<<endl;
            }
            if(query[i]==' ' && query[i+1]=='W' && query[i+2]=='H' && 
                query[i+3]=='E' && query[i+4]=='R' && query[i+5]=='E' && query[i+6]==' '){
                    i+=7;
                    break;
                }
            name += query[i];
        }
        // check the validity of the command
        if(name.size()<=0){
            cout<<"no table given"<<endl;
            return;
        }
    }    
}