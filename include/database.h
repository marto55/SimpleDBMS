#ifndef DBA_H
#define DBA_H

#include <string>

using std::string;

class Database{
private:
    string query;
public:
	void read_input();
	void create_table();
	void list_tables();
	void drop_table();
	void table_info();
};

#endif // DBA_H