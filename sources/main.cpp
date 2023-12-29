#include "database.h"
#include "table.h"
#include "column.h"
#include "bitmap.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;

struct Person{
    char name[25];
};

class BinaryData {
private:
    char name[25];
public:
    BinaryData(){}
    ~BinaryData(){}

    void SetData(string name){
        //sprintf(this->age, "%d", age);
        sprintf(this->name, "%s", name.c_str());
    }
    void Save(ofstream &of){
        of.write( (char*) &name, sizeof(name));
    }

    void WriteBinaryFile(string strFile){
        ofstream fs;
        fs.open(strFile, ios::out | ios::binary | ios::app);
        if (!fs.is_open()){
            cout << "cannot open file" << strFile << endl;
        }
        else{
            this->Save(fs);
        }
        fs.close();
    }

    void ReadBinaryFile(string strFile){
        Person p;
        ifstream binaryFile;
        int size=0;
        binaryFile.open(strFile, ios::in | ios::binary);
        binaryFile.seekg(0, ios::end);
        size = (int) binaryFile.tellg();
        binaryFile.seekg(0, ios::beg);
        while(binaryFile.tellg() < size){
            
            binaryFile.read( (char*) p.name, sizeof(p.name));
            cout << binaryFile.tellg()<< " ! "<< p.name << endl;
        }
        binaryFile.close();
    }
};

int main()
{
    
    Database db;
    db.read_input();


    // string name;
    // unique_ptr<BinaryData> bd (new BinaryData());
    // cout << "enter name:";
    // cin >> name;
    // bd->SetData(name);
    // bd->WriteBinaryFile("data.dat");
    // bd->ReadBinaryFile("data.dat");

    return 0;
}