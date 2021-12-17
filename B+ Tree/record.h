#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
const int MAX_RECORD_WIDTH = 50;


bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

bool f_read_info(const char filename[], vector<string>& v);
bool f_write_info(const char filename[], const vector<string> &v);
long f_size(fstream& f, int max_size);


class Record{
public:
//    Record(){
//        record[0] = NULL;
//        recno = -1;
//    }

//    Record(char str[]){
//        strcpy(record, str);
//    }

//    long write(fstream& outs);
//    long read(fstream& ins, long recno);


//    friend ostream& operator<<(ostream& outs,
//                               const Record& r);

    Record(long r = 0);
    long rec_no();
    streampos write(fstream& outs);
    streampos read(fstream &ins, streampos recno);
    friend ostream& operator <<(ostream& out,
                          const Record& r);

    vector<string> get_vector();

    char record[MAX_RECORD_WIDTH][MAX_RECORD_WIDTH];
//private:
//    static const int MAX = 10;
//    int recno;
//    char record[MAX];
};


Record::Record(long r)
{
    strcpy(record[0], to_string(r).c_str());
    for (int i = 1 ; i<MAX_RECORD_WIDTH; i++)
        record[i][0] = NULL;
}

long Record::rec_no(){

    return stol(string(record[0]));
}

streampos Record::write(fstream& outs){
    //write to the end of the file.

//    if(pos.__off_ < 0)
    streampos pos = outs.tellp();
    outs.write(&record[0][0], sizeof(record));
    return pos/sizeof(record);
}

streampos Record::read(fstream& ins, streampos recno){
    streampos pos= recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);
    //ins.read(&record[1][0], MAX_RECORD_WIDTH*(MAX_RECORD_WIDTH-1));
    ins.read(&record[0][0], sizeof(record));
    return ins.gcount();
}

vector<string> Record::get_vector()
{
   vector<string> hold;
   for(int i = 1;record[i][0]!=NULL && i<MAX_RECORD_WIDTH;i++){
       hold.push_back(record[i]);
   }
   return hold;
}

ostream& operator <<(ostream& outs,
                      const Record& r){
    for (int i=0; r.record[i][0]!= NULL && i<MAX_RECORD_WIDTH; i++){
            outs<<setw(15)<<r.record[i];
    }
    return outs;
}


//long Record::write(fstream &outs){
//    //write to the end of the file.
//    long pos = outs.tellp();

//    //outs.write(&record[0], sizeof(record));
//    outs.write(record, sizeof(record));

//    return pos;
//}

//long Record::read(fstream &ins, long recno){
//    long pos= recno * sizeof(record);
//    ins.seekg(pos, ios_base::beg);


//    ins.read(record, sizeof(record));
//    return ins.gcount();

//}
//ostream& operator<<(ostream& outs,
//                    const Record& r){
//    outs<<r.record;
//    return outs;
//}

////-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
             std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
                std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

bool f_read_info(const char filename[], vector<string>& v){
    ifstream f;
    f.open(filename);
    if (f.fail()){
        return false;
    }
    string s;
    getline(f, s);
    while (!f.eof()){
        v.push_back(s);
        getline(f, s);
    }
    f.close();
    return true;
}
bool f_write_info(const char filename[], const vector<string>& v){
    ofstream f;
    f.open(filename);
    if (f.fail()) return false;
    for (int i= 0; i<v.size(); i++){
        f<<v[i]<<endl;
    }
    f.close();
    return true;
}

long f_size(fstream& f, int max_size)
{
    f.seekg(0, f.end);
    return f.tellp()/max_size;
}

void save_list(Record list[], int count){
    fstream f;
    open_fileW(f, "record_list.bin");
    for (int i= 0; i<count; i++){
        list[i].write(f);
    }
    f.close();

}

#endif // RECORD_H
