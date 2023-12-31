#include "table.h"

// NOT YET TESTED
void Table::init(string name, Column columns[MAX_NUMBER_OF_COLUMNS]){
    sprintf(this->name, "%s", name.c_str());
    for(int i=0; i<MAX_NUMBER_OF_COLUMNS; i++){
        // somehow save columns[] in this->columns
        // put this here to prevent a warning, but its useless
        columns+=0;
    }
}