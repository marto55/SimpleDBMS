#include <iostream>
#include <fstream>
#include <vector>

#include "database.h"

using std::cout;
using std::endl;
using std::ios;
using std::vector;

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

        if(column_names.size() != values.size()){
            cout<<"number of columns given does not match number of values"<<endl;
            return;
        }
    }else{
        cout<<"Wrong command"<<endl;
        return;
    }


    // for(unsigned j=0; j<column_names.size(); ++j){
    //     cout<<column_names[j]<<endl;
    // }
    // cout<<endl;
    // for(unsigned j=0; j<values.size(); ++j){
    //     cout<<values[j]<<endl;
    // }
    
}