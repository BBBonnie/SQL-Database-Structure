#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include<vector>
#include"../../BTree/BTree_class/mmap.h"
#include"../../TheStateMachine/ST_00_The_State_Machine/stokenizer.h"
#include"constants.h"

using namespace std;

static const int MAX = 100;


class Parser
{
public:
    Parser(){

    }
    Parser(char command[MAX]){

    }




    bool get_parse_tree(){

    }


    void set_string(char command[MAX]){
        _s = command;
        _stk.set_string(command);

        Token t;
        _stk>>t;
        while(!_stk.done()){
            if(t.type_string() == "ALPHA" ||t.type_string() == "NUMBER"){

            }

        }
    }







private:

    vector<string> _command;
    string _s;
    STokenizer _stk;

    MMap<string,string> mp;


    void setup(){
        mp["select"] = SELECT;
        mp["*"] = STAR;

    }



    void make_table(){

    }



    void parse_tree(){

    }

    void select_ptree(){

    }


    void create_ptree(){

    }






};





#endif // PARSER_H
