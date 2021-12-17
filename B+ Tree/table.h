#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include<vector>
#include<string>
#include <fstream>
#include <time.h>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <algorithm>
#include <unistd.h>

#include"tokenizer.h"
#include"../../BTree/BTree_class/map.h"
#include"../../BTree/BTree_class/mmap.h"
#include"record.h"
#include"array_funcs.h"


using namespace std;
class Table
{
public:

    //USER:
    //Table t("student", {first, last, age, major});
    //t.insert_info({"Joe", "blow, "45", "math"});
    //t.insert_into()
    //t.select_all()
    //t.select(condition)


    typedef vector<string> vectStr;
    typedef vector<MMap<string, int>> vectMmap;
    typedef queue<string> qStr;

    Table();
    Table(string name);
    Table(string name, vectStr field_list);
    Table(string name, vectStr field_list, vectMmap mmapVect);

    Table create(string name, vectStr fields_list);
    Table select(vectStr& fields_list);
    Table select(vectStr &fields_list, vector<string> &conditions);

    //----The Big Three----
    Table(const Table& other);
    Table& operator =(const Table& RHS);
    ~Table();

    //    void insert_into(MMap<string, string> items);
    void insert(vectStr fields_item);

    vectStr get_index(int index);
    void loadFields();
    void writeFields(vectStr fields);

    string get_db_name(){return _dbname;}

    friend ostream& operator << (ostream& outs,  Table& print_me);


private:
    //indexing functions
    void index_from_file();
    void index_it(vectStr& entries);
    void set_precedence();
    qStr shuntingYard(vectStr& conditions);


    MMap<string, int> precedence;
    vectMmap _mmapVect;
    vectStr _field_list;
    MMap<string, int> _field_list_map;
    char _filename[100];
    char _filename_fields[100];
    string _dbname;
    string _db_fields_list;
    int _recno = 0;
    Record _rec;

};

Table::Table(){
    _dbname[0] = NULL;
    _db_fields_list[0] = NULL;
    _filename[0] = NULL;
    _recno = 0;
    set_precedence();
}

Table::Table(string name){
    _dbname = name;
    string temp = name + "_db.txt";
    _db_fields_list = _dbname + "_fields.txt";
    strcpy(_filename, temp.c_str());
    strcpy(_filename_fields, _db_fields_list.c_str());

    fstream _f;
    open_fileRW(_f, _filename);

    if(file_exists(_filename_fields))
        loadFields();

    _mmapVect.clear();
    for(int i = 0; i < _field_list.size(); i++)
    {
        MMap<string, int> mmap;
        _mmapVect.push_back(mmap);
    }

    _rec.read(_f, _recno);
    _recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);


    cout << "Table CTOR: f_size: " << f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH) << endl;

    if(f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH) > 0)
        index_from_file();


    set_precedence();
    _f.close();
}

Table::Table(string name, vectStr field_list)
{
    fstream _f;
    _field_list = field_list;
    for(int i = 0; i < field_list.size(); i++)
    {
        _field_list_map.insert(field_list[i], i);
    }
    _dbname = name;
    string temp = _dbname + "_db.txt";
    _db_fields_list = _dbname + "_fields.txt";
    strcpy(_filename, temp.c_str());
    strcpy(_filename_fields, _db_fields_list.c_str());
    open_fileRW(_f, _filename);
    _rec.read(_f, _recno);
    _recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
    _mmapVect.clear();
    for(int i = 0; i < field_list.size(); i++)
    {
        MMap<string, int> mmap;
        _mmapVect.push_back(mmap);
    }
    writeFields(field_list);
    if(f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH) > 0)
        index_from_file();
    _f.close();
}


Table::Table(const Table& other)
{
    time_t rawtime;
    time(&rawtime);
    _mmapVect = other._mmapVect;
    _field_list = other._field_list;
    //    srand(time(0));
    _dbname = other._dbname/* + to_string(rand())*/;
    string temp = _dbname + "_db.txt";
    _db_fields_list = _dbname + "_fields.txt";
    strcpy(_filename, temp.c_str());
    strcpy(_filename_fields, _db_fields_list.c_str());
    _recno = other._recno;
}

Table& Table::operator =(const Table& RHS)
{
    _mmapVect = RHS._mmapVect;
    _field_list = RHS._field_list;
    _dbname = RHS._dbname;
    _recno = RHS._recno;
    string temp = _dbname + "_db.txt";
    _db_fields_list = _dbname + "_fields.txt";
    strcpy(_filename, temp.c_str());
    strcpy(_filename_fields, _db_fields_list.c_str());
    //    _filename = RHS._filename;
    //    _filename_fields = RHS._filename_fields;
    return *this;
}

Table::~Table()
{
    _mmapVect.clear();
    _field_list.clear();
}

void Table::insert(vectStr fields_item)
{
    fstream _f;
    open_fileRW(_f, _filename);
    for(int i = 0; i <fields_item.size(); i++)
    {
        strcpy(_rec.record[i], fields_item[i].c_str());
    }

    _f.seekg(0, _f.end);
    _recno = _rec.write(_f);

    if(_mmapVect.size() == 0)
    {
        for(int i = 0; i < fields_item.size(); i++)
        {
            MMap<string, int> mmap;
            _mmapVect.push_back(mmap);
        }
    }

    index_it(fields_item);
    if(false){
        cout << "recno: " << _recno << endl << endl;
        for(int i = 0; i < _recno; i++){
            _rec.read(_f, i);
            cout<<"recono: "<<setw(4)<<i<<" "<<_rec<<endl;
        }
//        cout << "Table: insert: _mmapVect:\n" << _mmapVect << endl << endl;
    }
    _f.close();
}

void Table::index_from_file()
{
    fstream _f;
    open_fileRW(_f, _filename);
    int file_rec_no = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
    if(file_rec_no == 0)
        return;
    if(_mmapVect.size() == 0){
        for(int i = 0; i < _field_list.size(); i++){
            MMap<string, int> mmap;
            _mmapVect.push_back(mmap);
        }
    }
    for(int i = 0; i < file_rec_no; i++){
        _rec.read(_f, i);
        for(int j = 0; j < _field_list.size(); j++){
            _mmapVect[j].insert(_rec.record[j], i);

            cout << "Table: index: j: " << j << endl;
            cout << "Table: index: _mmapVect:\n " << _mmapVect[j] << endl;
        }

    }
    _f.close();
}

void Table::index_it(vectStr& entries){

    cout << "Table: index_it got called\n";
    if(entries.size() == 0)
        return;
    if(_mmapVect.size() == 0){
        for(int i = 0; i < _field_list.size(); i++){
            MMap<string, int> mmap;
            _mmapVect.push_back(mmap);
        }
    }

    for(int i = 0; i < _field_list.size(); i++){

        _mmapVect[i].insert(entries[i], _recno);
    }
}

Table Table::create(string name, vectStr fields_list){
    fstream _f;
    _field_list = fields_list;
    _dbname = name + "_db.txt";
    _db_fields_list = _dbname + "_fields.txt";
    strcpy(_filename, _dbname.c_str());
    open_fileRW(_f, _filename);
    _rec.read(_f, _recno);
    _recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
    //    delete _mma pVect;
    _mmapVect.clear();
    for(int i = 0; i < fields_list.size(); i++){
        MMap<string, int> mmap;
        _mmapVect.push_back(mmap);
    }
    index_from_file();
    _f.close();
    return *this;
}

Table Table::select(vectStr& fields_list){
    if(fields_list[0] == "*"){
        Table temp(*this);
        //        _main.insert(temp.get_db_name(), temp);
        return temp;
    }else{
        vector<int> temp_vect;
        vector<string> str_vect;

        for(int i = 0; i < _field_list.size(); i++){
            //            MMap<string, string> mmap(_field_list_map[fields_list[i]]);

            for(int j = 0; j < fields_list.size(); j++){
                if(fields_list.at(j) == _field_list.at(i))
                    temp_vect.push_back(i);
            }
        }

        fstream _f;
        fstream _f_source;
        int recno = 0;
        Record f_temp(recno);
        srand(time(0));
        string new_db_name = _dbname + to_string(rand());
        string f_temp_name = new_db_name + "_db.txt";
        Table temp(new_db_name, fields_list);
        open_fileRW(_f, f_temp_name.c_str());
        open_fileRW(_f_source, _filename);
        _f_source.seekg(0, _f_source.end);
        //        f_temp.read(_f, recno);
        recno = f_size(_f_source, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
        for(int i = 0; i < recno; i++)
        {
            _rec.read(_f_source, i);
            for(int j = 0; j < temp_vect.size(); j++)
                str_vect.push_back(_rec.record[temp_vect[j]]);
            temp.insert(str_vect);
            str_vect.clear();
        }

        //        time.sleep(1000);
        usleep(800000);
        return temp;

    }

}

Table Table::select(vectStr& fields_list, vectStr& conditions){

    cout << "Table Select w/ fields and conds got called\n";

    vector<int> temp_vect;
    vector<string> str_vect;

    if(fields_list[0] == "*")
        for(int i = 0; i < _field_list.size(); i++)
            temp_vect.push_back(i);
    else
        for(int i = 0; i < _field_list.size(); i++)
        {
            for(int j = 0; j < fields_list.size(); j++)
            {
                if(fields_list.at(j) == _field_list.at(i))
                    temp_vect.push_back(i);
            }
        }

    queue<string> queue_temp;
    stack<string> stack_temp;
    vector<vector<int>> vectVect;
    vector<int> v2;
    queue_temp = shuntingYard(conditions);

    while(!queue_temp.empty())
    {
        cout << "SY: " << queue_temp.front() << endl;
        queue_temp.pop();
    }

    for(int i = 0; i < _field_list.size(); i++)
        _field_list_map.insert(_field_list[i], i);

    while(!queue_temp.empty())
    {
        if(precedence.contains(queue_temp.front()))
        {
            switch(precedence[queue_temp.front()][0])
            {

            case TEN: //operators > < >= <= =
                if(queue_temp.front() == "="){
                    string RHS = stack_temp.top();
                    stack_temp.pop();
                    string LHS = stack_temp.top();
                    stack_temp.pop();

                    MMap<string, int> v = _mmapVect[_field_list_map[LHS][0]];
                    v2 = v[RHS];
                    vectVect.push_back(v2);

                }else if(queue_temp.front() == ">"){
                    string RHS = stack_temp.top();
                    stack_temp.pop();
                    string LHS = stack_temp.top();
                    stack_temp.pop();

                    MMap<string, int>::Iterator it = _mmapVect[_field_list_map[LHS][0]].upper_bound(RHS);
                    MMap<string, int>::Iterator it_end = _mmapVect[_field_list_map[LHS][0]].end();
                    while(it != it_end){
                        v2 += (*it).value_list;
                        it++;
                    }
                    vectVect.push_back(v2);
                }else if(queue_temp.front() == "<"){
                    string RHS = stack_temp.top();
                    stack_temp.pop();
                    string LHS = stack_temp.top();
                    stack_temp.pop();


                    MMap<string, int>::Iterator it = _mmapVect[_field_list_map[LHS][0]].lower_bound(RHS);
                    MMap<string, int>::Iterator it_begin = _mmapVect[_field_list_map[LHS][0]].begin();
                    while(it != it_begin)
                    {
                        v2 += (*it_begin).value_list;
                        it_begin++;
                    }
                    vectVect.push_back(v2);
                }
                else if(queue_temp.front() == ">=")
                {
                    string RHS = stack_temp.top();
                    stack_temp.pop();
                    string LHS = stack_temp.top();
                    stack_temp.pop();

                    MMap<string, int>::Iterator it = _mmapVect[_field_list_map[LHS][0]].lower_bound(RHS);
                    MMap<string, int>::Iterator it_end = _mmapVect[_field_list_map[LHS][0]].end();
                    while(it != it_end)
                    {
                        v2 += (*it).value_list;
                        it++;
                    }
                    vectVect.push_back(v2);
                }
                else if(queue_temp.front() == "<=")
                {
                    string RHS = stack_temp.top();
                    stack_temp.pop();
                    string LHS = stack_temp.top();
                    stack_temp.pop();
                    MMap<string, int>::Iterator it = _mmapVect[_field_list_map[LHS][0]].upper_bound(RHS);
                    MMap<string, int>::Iterator it_begin = _mmapVect[_field_list_map[LHS][0]].begin();
                    while(it != it_begin)
                    {
                        v2 += (*it_begin).value_list;
                        it_begin++;
                    }
                    vectVect.push_back(v2);
                }
                break;

            case SEVEN: //and
                //                for(int i = vectVect.size() - 1; i >= 0 && vectVect.size() - 3; i--)
            {
                //                    v2.clear();
                vector<int> vect_temp1 = vectVect.back();
                vectVect.pop_back();
                vector<int> vect_temp2 = vectVect.back();
                vectVect.pop_back();
                set_intersection(vect_temp1.begin(), vect_temp1.end(), vect_temp2.begin(),
                                 vect_temp2.end(), v2.begin());
                vectVect.push_back(v2);
            }
                break;
            case TWO: //or
            {
                //                    v2.clear();
                vector<int> vect_temp1 = vectVect.back();
                vectVect.pop_back();
                vector<int> vect_temp2 = vectVect.back();
                vectVect.pop_back();
                for(int i = 0; i < vect_temp2.size(); i++)
                    vect_temp1.push_back(vect_temp2[i]);
                sort(vect_temp1.begin(), vect_temp1.end());
                //                vector<int>::iterator it;

                //                it = unique(vect_temp1.begin(), vect_temp1.end());

                //                vect_temp1.resize(distance(vect_temp1.begin(),it));
                vect_temp1.erase(unique (vect_temp1.begin(), vect_temp1.end()), vect_temp1.end());
                v2 = vect_temp1;
                vectVect.push_back(v2);
            }
                break;
            default: //condition names/fields

                break;
                v2.empty();
            }//switch
            queue_temp.pop();

        }//if contains
        else
        {
            stack_temp.push(queue_temp.front());
            queue_temp.pop();


        }//else (!contains)
    }

    fstream _f;
    fstream _f_source;
    int recno = 0;
    Record f_temp(recno);
    srand(time(0));
    //    srand(1000);
    string new_db_name = _dbname + to_string(rand());
    string f_temp_name = new_db_name + "_db.txt";
    Table temp(new_db_name, fields_list);
    open_fileRW(_f, f_temp_name.c_str());
    open_fileRW(_f_source, _filename);
    _f_source.seekg(0, _f_source.end);
    recno = f_size(_f_source, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
    for(int i = 0; i < v2.size(); i++)
    {
        //        cout << "v2: " << v2 << endl;
        for(int j = 0; j < temp_vect.size(); j++)
        {
            _rec.read(_f_source, v2[i]);
            str_vect.push_back(_rec.record[temp_vect[j]]);
            //            temp.insert(str_vect);
            //            str_vect.clear();
        }
        temp.insert(str_vect);
        str_vect.clear();
    }

    //    time.sleep(100);
    usleep(800000);
    return temp;



}

void Table::loadFields(){
    ifstream file(_filename_fields);
    string temp;
    int i = 0;
    if(file.is_open()){
        while(getline(file, temp))
        {
            _field_list.push_back(temp);
            _field_list_map.insert(temp, i);
            i++;
            //its easier to use map
        }
    }

    cout << "Table: loadFields: _field_list.size: " << _field_list.size() << endl;
    file.close();
}

void Table::writeFields(vectStr fields){

    cout << "\n\n\nTable: writeFields got called\n\n";
    ofstream file;
    file.open(_filename_fields);

    for(auto &f : fields)
    {
        file << f << "\n";
    }
    file.close();
}

void Table::set_precedence()
{
    precedence.insert("=", 10);
    precedence.insert("<", 10);
    precedence.insert(">", 10);
    precedence.insert("<=", 10);
    precedence.insert(">=", 10);
    precedence.insert("and", 7);
    precedence.insert("or", 2);

}

Table::qStr Table::shuntingYard(vectStr &conditions){
    queue<string> temp_q;
    stack<string> temp_s;

    set_precedence();
    for(int i = 0; i < conditions.size(); i++)
    {
        if(precedence.contains(conditions[i]))
        {
            while(!temp_s.empty() && precedence[temp_s.top()][0] >= precedence[conditions[i]][0])
            {
                temp_q.push(temp_s.top());
                temp_s.pop();
            }
            temp_s.push(conditions[i]);
        }else{
            temp_q.push(conditions[i]);
        }
    }
    while(!temp_s.empty()){
        temp_q.push(temp_s.top());
        temp_s.pop();
    }
    return temp_q;
}

Table::vectStr Table::get_index(int index){
    vector<string> temp;
    fstream _f;
    int recno = index;
    Record frec(recno);
    open_fileRW(_f,_filename);
    frec.read(_f,recno);
    temp = frec.get_vector();
    _f.close();
    return temp;
}

ostream& operator <<(ostream& outs,  Table& print_me){


    cout << "\n\n\n#######\nTable << operator got called\n\n\n";
    fstream _f;
    open_fileRW(_f, print_me._filename);
    print_me._recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);


    outs << "Table: <<: _recno: " << print_me._recno << endl;

    for(int i = 0; i < print_me._recno; i++){
        print_me._rec.read(_f, i);
        outs<<"recono: "<<setw(4)<<i<<" / "<<print_me._recno<<" "<<print_me._rec<<endl;
    }
    _f.close();
    return outs;
}

#endif // TABLE_H
