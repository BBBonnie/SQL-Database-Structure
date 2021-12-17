#include"record.h"

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

    streampos pos = outs.tellp();
    outs.write(&record[0][0], sizeof(record));
    return pos/sizeof(record);
}

streampos Record::read(fstream& ins, streampos recno){
    streampos pos= recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);

    ins.read(&record[0][0], sizeof(record));
    return ins.gcount();
}

vector<string> Record::get_vector(){
   vector<string> hold;
   for(int i = 1;record[i][0]!=NULL && i<MAX_RECORD_WIDTH;i++){
       hold.push_back(record[i]);
   }
   return hold;
}

ostream& operator <<(ostream& outs, const Record& r){
    for (int i=0; r.record[i][0]!= NULL && i<MAX_RECORD_WIDTH; i++){
            outs<<setw(15)<<r.record[i];
    }
    return outs;
}
