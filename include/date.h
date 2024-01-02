#ifndef DTE_H
#define DTE_H

#include <string>

using std::string;

class Date{
    char day[2];
	char month[2];
    char year[4];
public:
    void init(string day, string month, string year);
    const char* get_day() const;
    const char* get_month() const;
    const char* get_year() const;
};

#endif // DTE_H