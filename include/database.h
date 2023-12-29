#ifndef DBA_H
#define DBA_H

#include <string>
#include <fstream>
#include "bitmap.h"
#include "constants.h"

using std::string;
using std::fstream;

class Database{
private:
	fstream file;
	Bitmap memory_chunks_map;
    Bitmap tables_map;
	string query;
public:
	Database();

	void read_input();
	void create_table();
	void list_tables();
	void drop_table();
	void table_info();
};

#endif // DBA_H