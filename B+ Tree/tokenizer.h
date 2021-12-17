#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <iostream>
#include <sstream>


#include"constants.h"
#include"../../BTree/BTree_class/mmap.h"
#include"../../TheStateMachine/ST_00_The_State_Machine/stokenizer.h"


using namespace std;


void t_init_table(int _table[][t_MAX_COLUMNS]){
    for(int i = 0; i < t_MAX_STATE; i++)
    {
        for(int j = 1; j < t_MAX_COLUMNS; j++)
        {
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
    for(int i = from; i <= to; i++)
    {
        _table[row][i] = state;
    }
}

void t_mark_cells(int row, int _table[][t_MAX_COLUMNS], int column, int state){
    _table[row][column] = state;
}

//This can realistically be used on a small table
void t_print_table(int _table[][t_MAX_COLUMNS]){
    for(int i = 0; i < t_MAX_STATE; i++)
    {
        for(int j = 0; j < t_MAX_COLUMNS; j++)
        {
            cout << "[" <<_table[i][j] << "]  ";
        }
        cout << endl;
    }
}



class Tokenizer{
public:

    Tokenizer();
    Tokenizer(string buffer);
    string next_token();
    bool t_more();
    bool t_done();
    void set_buffer(string buffer);
    void make_table(int table[][t_MAX_COLUMNS]);
    bool get_token(int start_state, MMap<string, string>& map_token);
    void make_dictionary();
    friend Tokenizer& operator >>(Tokenizer& tokenizer, MMap<string, string>& token){
        tokenizer.get_token(0, token);
        return tokenizer;
    }

    friend ostream& operator << (ostream& outs, const Tokenizer& print_me){
        outs << print_me._dictionary;
        return outs;
    }


private:
    MMap<string, int> _dictionary;
    //    Token<T> _token;
    vector<pair<string, int>> _v;
    STokenizer _stoken;
    int _ttable[t_MAX_STATE][t_MAX_COLUMNS];
    int _pos;
    int _bufferSize;
};

Tokenizer::Tokenizer()
{
    _pos = 0;
    _bufferSize = 0;
    make_table(_ttable);

}

Tokenizer::Tokenizer(string buffer)
{

    _pos = 0;
    make_dictionary();

    for(int i = 0; i < _v.size(); i++){
        _dictionary.insert(_v[i].first, _v[i].second);
            cout << "Tokenizer CTOR: for loop: i: " << i << " " << _v[i].first
                 << " " << _v[i].second << endl;
    }

    int n = buffer.length();
    char temp[n];
    strcpy(temp, buffer.c_str());
    make_table(_ttable);
    _stoken.set_string(temp);
}

bool Tokenizer::t_done()
{
    return _stoken.done();
}

bool Tokenizer::t_more()
{
    return _stoken.more();
}

void Tokenizer::set_buffer(string buffer)
{
    _pos = 0;
    int n = buffer.length();
    char temp[n];
    strcpy(temp, buffer.c_str());
    _stoken.set_string(temp);
//    cout << _stoken << endl;
}

string Tokenizer::next_token()
{
    string temp;
    if(_stoken.more())
        _stoken.get_token(10, temp);

    return temp;
}

void Tokenizer::make_table(int table[][t_MAX_COLUMNS])
{
    t_init_table(table);

    for(int i = 0; i < t_MAX_STATE; i++)
        t_mark_fail(table, i);

    t_mark_success(table, 9); //mark state 4 success

    t_mark_cells(0, table, SELECT, 1);
    t_mark_cells(0, table, INSERT, 20);
    t_mark_cells(0, table, CREATE, 30);
    t_mark_cells(0, table, SPACE, 0);

    t_mark_cells(1, table, SYMBOL, 2);
    t_mark_cells(1, table, SPACE, 1);
    t_mark_cells(1, table, QOUTATION, 2);

    t_mark_cells(2, table, COMMA, 1);
    t_mark_cells(2, table, FROM, 3);
    t_mark_cells(2, table, SPACE, 2);
//    t_mark_cells(2, )

    t_mark_cells(3, table, SYMBOL, 4);
    t_mark_cells(3, table, SPACE, 3);

    t_mark_cells(4, table, WHERE, 5);
    t_mark_cells(4, table, SEMICOLON, 9);
    t_mark_cells(4, table, SPACE, 4);


    t_mark_cells(5, table, SYMBOL, 6);
    t_mark_cells(5, table, SPACE, 5);
    t_mark_cells(5, table, QOUTATION, 6);

//    t_mark_cells(6, table, TEQUAL, 7);
    t_mark_cells(6, table, SYMBOL, 6);
    t_mark_cells(6, table, SPACE, 6);
    t_mark_cells(6, table, SEMICOLON, 9);
    t_mark_cells(6, table, QOUTATION, 6);

//    t_mark_cells(7, table, SYMBOL, 8);
//    t_mark_cells(7, table, QOUTATION, 12);
//    t_mark_cells(7, table, SPACE, 7);

//    t_mark_cells(8, table, AND, TOR, 5);
//    t_mark_cells(8, table, BIGGER, TSMALLEQU, 5);
//    t_mark_cells(8, table, SEMICOLON, 9);
//    t_mark_cells(8, table, SPACE, 8);

    t_mark_cells(9, table, SEMICOLON, 9);
    t_mark_cells(9, table, SPACE, 9);

//    t_mark_cells(10, table, SYMBOL, 11);
//    t_mark_cells(10, table, SPACE, 10);
//    t_mark_cells(10, table, QOUTATION, 2);

//    t_mark_cells(11, table, SPACE, 10);
//    t_mark_cells(11, table, QOUTATION, 2);

//    t_mark_cells(12, table, SYMBOL, 13);
//    t_mark_cells(12, table, SPACE, 12);
//    t_mark_cells(12, table, QOUTATION, 8);

//    t_mark_cells(13, table, SPACE, 12);
//    t_mark_cells(13, table, QOUTATION, 8);

    t_mark_cells(20, table, INTO, 21);
    t_mark_cells(20, table, SPACE, 20);

    t_mark_cells(21, table, SYMBOL, 22);
    t_mark_cells(21, table, SPACE, 21);

    t_mark_cells(22, table, VALUE, 23);
    t_mark_cells(22, table, SPACE, 22);

    t_mark_cells(23, table, SYMBOL, 24);
    t_mark_cells(23, table, SPACE, 23);
    t_mark_cells(23, table, QOUTATION, 24);

    t_mark_cells(24, table, COMMA, 23);
    t_mark_cells(24, table, SEMICOLON, 9);
    t_mark_cells(24, table, SPACE, 24);

    t_mark_cells(30, table, SYMBOL, 31);
    t_mark_cells(30, table, SPACE, 30);
    t_mark_cells(30, table, CREATE, 30);

    t_mark_cells(31, table, FIELDS, 32);
    t_mark_cells(31, table, SPACE, 31);

    t_mark_cells(32, table, SYMBOL, 33);
    t_mark_cells(32, table, SPACE, 32);
    t_mark_cells(32, table, QOUTATION, 33);


    t_mark_cells(33, table, COMMA, 32);
    t_mark_cells(33, table, SEMICOLON, 9);
    t_mark_cells(33, table, SPACE, 33);
    t_mark_cells(33, table, QOUTATION, 33);






        t_print_table(table);

}

bool Tokenizer::get_token(int start_state, MMap<string, string> &map_token)
{
    /***************
     * get the first token key from the dictionary
     * go to the new state
     * if the new state is success, last_success = _pos
     * if -1 : return 0
     * if != -1 : gets the new state
     * *************/
    Token t;
    int new_state = start_state;
    int i = 0;
    vector<int> dict;
    string temp;
    temp.clear();
    while(new_state != -1 && !_stoken.done())
    {
        _stoken >> t;
//        cout << "\n\nInside the while loop:\n" << t.token_str() << " : " << t.type() << endl;
        if(_dictionary.contains(t.token_str()))
        {
            dict = _dictionary[t.token_str()];
            i = dict[0];
        }
        else
        {
            i = SYMBOL;
        }

        new_state = _ttable[new_state][i];
        if(new_state == ONE && t.type() == START_ALPHA)
            //if its state 1, the token is command
            map_token.insert("command", t.token_str());
        if(new_state == TWO && t.token_str() == "\"" && t.type() != SPACE)
        {
            _stoken >> t;
            while(t.token_str() != "\"")
            {
                temp = temp + t.token_str();
                _stoken >> t;
            }
            map_token.insert("fields", temp);
            temp.clear();
        }
        if(new_state == TWO && t.token_str() != "\"" && t.type() != SPACE)
            //if its state 2, the token is a field token
            map_token.insert("fields", t.token_str());

        if(new_state == FOUR && t.type() != SPACE)
            map_token.insert("table", t.token_str());
//        if(new_state == TFIVE && t.type() != SPACE)
//            map_token.insert("andor", t.token_str());
        if(new_state == SIX && t.token_str() == "\"" && t.type() != SPACE)
        {
            _stoken >> t;
            while(t.token_str() != "\"")
            {
                temp = temp + t.token_str();
                _stoken >> t;
            }
            map_token.insert("conditions", temp);
            temp.clear();
        }
        if(new_state == SIX && t.token_str() != "\"" && t.type() != SPACE)
            map_token.insert("conditions", t.token_str());

        if(new_state == TWENTY && t.type() == ALPHA)
            map_token.insert("command", t.token_str());
        if(new_state == TWENTYTWO && t.type() != SPACE)
            map_token.insert("table", t.token_str());
        if(new_state == TWENTYFOUR && t.token_str() == "\"" && t.type() != SPACE)
        {
            _stoken >> t;
            while(t.token_str() != "\"")
            {
                temp = temp + t.token_str();
                _stoken >> t;
            }
            map_token.insert("values", temp);
            temp.clear();
        }
        if(new_state == TWENTYFOUR && t.token_str() != "\"" && t.type() != SPACE)
            map_token.insert("values", t.token_str());

        if(new_state == THIRTY && t.type() == START_ALPHA)
            map_token.insert("command", t.token_str());
        if(new_state == THIRTYONE && t.type() != SPACE)
            map_token.insert("table", t.token_str());
        if(new_state == THIRTYTHREE && t.token_str() == "\"" && t.type() != SPACE)
        {
            _stoken >> t;
            while(t.token_str() != "\"")
            {
                temp = temp + t.token_str();
                _stoken >> t;
            }
            map_token.insert("fields", temp);
            temp.clear();
        }
        if(new_state == THIRTYTHREE && t.token_str() != "\"" && t.type() != SPACE)
            map_token.insert("fields", t.token_str());

        if(new_state == 9)
            return true;

    }

    return false;

}


void Tokenizer::make_dictionary()
{
    _v.push_back(make_pair("select", 1));
    _v.push_back(make_pair("symbol", 2));
    _v.push_back(make_pair(",", 3));
    _v.push_back(make_pair("from", 4));
    _v.push_back(make_pair("where", 5));
//    _v.push_back(make_pair("=", 6));
//    _v.push_back(make_pair("and", 7));
//    _v.push_back(make_pair("or", 8));
    _v.push_back(make_pair(";", 9));
//    _v.push_back(make_pair(">", 10));
//    _v.push_back(make_pair("<", 11));
//    _v.push_back(make_pair(">=", 12));
//    _v.push_back(make_pair("<=", 13));

    _v.push_back(make_pair("insert", 20));
    _v.push_back(make_pair("into", 21));

    _v.push_back(make_pair("values", 22));
    _v.push_back(make_pair("make", 30));

    _v.push_back(make_pair("table", 30));
    _v.push_back(make_pair("fields", 31));
    _v.push_back(make_pair(" ", 40));
    _v.push_back(make_pair("\n", 40));
    _v.push_back(make_pair("\t", 40));
//    _v.p

    _v.push_back(make_pair("\"", 41));
    _v.push_back(make_pair("”", 41));

}

#endif // TOKENIZER_H
