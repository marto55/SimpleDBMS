#include "date.h"

void Date::init(string day, string month, string year){
    sprintf(this->day, "%s", day.c_str());
    sprintf(this->month, "%s", month.c_str());
    sprintf(this->year, "%s", year.c_str());
}

// Getter for day
const char* Date::get_day() const{
    return day;
}

// Getter for month
const char* Date::get_month() const {
    return month;
}

// Getter for year
const char* Date::get_year() const {
    return year;
}