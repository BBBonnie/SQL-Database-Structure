#include <iostream>
#include"sql.h"

using namespace std;

void run_from_file(string filename);


int main()
{
    cout << "---------SQL---------" << endl;

//        SQL sql;
    //    //    sql.set_string("select * from student");

    //    sql.set_string("make table student fields fname, lname, major, age");
    //    sql.set_string("insert into student values 	\"Flo\", 			Yao, 		CS, 				20");

    //    sql.set_string("insert into student values 	\"Anna Grace\", 	 \"Del Rio\", 	CS,	 				22");


//        sql.set_string("select * from student where age <= 23");



    //    return 0;

    //    string s ;

    //    cin>>s;

    //    sql.set_string(s);

    //    Map<string, MMap<string, long> > a;
    //    cout<<a["m"]<<endl;

    run_from_file("test.txt");
//    cout<<

    cout<<endl<<endl<<"======THIS IS THE END======"<<endl;
    return 0;
}

void run_from_file(string filename)
{
    ifstream file;
    string buffer;
    SQL sql;

    int i = 0;

    file.open(filename);

    if(file.fail()){
        cout<<"FAIL TO OPEN FILE"<<endl;
    }

  //  getline(file, buffer);

    char temp[100];


    if(file.is_open()){
        while(getline(file, buffer)){
            strcpy(temp,buffer.c_str());

//            cout<<endl<<"temp[0]: "<<temp[0]<<endl;

            if(temp[0] == '/' || strlen(temp) == 0|| temp[0]==' '){
                cout<<buffer<<endl;
                continue;
            }
            cout << "[" << i << "] Command: " << buffer << endl;
            i++;
//            if(i == 99){
//                cout<<" "<<endl;
//            }
            sql.set_string(buffer);
            buffer.clear();
        }
    }

//    sql.print();
    file.close();

}
