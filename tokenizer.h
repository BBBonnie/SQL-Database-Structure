#ifndef TOKENIZER_H
#define TOKENIZER_H
#include"../../BTree/BTree_class/mmap.h"
#include"../../TheStateMachine/ST_00_The_State_Machine/stokenizer.h"
#include"table_const.h"


void t_init_table(int _table[][t_MAX_COLUMNS]);
void t_mark_success(int _table[][t_MAX_COLUMNS], int state);
void t_mark_fail(int _table[][t_MAX_COLUMNS], int state);
bool t_is_success(int _table[][t_MAX_COLUMNS], int state);
void t_mark_cells(int row, int _table[][t_MAX_COLUMNS], int from, int to, int state);
void t_mark_cells(int row, int _table[][t_MAX_COLUMNS], int column, int state);
void t_print_table(int _table[][t_MAX_COLUMNS]);



class Tokenizer
{
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
    friend Tokenizer& operator >>(Tokenizer& tokenizer, MMap<string, string>& token)
    {
//        cout<<"inside of tokenizer::>>------"<<endl;
        tokenizer.get_token(0, token);
        return tokenizer;
    }

    friend ostream& operator << (ostream& outs, const Tokenizer& print_me)
    {
        outs << print_me._dictionary;
        return outs;
    }


private:
    MMap<string, int> _dictionary;
    //    Token<T> _token;
    vector<pair<string, int>> _dic;
    STokenizer _stoken;
    int _ttable[t_MAX_STATE][t_MAX_COLUMNS];
    int _pos;
    int _bufferSize;
};




#endif // TOKENIZER_H
