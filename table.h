#ifndef TABLE_H
#define TABLE_H

#include"../../BTree/BTree_class/mmap.h"
#include"record.h"
#include"file_funcs.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include"../../stack_and_queue/stack.h"
#include"../../stack_and_queue/queue.h"
#include"table_const.h"
#include <stdlib.h>


using namespace std;

class Table{
public:

//    typedef vector<string> vectStr;
    typedef vector<MMap<string, int>> vectMmap;


    Table();
    Table(string name);
    Table(string name, vector<string> field_list);

    Table create(string name, vector<string> fields_list);
    Table select(vector<string>& fields_list);
    Table select(vector<string> &fields_list, vector<string> &conditions);

    void insert(vector<string> fields_item);

//    vector<string> get_index(int index);
    void loadFields();
    void writeFields(vector<string> fields);

    string get_db_name(){return _dbname;}



    vector<int> Intersection(vector<int> left, vector<int> right);
    vector<int> Union(vector<int> left, vector<int> right);

    friend ostream& operator << (ostream& outs,  Table& print_me){
        fstream _f;
        open_fileRW(_f, print_me._filename);
        print_me._recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
                outs<<"record";
        for(int i = 0; i < print_me._field_list.size(); i++){

            outs<<"          ";
            outs<<print_me._field_list[i];
        }
        cout<<endl;

        for(int i = 0; i < print_me._recno; i++){
            print_me._rec.read(_f, i);
            outs/*<<"recono: "<<setw(4)*/<<i<<" / "<<print_me._recno<<" "<<print_me._rec<<endl;
        }

        _f.close();
        return outs;
    }


private:
    //indexing functions
    void index_this();
    void re_index(vector<string>& entries);
    void set_precedence();
    Queue<string> shuntingYard(vector<string>& conditions);


    MMap<string, int> precedence;
    vectMmap _mmapVect;
    vector<string> _field_list;
    MMap<string, int> _field_list_map;
    char _filename[50];
    char _filename_fields[50];
    string _dbname;
    string _db_fields_list;
    int _recno = 0;
    Record _rec;

};


#endif // TABLE_H
