#include "column.h"

void Column::init(string name, char type, string column_default){
    name.resize(DATABASE_STRING_SIZE, 0);
    for(int k=0; k<DATABASE_STRING_SIZE; ++k){
        this->name[k]=name[k];
    }
    this->type = type;
    column_default.resize(DATABASE_STRING_SIZE, 0);
    for(int k=0; k<DATABASE_STRING_SIZE; ++k){
        this->column_default[k]=column_default[k];
    }
}

// Getter for name
char* Column::get_name() {
    return name;
}

// Getter for type
char Column::get_type() {
    return type;
}

// Getter for column_default
char* Column::get_column_default() {
    return column_default;
}