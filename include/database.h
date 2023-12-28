#ifndef DBA_H
#define DBA_H

#include <string>

using std::string;

class Database{
private:
    string query;
public:
	void read_input();
};

#endif // DBA_H