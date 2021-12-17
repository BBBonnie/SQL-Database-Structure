#ifndef CONSTANTS_H
#define CONSTANTS_H
//const int MAX_ROWS = 7;
//const int MAX_BUFFER = 20;
//const int MAX_COLUMNS = 7;



//const int SUCCESS = 1;
//const int FAIL = 0;

//const char DIGITS[] = "0123456789";
//const char ALPHAS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//const char SPACES[] = " ";
//const char PUNCTUATIONS[] = ".,!#$%ˆ&*";


const int t_MAX_COLUMNS = 50;
const int t_MAX_ROWS = 50;
const int t_MAX_STATE = 50;


//const int START_NUMBER = 0;
//const int START_ALPHA = 100;
//const int START_SPACE = 70;
//const int START_PUNCTUATION = 80;
//const int UNKNOWN = 100;

enum num{ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5,
        SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10,
        ELEVEN = 11, TWELVE = 12, THIRTEEN = 13, TWENTY = 20,
        TWENTYFOUR = 24, THIRTY = 30, THIRTYONE = 31, THIRTYTHREE = 33};



enum keys {SELECT = 1, FROM = 4, SEMICOLON = 9, SYMBOL = 2, COMMA =3,
           WHERE = 5, EQUAL = 6, AND = 7, OR = 8, BIGGER = 10, SMALLER = 11,
           BIGGEREQ = 12, SMALLEREQ = 13, QOUTATION = 41, INSERT = 20,
           INTO = 21,VALUE = 22, CREATE = 30, FIELDS = 31, SPACE = 40 };


#endif // CONSTANTS_H
