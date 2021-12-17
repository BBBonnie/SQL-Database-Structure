#include"sql.h"
SQL::SQL(){}

SQL::SQL(string &input){
    Tokenizer _tok(input);
    while(_tok.t_more()){
        _tok >> _ptree;
    }
}

void SQL::run(){

//    cout<<_ptree;


    vector<string> COMMAND = _ptree["command"];


    if(COMMAND[0] == "select"){

//        cout<<"inside sql::select"<<endl;

        vector<string> TABLE = _ptree["table"];
        vector<string> FIELDS = _ptree["fields"];
        vector<string> CONDITIONS = _ptree["conditions"];
        if(_table.contains(TABLE[0])){

//                    cout<<"inside sql::_table.contains()"<<endl;


            if(!CONDITIONS.empty()){

//                cout<<"inside sql::CONDITIONS.empty()"<<endl;

//                cout<<"TABLE[0]: "<<TABLE[0]<<endl;

//                cout<<"_table[TABLE[0]]: "<<_table[TABLE[0]]<<endl;

//                cout<<"_table: "<<_table<<endl;

                Table temp = _table[TABLE[0]].select(FIELDS, CONDITIONS);
//                                cout<<"=====2222==="<<endl;
                _table.insert(temp.get_db_name(), temp);

                cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _table[temp.get_db_name()] << endl;

            }else{
                Table temp = _table[TABLE[0]].select(FIELDS);
                _table.insert(temp.get_db_name(), temp);
                cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _table[temp.get_db_name()] << endl;


            }
        }else{
            Table t(TABLE[0]);
            _table.insert(TABLE[0], t);
            if(!CONDITIONS.empty()){
                Table temp = _table[TABLE[0]].select(FIELDS, CONDITIONS);
                _table.insert(temp.get_db_name(), temp);
                cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _table[temp.get_db_name()] << endl;

                //                cout << temp;
            }else{
                Table temp = _table[TABLE[0]].select(FIELDS);
                _table.insert(temp.get_db_name(), temp);
                cout << endl << "Table name: " << temp.get_db_name() << ":\n" << _table[temp.get_db_name()] << endl;

            }
        }


//        cout<<"&&&&&&&&&&&&&&&&&&&&&_table[TABLE[0]]: "<<_table[TABLE[0]]<<endl<<endl<<endl;
    }

    if(COMMAND[0] == "make"){
//        cout << "SQL: run: create got called";
        vector<string> TABLE = _ptree["table"];
        vector<string> FIELD = _ptree["fields"];

        Table t(TABLE[0], FIELD);
    }

    if(COMMAND[0] == "insert"){
        vector<string> TABLE = _ptree["table"];
        vector<string> VALUES = _ptree["values"];
        if(_table.contains(TABLE[0])){
            //            cout << "SQL: contains table";
            _table[TABLE[0]].insert(VALUES);
        }else{
            //            cout << "SQL: does not contain the table";
            Table t(TABLE[0]);

            t.insert(VALUES);

            _table.insert(TABLE[0], t);

        }
//        cout << "SQL::run: Inserted";

    }
     cout<<endl<<endl<< "====SQL DONE!======"<<endl<<endl<<endl;
}

void SQL::set_string(string input)
{
//    cout<<"SQL::inside set_string"<<endl;

    //    getline(cin, input);
    Tokenizer _tok(input);

    //    _ptree.clear();
    MMap<string, string> s_mmap;

    _ptree = s_mmap;

    while(!_tok.t_done()){
//        cout<<_tok<<endl;
//        cout<<"!!!!!!!!!!!!!!!!!!!!"<<endl;

        _tok >> _ptree;
//        cout<<_tok<<endl;
//        cout<<_ptree<<endl;
    }
//    cout<<"======================"<<endl;


    run();
}
