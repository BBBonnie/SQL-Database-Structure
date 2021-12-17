#ifndef SQL_H
#define SQL_H
#include <iostream>
#include"tokenizer.h"
#include"table.h"
#include"../../BTree/BTree_class/map.h"


using namespace std;
class SQL
{
public:



    SQL();
    SQL(string& input);
    SQL(string& input, fstream& f);

    Table& operator[](const string& key){
        return _table[key];
    }

    void set_string(string input);

    void run();

    friend ostream& operator <<(ostream& outs, SQL& print_me){
        outs << print_me._table;
        return outs;
    }

    void print(){
        cout<<_table;
    }


private:
    Map<string, Table> _table;
    MMap<string, string> _ptree;
    vector<string> _command;
};



#endif // SQL_H
