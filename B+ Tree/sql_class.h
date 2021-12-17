#ifndef SQL_CLASS_H
#define SQL_CLASS_H
#include <iostream>
#include <string>
#include "../../BTree/BTree_class/map.h"
#include "../../BTree/BTree_class/mmap.h"
#include "tokenizer.h"
#include "table.h"

using namespace std;
class SQL
{
public:


        typedef MMap<string, string> MMapStr;
        typedef vector<string> vectStr;
        typedef vector<MMap<string, string>> vectMmap;

        SQL();
        SQL(string& input);
        SQL(string& input, fstream& f);

        Table& operator[](const string& key)
        {
            return _main[key];
        }

        void set_string(string input);

        string get_command();

        void run();

        friend ostream& operator <<(ostream& outs, SQL& print_me)
        {
    //        for(int i = 0; i < _db.size())
            outs << print_me._main;
            return outs;
        }


    private:
        Map<string, Table> _main;
        MMapStr _ptree;
        Table _table;
        vectStr _command;
        vectMmap _db;
    };

    SQL::SQL()
    {

    }

    SQL::SQL(string &input)
    {
        Tokenizer _tok(input);
        while(_tok.t_more())
        {
            _tok >> _ptree;
        }
    //    cout << endl << _ptree["command"] << endl;

    }

    void SQL::run()
    {
    //    cout << endl << "table: "  << _ptree["command"] << endl;
        vectStr command = _ptree["command"];
        if(command[0] == "insert")
        {
            vectStr table = _ptree["table"];
            vectStr values = _ptree["values"];
            if(_main.contains(table[0]))
            {
    //            cout << "SQL: contains table\n";
                _main[table[0]].insert(values);
            }
            else
            {
    //            cout << "SQL: not contains the table\n";
                Table t(table[0]);
    //            cout << "SQL: insert: after else\n";

                t.insert(values);

                _main.insert(table[0], t);

            }
            cout << "\nSQL::run: Inserted\n";
    //        cout << t << endl;
    //        cout << endl << table[0] << ": " << _main[table[0]]<<endl;
        }
        if(command[0] == "select")
        {
            vectStr table = _ptree["table"];
            vectStr fields = _ptree["fields"];
            vectStr conditions = _ptree["conditions"];
            if(_main.contains(table[0]))
            {
                if(!conditions.empty())
                {
                    Table temp = _main[table[0]].select(fields, conditions);
                    _main.insert(temp.get_db_name(), temp);
                    cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _main[temp.get_db_name()] << endl;

    //                cout << temp;
                }
                else
                {
                    Table temp = _main[table[0]].select(fields);
                    _main.insert(temp.get_db_name(), temp);
                    cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _main[temp.get_db_name()] << endl;

    //                cout << "\n\n\n\n========\n\nFinal table: " << temp;

                }
            }
            else
            {
                Table t(table[0]);
                _main.insert(table[0], t);
                if(!conditions.empty())
                {
                    Table temp = _main[table[0]].select(fields, conditions);
                    _main.insert(temp.get_db_name(), temp);
                    cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _main[temp.get_db_name()] << endl;

    //                cout << temp;
                }
                else
                {
                    Table temp = _main[table[0]].select(fields);
                    _main.insert(temp.get_db_name(), temp);
                    cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _main[temp.get_db_name()] << endl;
    //                cout << "\n\n\n\n========\n\nFinal table: " << temp;

                }
            }
    //        cout << endl << "Table name: " << table[0] << ":\n " << _main[table[0]]<<endl;
        }
        if(command[0] == "make")
        {

            vectStr tab = _ptree["table"];
            vectStr fie = _ptree["fields"];

            Table t(tab[0], fie);
            _main.insert(tab[0], t);
    //        cout << t << endl;
        }

            cout << _main << endl;

        cout << "\n\nSQL DONE!\n\n";

    }

    void SQL::set_string(string input)
    {
        ;
    //    getline(cin, input);
        Tokenizer _tok(input);
    //    _ptree.clear();
        MMap<string, string> nmmap;
        _ptree = nmmap;
        while(_tok.t_more())
            _tok >> _ptree;
        run();
    }


#endif // SQL_CLASS_H
