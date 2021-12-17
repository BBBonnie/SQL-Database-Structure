#ifndef TOKEN_H
#define TOKEN_H
#include "constants.h"
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

class Token
{
public:
    Token();
    Token(string str, int type);

    int type();
    string type_string() const;
    string token_str() const;


    friend ostream& operator <<(ostream& outs, const Token& t){
        outs<<t.token_str();
        return outs;
    }

private:
    string _token;
    int _type;
};



Token::Token()
{

}

Token::Token(string str, int type): _type(type), _token(str){

}


int Token::type(){
    return _type;
}

string Token::type_string() const{
    string str;

//    cout<<"===type=====:"<<_type<<endl;

//    switch (_type) {
//    case START_NUMBER:
//        str = "NUMBER";

//        break;
//    case START_ALPHA:
//        str = "ALPHA";

//        break;
//    case START_SPACE:
//        str = "SPACE";

//        break;
//    case START_PUNCTUATION:
//        str = "PUNCTUATION";

//        break;
//    case UNKNOWN:
//        str = "UNKNOWN";

//        break;
//    }
    return str;
}

string Token::token_str() const{
    return _token;
}


#endif // TOKEN_H
