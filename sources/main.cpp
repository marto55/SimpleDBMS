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
    char age[25];
    char first[25];
    char last[25];
    char phone[25];
};

class BinaryData {
private:
    char age[25];
    char first[25];
    char last[25];
    char phone[25];
public:
    BinaryData(){}
    ~BinaryData(){}

    void SetData(int age, string strFirst, string strLast, string phone){
        sprintf(this->age, "%d", age);
        sprintf(this->first, "%s", strFirst.c_str());
        sprintf(this->last, "%s", strLast.c_str());
        sprintf(this->phone, "%s", phone.c_str());
    }
    void Save(ofstream &of){
        of.write( (char*) &age, sizeof(age));
        of.write( (char*) &first, sizeof(first));
        of.write( (char*) &last, sizeof(last));
        of.write( (char*) &phone, sizeof(phone));
    }

    void WriteBinaryFile(string strFile){
        ofstream fs;
        fs.open(strFile, ios::out | ios::in | ios::binary);
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
            binaryFile.read( (char*) p.age, sizeof(p.age));
            binaryFile.read( (char*) p.first, sizeof(p.first));
            binaryFile.read( (char*) p.last, sizeof(p.last));
            binaryFile.read( (char*) p.phone, sizeof(p.phone));
            cout << p.age << "\t" << p.first << "\t" << p.last << "\t" << p.phone << endl;
        }
        binaryFile.close();
    }
};

int main()
{
    // Bitmap memory_chunks_map;
    // Bitmap tables_map;
    // Database db;
    // db.read_input();


    string strFirst, strLast, strPhone;
    int age;
    unique_ptr<BinaryData> bd (new BinaryData());
    cout << "enter age:";
    cin >> age;
    cout << "enter first name:";
    cin >> strFirst;
    cout << "enter last name:";
    cin >> strLast;
    cout << "enter phone number:";
    cin >> strPhone;
    bd->SetData(age, strFirst, strLast, strPhone);
    bd->WriteBinaryFile("data.dat");
    bd->ReadBinaryFile("data.dat");

    return 0;
}