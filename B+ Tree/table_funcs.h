#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <cstring>
#include <iomanip>
#include "constants.h"
using namespace std;



//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state);

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state);

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state);

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state);

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], char from, char to, int state);

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state);

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]);

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);






//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COLUMNS; j++){
            _table[i][j] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    //        for(int i = 0; i < MAX_COLUMNS; i++){
    _table[state][0] = SUCCESS;
    //        }

}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    //    for(int i = 0; i < MAX_COLUMNS; i++){
    _table[state][0] = FAIL;
    //    }

}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    if(_table[state][0] == SUCCESS)
        return true;
    else
        return false;
}

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    for(int i = from; i < to; i++){
        _table[row][i] = state;
    }
}

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], char from, char to, int state){

    cout<<"inside mark cell"<<endl;
    for(int i = static_cast<int>(from); i <= static_cast<int>(to); i++){
        _table[row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    int number_of_colume;
    int i = 0;
    //    for ( int i = 0; i < SIZE; i++){
    while(columns[i] != '\0'){
        number_of_colume = static_cast<int>(columns[i]);
        _table[row][number_of_colume] = state;
        i++;
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state){
    //    for(int i = 0; i < row; i++){
    table[row][column] = state;
    //    }
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){
    cout<<"being called"<<endl;
    for(int i = 0; i < MAX_COLUMNS; i++){
        cout<<endl;
        for(int j = 0; j < MAX_COLUMNS; j++){
            cout<<setw(2)<<_table[i][j];
        }
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos){

//    string str = s;
//    str.at(_pos);
    cout<<"----------------"<<endl;
    cout<<s<<"]<--"<<endl;
    for (int i= 0; i<_pos; i++){
        cout<<" ";
    }
    cout<<"ˆ";
    cout<<endl;
    cout<<"------------------"<<endl;

}

#endif // TABLE_H
