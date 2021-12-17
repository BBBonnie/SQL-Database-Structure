#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
const int MAX_RECORD_WIDTH = 50;



struct Record{
public:
    Record(long r = 0);
    long rec_no();
    streampos write(fstream& outs);
    streampos read(fstream &ins, streampos recno);
    friend ostream& operator <<(ostream& out,
                          const Record& r);

    vector<string> get_vector();

    char record[MAX_RECORD_WIDTH][MAX_RECORD_WIDTH];

};





#endif // RECORD_H
