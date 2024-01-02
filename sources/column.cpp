#include "column.h"

// NOT YET TESTED
void Column::init(string name, char type, string column_default){
    sprintf(this->name, "%s", name.c_str());
    this->type = type;
    sprintf(this->column_default, "%s", column_default.c_str());
}