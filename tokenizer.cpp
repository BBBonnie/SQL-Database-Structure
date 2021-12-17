#include"tokenizer.h"


void t_init_table(int _table[][t_MAX_COLUMNS]){
    for(int i = 0; i < t_MAX_STATE; i++){
        for(int j =1; j < t_MAX_COLUMNS; j++){
            _table[i][j] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void t_mark_success(int _table[][t_MAX_COLUMNS], int state){
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void t_mark_fail(int _table[][t_MAX_COLUMNS], int state){
    _table[state][0] = 0;
}

//true if state is a success state
bool t_is_success(int _table[][t_MAX_COLUMNS], int state){
    if(_table[state][0] == 1)
        return true;
    return false;
}

//Mark a range of cells in the array.
void t_mark_cells(int row, int _table[][t_MAX_COLUMNS], int from, int to, int state){
    for(int i = from; i <= to; i++){
        _table[row][i] = state;
    }
}

void t_mark_cells(int row, int _table[][t_MAX_COLUMNS], int column, int state){
    _table[row][column] = state;
}


void t_print_table(int _table[][t_MAX_COLUMNS]){
    for(int i = 0; i < t_MAX_STATE; i++){
        for(int j = 0; j < t_MAX_COLUMNS; j++){
            cout << "[" <<_table[i][j] << "]  ";
        }
        cout << endl;
    }
}



Tokenizer::Tokenizer(){
    _pos = 0;
    _bufferSize = 0;
    make_table(_ttable);

}

Tokenizer::Tokenizer(string buffer){

    _pos = 0;
    make_dictionary();

    for(int i = 0; i < _dic.size(); i++){
        _dictionary.insert(_dic[i].first, _dic[i].second);

    }

    int n = buffer.length();
    char temp[n];
    strcpy(temp, buffer.c_str());
    make_table(_ttable);
    _stoken.set_string(temp);
}

bool Tokenizer::t_done(){
    return _stoken.done();
}

bool Tokenizer::t_more(){
    return _stoken.more();
}

void Tokenizer::set_buffer(string buffer){
    _pos = 0;
    int n = buffer.length();
    char temp[n];
    strcpy(temp, buffer.c_str());
    _stoken.set_string(temp);
    //    cout << _stoken << endl;
}

string Tokenizer::next_token(){
    string temp;
    if(_stoken.more())
        _stoken.get_token(10, temp);
    return temp;
}

void Tokenizer::make_table(int table[][t_MAX_COLUMNS]){

//    cout<<"--inside make_table--"<<endl;

    t_init_table(table);

    for(int i = 0; i < t_MAX_STATE; i++)
        t_mark_fail(table, i);

    t_mark_success(table, 4); //mark state 4 success
    t_mark_success(table, 8); //mark state 8 success
    t_mark_success(table, 24); //mark state 24 success
    t_mark_success(table, 34); //mark state 34 success



    /*
     *
     * MAEK CELLS FOR "SELECT"
     *
     */

    t_mark_cells(0, table, TSELECT, 1);
    t_mark_cells(1, table, TSTAR, 2);
    t_mark_cells(2, table, TFROM, 3);
    t_mark_cells(3, table, TSYMBOL, 4);
    t_mark_cells(4, table, TWHERE, 5);
    t_mark_cells(5, table, TSYMBOL, 6);
    t_mark_cells(6, table, TSMALLER, 7);
    t_mark_cells(6, table, TBIGGER, 7);
    t_mark_cells(6, table, TSMALLEQU, 7);
    t_mark_cells(6, table, TEQUAL, 7);
    t_mark_cells(6, table, TBIGEQU, 7);
    t_mark_cells(7, table, TQOUTATION, 8);
    t_mark_cells(7, table, TSYMBOL, 8);
    t_mark_cells(8, table, TQOUTATION, 9);
    t_mark_cells(8, table, TAND, 9);
    t_mark_cells(8, table, TOR, 9);
//    t_mark_cells(9, table, TQOUTATION, 6);
    t_mark_cells(9, table, TSYMBOL, 6);
//    t_mark_cells(6, table, TQOUTATION, 7);


    /*
     *
     * MAEK CELLS FOR "CREATE / MAKE"
     *
     */


    t_mark_cells(0, table, TCREATE, 20);
    t_mark_cells(0, table, TMAKE, 20);
    t_mark_cells(20, table, TTABLE, 21);
    t_mark_cells(21, table, TSYMBOL, 22);
    t_mark_cells(22, table, TFIELD, 23);
    t_mark_cells(23, table, TSYMBOL, 24);
    t_mark_cells(24, table, TCOMMA, 25);
    t_mark_cells(25, table, TSYMBOL, 24);



    /*
     *
     * MAEK CELLS FOR "INSERT"
     *
     */

    t_mark_cells(0, table, TINSERT, 30);
    t_mark_cells(30, table, TINTO, 31);
    t_mark_cells(31, table, TSYMBOL, 32);
    t_mark_cells(32, table, TVALUE, 33);
    t_mark_cells(33, table, TSYMBOL, 34);
    t_mark_cells(33, table, TQOUTATION, 34);
    t_mark_cells(34, table, TCOMMA, 35);
    t_mark_cells(35, table, TSYMBOL, 34);
    t_mark_cells(35, table, TQOUTATION, 34);


//    for(int i = 0 ; i < 51; i++){
//        for(int j = 0 ; j < 51; j++){
//            cout<<table[i][j]<<"  ";
//        }
//        cout<<endl;
//    }

}

bool Tokenizer::get_token(int start_state, MMap<string, string> &map_token){

//    cout<<"inside tokenizer::get_token----"<<endl;

    bool b_table = false;
    bool b_field = false;
    bool b_cond = false;
    bool b_value = false;


    Token t;
    int new_state = start_state;
    int i = 0;
    vector<int> dict;
    string temp;
    temp.clear();


    while(new_state != -1 && !_stoken.fail()){
//        cout<<"   "<<t<<endl;
        _stoken >> t;
        if (t.type() == START_SPACE) continue;

        //        cout<<"t: "<<t<<endl<<endl;
        if(_dictionary.contains(t.token_str())){
            dict = _dictionary[t.token_str()];
            //            cout<<"dict:"<<dict<<endl;
            i = dict[0];
//                        cout<<"i: "<<i<<endl<<endl;
        }else{
            i = TSYMBOL;
        }



        new_state = _ttable[new_state][i];



        /*
         * *********************************
         * STATEMACHINE FOR "SELECT"
         * *********************************
         */


        if(new_state == TONE && t.type() == START_ALPHA){
            //if its state 1, the token is command
//            cout<<"==inside state 1==="<<endl;
            map_token.insert("command", t.token_str());
        }

        if(new_state == TTWO && t.type() != START_SPACE ){   // "  *  "

            //NOTHING
            map_token.insert("fields", t.token_str());
        }

        if(new_state == TTHREE && t.type() != START_SPACE){    //"  from  "
            //            cout<<"==inside state 3"<<endl;
            //            //if its state 2, the token is a field token
            //            map_token.insert("from", t.token_str());
            b_table = true;

        }

        if(new_state == TFOUR && t.type() != START_SPACE){     //" table name "

            if(b_table){
                map_token.insert("table", t.token_str());
            }

            b_table = false;

        }

        if(new_state == TFIVE && t.type() != START_SPACE){
            //            map_token.insert("where", t.token_str());
            b_cond = true;
        }

        if(new_state == TSIX && t.type() != START_SPACE){
//            cout<<"==inside state 6"<<endl;

            if(b_cond){
                map_token.insert("conditions", t.token_str());
            }
        }

        if(new_state == TSEVEN && t.type() != START_SPACE){
//            cout<<"==inside state 7"<<endl;
            if(b_cond){
                map_token.insert("conditions", t.token_str());
            }
        }

        if(new_state == TEIGHT && t.token_str() == "\"" && t.type() != START_SPACE){
//            cout<<"==inside state 8"<<endl;
            _stoken >> t;
            while(t.token_str() != "\""){
                temp = temp + t.token_str();
                _stoken >> t;
            }

            if(b_cond){
                map_token.insert("conditions", temp);
            }

            temp.clear();
        }

        if(new_state == TEIGHT && t.token_str() != "\"" && t.type() != START_SPACE){
//            cout<<"==inside state 8"<<endl;
            //if its state 2, the token is a field token

            if(b_cond){
                map_token.insert("conditions", t.token_str());
            }
        }

        if(new_state == TNINE && t.type() != START_SPACE){
//            cout<<"==inside state 9"<<endl;
            //if its state 2, the token is a field token

            if(b_cond){
                map_token.insert("conditions", t.token_str());
            }
        }


        /*
         * *********************************
         * STATEMACHINE FOR "CREATE / MAKE"
         * *********************************
         */


        if(new_state == TTWENTY && t.type() == START_ALPHA)
//            cout<<"inside state 20"<<endl;
            map_token.insert("command", t.token_str());

        if(new_state == TTWENTYONE && t.type() != START_SPACE){
            b_table = true;
            //            map_token.insert("table", t.token_str());
        }

        if(new_state == TTWENTYTWO && t.type() != START_SPACE){
            if(b_table){
                map_token.insert("table", t.token_str());
            }
            b_table = false;
        }

        if(new_state == TTWENTYTHREE && t.type() != START_SPACE){
            b_field = true;
        }

        if(new_state == TTWENTYFOUR && t.type() != START_SPACE){

            if(b_field){
                map_token.insert("fields", t.token_str());
            }

            //            b_field = false;
        }

        if(new_state == TTWENTYFIVE && t.type() != START_SPACE)
            map_token.insert("comma", t.token_str());


        /*
         * *********************************
         * STATEMACHINE FOR "INSERT"
         * *********************************
         */

        if(new_state == TTHIRTY && t.type() == START_ALPHA){
//            cout<<"inside state 30"<<endl;
            map_token.insert("command", t.token_str());
        }

        if(new_state == TTHIRTYONE && t.type() != START_SPACE){
            //            map_token.insert("into", t.token_str());
            b_table = true;
        }


        if(new_state == TTHIRTYTWO && t.type() != START_SPACE){
            if(b_table){
                map_token.insert("table", t.token_str());
            }

            b_table = false;
        }

        if(new_state == TTHIRTYTHREE && t.type() != START_SPACE){
            b_value = true;
        }
        //            map_token.insert("fields", t.token_str());

        if(new_state == TTHIRTYFOUR && t.token_str() == "\"" && t.type() != START_SPACE){
//            cout<<"==inside state 34----1"<<endl;
            _stoken >> t;

            //            cout<<"t1: "<<t<<endl;

            while(t.token_str() != "\""){
                temp = temp + t.token_str();
                _stoken >> t;

                //                cout<<"temp: "<<"["<<temp<<"]"<<endl;
                //                cout<<"t2:"<<t<<endl;

            }

            if(b_value){
                map_token.insert("values", temp);
            }
            temp.clear();
        }

        if(new_state == TTHIRTYFOUR && t.token_str() != "\"" && t.type() != START_SPACE){

//            cout<<"==inside state 34----2"<<endl;
            if(b_value){
                //                cout<<"inside value if"<<endl;
                map_token.insert("values", t.token_str());
            }
        }

        if(new_state == TTHIRTYFIVE && t.type() != START_SPACE)
            //            cout<<"inside state 35"<<endl;
            map_token.insert("comma", t.token_str());

        if(new_state == 10)
            return true;
    }
    return false;
}


void Tokenizer::make_dictionary(){
    _dic.push_back(make_pair("select", 1));
    _dic.push_back(make_pair("*", 2));

    _dic.push_back(make_pair(",", 4));
    _dic.push_back(make_pair("from", 3));
    _dic.push_back(make_pair("symbol", 45));
    _dic.push_back(make_pair("where", 5));
    _dic.push_back(make_pair("=", 6));
    _dic.push_back(make_pair("and", 7));
    _dic.push_back(make_pair("or", 8));


    //    _dic.push_back(make_pair(";", 9));


    _dic.push_back(make_pair(">", 10));
    _dic.push_back(make_pair("<", 11));
    _dic.push_back(make_pair(">=", 12));
    _dic.push_back(make_pair("<=", 13));

    _dic.push_back(make_pair("insert", 20));
    _dic.push_back(make_pair("into", 21));

    _dic.push_back(make_pair("values", 22));
    _dic.push_back(make_pair("fields", 23));
    _dic.push_back(make_pair("create", 30));
    _dic.push_back(make_pair("make", 31));

    _dic.push_back(make_pair("table", 32));

    _dic.push_back(make_pair(" ", 40));
    _dic.push_back(make_pair("\n", 40));
    _dic.push_back(make_pair("\t", 40));


    _dic.push_back(make_pair("\"", 41));
    //    _dic.push_back(make_pair("”", 41));

}
