#include "column.h"

// NOT YET TESTED
void Column::init(string name, char type){
    sprintf(this->name, "%s", name.c_str());
    this->type = type;
}