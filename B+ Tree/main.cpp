#include <iostream>
#include <string>
#include"bplustree_class.h"
#include"array_funcs.h"
#include"sql_class.h"
#include"../../BTree/BTree_class/mmap.h"
#include"../../TheStateMachine/ST_00_The_State_Machine/ftokenizer.h"
using namespace std;


void test_make_tree();
void test_BTree_auto(int tree_size=5000, int how_many=500, bool report=false);
bool test_btree_auto(int how_many, bool report=true);
MMap<string, long> get_word_indices(char* file_name);


int main()
{
    cout << "--------B PLUS TREE----------" << endl;

    //    test_BTree_auto(200,200,true);

    //    cout<<"====THIS IS THE END===="<<endl;
    //    return 0;



//    test_make_tree();
//    BPlusTree<int> bpt;
//    for (int i= 1; i<6; i++){
//        bpt.insert(i*10);
//    }

//    BPlusTree<int>::Iterator it;
//    if(bpt.contains(10)){
//        cout<<"yea";
//    }

//    cout<<*it<<endl;

//    return 0;



//    MMap<string,int> mm;
//    mm.insert("Bob",2);
//    mm.insert("Joe",4);
//    mm.insert("Pan",6);
//    mm.insert("Pan",2);

//    cout<<mm<<endl;

//    cout<<mm["Pan"]<<endl;
//    return 0;

//    MMap<string, long> word_indices;
//        word_indices = get_word_indices("solitude.txt");
////        cout<<"===here==="<<endl;
//        cout<<endl<<endl<<endl;

//        //list all nodes of the index mmap:
//        for (MMap<string, long>::Iterator it = word_indices.begin();
//             it != word_indices.end(); it++){
//            cout<<*it<<endl;
//        }


//        cout<<endl<<endl<<endl;
//        cout<<"---------------------------------------------------"<<endl;
//        string this_word = "ice";
//        cout<<"---------------------------------------------------"<<endl;
//        cout<<"Indices of \""<<this_word<<"\""<<endl;
//        //list indices of this_word:
//        if (word_indices.contains(this_word)){
//            cout<<this_word<<": "<<word_indices[this_word]<<endl;
//        }
//        cout<<endl<<endl<<endl;

//        cout<<"---------------------------------------------------"<<endl;
//        string from = "ask";
//        string to = "asker";
//        //list from .. to:
//        cout<<"listing indices from \""<<from<<"\" to \""<<to<<"\""<<endl;
//        cout<<"---------------------------------------------------"<<endl;
//        for (MMap<string, long>::Iterator it =
//                    word_indices.lower_bound(from);
//             it != word_indices.upper_bound(to) &&
//             it != word_indices.end(); it++){
//            cout<<*it<<endl;
//        }

//        cout<<endl<<endl<<endl<< "========== E N D  ====================" << endl;
//        return 0;


    //    cout <<endl<<endl<<endl<< "================================" << endl;
    //    Record list[10] = {"zero",
    //                       "one",
    //                       "two",
    //                       "three",
    //                       "four",
    //                       "five",
    //                       "six",
    //                       "seven",
    //                       "eight",
    //                       "nine"};
    //    for (int i = 0; i< 10; i++){
    //        cout<<list[i]<<endl;
    //    }
    //    save_list(list, 10);


    //    Record r;
    //    fstream f;
    //    open_fileRW(f, "record_list.bin" );
    //    r.read(f, 3);
    //    cout<<"record 3: "<<r<<endl;
    //    r.read(f, 6);
    //    cout<<"record 6: "<<r<<endl;

    //    cout <<endl<<endl<<endl<< "================================" << endl;
    //    return 0;












    //    test_make_tree();
//        BPlusTree<int> bpt;
//            for (int i= 1; i<6; i++){
//                bpt.insert(i*10);
//            }
    //    bpt.insert(20);
    //    bpt.insert(25);
    //    bpt.insert(30);
    //    bpt.insert(35);
    //    bpt.insert(40);
    //    bpt.insert(45);

//        cout<<bpt<<endl;
    //    //    bpt.list_keys();
    //    cout<<"====================================="<<endl;
    //    cout<<"====================================="<<endl;


    //    //    bpt.insert(11);
    //    //    bpt.insert(22);
    //    //    bpt.insert(33);
    //    //    bpt.insert(44);
    //    //    bpt.insert(55);


    //    //    cout<<bpt<<endl;
    //    //    cout<<"====================================="<<endl;
    //    //    cout<<"====================================="<<endl;
    //    //    bpt.list_keys();


    //    //    bpt.insert(12);
    //    //    bpt.insert(23);
    //    //    bpt.insert(34);
    //    //    bpt.insert(45);
    //    //    bpt.insert(56);

    //    //    cout<<bpt<<endl;

    //    //    cout<<"====================================="<<endl;
    //    //    cout<<"====================================="<<endl;

    //    cout<<endl<<"remove 50: ----"<<endl;
    //    bpt.remove(50);
    //    cout<<bpt<<endl;
    //    cout<<"====================================="<<endl;
    //    cout<<"====================================="<<endl;
    //    //    bpt.list_keys();

    //    cout<<endl<<"remove 40: ----"<<endl;
    //    bpt.remove(40);
    //    cout<<bpt<<endl;
    //    cout<<"====================================="<<endl;
    //    cout<<"====================================="<<endl;


    //    cout<<endl<<"remove 45: ----"<<endl;
    //    bpt.remove(45);
    //    cout<<bpt<<endl;
    //    cout<<"====================================="<<endl;
    //    cout<<"====================================="<<endl;

    //    cout<<endl<<"remove 30: ----"<<endl;
    //    bpt.remove(30);
    //    cout<<bpt<<endl;
    //    cout<<"====================================="<<endl;
    //    cout<<"====================================="<<endl;


    //    BPlusTree<int>::Iterator it;
    //    cout<<"*it: "<<*it<<endl;



    SQL sql;





    cout<<"-------THIS IS THE END-------"<<endl;
    return 0;
}

MMap<string, long> get_word_indices(char* file_name){
//    const bool debug = false;
    MMap<string, long> word_indices;
    FTokenizer ftk("solitude.txt");

    Token t;
    long count = 0;


    ftk >> t;
    while (ftk.more()){

        cout << count << ' ';
        //only the "words"
        if (t.type_string() == "ALPHA"){

            string s;
            s = t.token_str();
            word_indices[s] += count;

            count++;

//            if (debug)
//                cout<<"|"<<t.token_str()<<"|"<<endl;
        }

        ftk >> t;


//        cout<<"----HERE___"<<endl;
    }

    return word_indices;
}

void test_make_tree(){

    bool b = false;
    BPlusTree<int> bpt(b);
    bpt.make_tree();
}

//void test_BTree_auto(int tree_size, int how_many, bool report){
//    bool verified = true;
//    for (int i = 0; i < how_many; i++){
//        if (report){
//            cout<<"*********************************************************"<<endl;
//            cout<<" T E S T:    "<<i<<endl;
//            cout<<"*********************************************************"<<endl;
//        }
//        if (!test_btree_auto(tree_size, report)){
//            cout<<"T E S T :   ["<<i<<"]    F A I L E D ! ! !"<<endl;
//            verified = false;
//            return;
//        }

//    }
//    cout<<"**************************************************************************"<<endl;
//    cout<<"**************************************************************************"<<endl;
//    cout<<"             E N D     T E S T: "<<how_many<<" tests of "<<tree_size<<" items: ";
//    cout<<(verified?"VERIFIED": "VERIFICATION FAILED")<<endl;
//    cout<<"**************************************************************************"<<endl;
//    cout<<"**************************************************************************"<<endl;
//}

//bool test_btree_auto(int how_many, bool report){
//    const int MAX = 10000;
//    assert(how_many < MAX);
//    BPlusTree<int> bpt;
//    int a[MAX];
//    int original[MAX];
//    int deleted_list[MAX];

//    int original_size;
//    int size;
//    int deleted_size = 0;

//    //fill a[ ]
//    for (int i= 0; i< how_many; i++){
//        a[i] = i;
//    }
//    //shuffle a[ ]: Put this in a function!
//    for (int i = 0; i< how_many; i++){
//        int from = /*Random(0, how_many-1)*/ rand()% how_many-1;
//        int to = /*Random(0, how_many -1)*/ rand()% how_many-1;
//        int temp = a[to];
//        a[to] = a[from];
//        a [from] = temp;
//    }
//    //copy  a[ ] -> original[ ]:
//    copy_array(original, a, original_size, how_many);
//    size = how_many;
//    original_size = how_many;
//    for (int i=0; i<size; i++){
//        bpt.insert(a[i]);

//    }
//    if (report){
//        cout<<"========================================================"<<endl;
//        cout<<"  "<<endl;
//        cout<<"========================================================"<<endl;
//        cout<<bpt<<endl<<endl;
//    }
//    for (int i= 0; i<how_many; i++){
//        //            cout<<"inside for "<<endl;

//        int r = /*Random(1, how_many - i - 1)*/ rand()% how_many-i-1;
//        if (report){
//            cout<<"inside if_report "<<endl;
//            cout<<"========================================================"<<endl;
//            cout<<bpt<<endl;
//            cout<<". . . . . . . . . . . . . . . . . . . . . . . . . . . . "<<endl;
//            cout<<"deleted: "; print_array(deleted_list, deleted_size);
//            cout<<"   from: "; print_array(original, original_size);
//            cout<<endl;
//            cout<<"  REMOVING ["<<a[r]<<"]"<<endl;
//            cout<<"========================================================"<<endl;
//        }
//        bpt.remove(a[r]);


//        delete_item(a, r, size, deleted_list[deleted_size++]);
//        if (!bpt.is_valid()){
//            cout<<setw(6)<<i<<" I N V A L I D   T R E E"<<endl;
//            cout<<"Original Array: "; print_array(original, original_size);
//            cout<<"Deleted Items : "; print_array(deleted_list, deleted_size);
//            cout<<endl<<endl<<bpt<<endl<<endl;
//            return false;
//        }

//    }
//    if (report) cout<<" V A L I D    T R E E"<<endl;

//    return true;
//}




