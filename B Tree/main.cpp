#include <iostream>
#include<stdlib.h>
#include"array_utility.h"
#include"btree_class.h"
#include"../../B+Tree/btree_plus_tree_class/bplustree_class.h"
#include"map.h"
#include"mmap.h"

using namespace std;

void f(const  Map<string,int>& m);

void test_make_tree();
int Random(int lo, int hi);
void test_BTree_auto(int tree_size=5000, int how_many=500, bool report=false);
void test_auto();
bool test_btree_auto(int how_many, bool report=true);

int main()
{




//    srand(time(0));
//        cout<<endl<<endl<<endl;
////        for (int i= 0; i<50; i++){
////            cout<<Random(0,10)<<" ";
////        }


//        test_auto();

//        cout<<"------END------"<<endl<<endl;
//        return 0;
    cout<< "----------------BTree---------------"<<endl<<endl;








//    srand(time(0));

    //        test_BTree_auto(5000,500,false);
//    test_BTree_auto(100,100,true);
//            test_make_tree();

    //        Map<string,int> m;
    //        m.insert("Bob",2);
    //        m.insert("Joe",4);
    //        m.insert("Pan",6);
    //        m.insert("Li",8);

    //        cout<<m<<endl;



            MMap<string,int> mm;
            mm.insert("Bob",2);
            mm.insert("Joe",4);
            mm.insert("Pan",6);
            mm.insert("Pan",2);

            cout<<mm<<endl;

            cout<<mm["Bob"];



//            cout<<"-----------for [ ]-----------"<<endl;
//            cout<<mm["Bob"]<<endl;


//        cout<<"-----------for at-----------"<<endl;
//        cout<<m.at("Joe")<<endl;

//            cout<<"-----------for f-----------"<<endl;
//            f(m);



    //    int a[10000];
    //    int size = 0;
    //    BTree<int> b;
    //    int how_many = 100;

    //    for(int i = 0; i < how_many; i++)
    //    {
    //        int ran = Random(0, 1000);
    //        insert_item(a, i, size, ran);
    //    }
    //    for(int k = 0; k < 100; k++)
    //    {

    ////        cout<<"-------------------------------"<<endl;
    ////        cout<<"after insert: "<<endl;
    //        for(int i = 0; i < how_many; i++)
    //        {
    //            b.insert(a[i]);
    //        }

    ////        cout<<"-------------------------------"<<endl;
    ////        cout<<"after remove: "<<endl;
    //        for(int i = 0; i < how_many; i++)
    //        {
    //            b.remove(a[i]);
    //        }

    //        cout << "B: " << b << endl;
    //    }

    //    cout << b << endl;


    cout<<endl<<endl<<"---------THIS IS THE END-----------"<<endl;

    return 0;
}


void f(const  Map<string,int>& m){
    cout<<"inside f"<<endl;
    cout<<m["ap"]<<endl;
}

void test_make_tree(){

    bool b = false;
    BTree<int> btree(b);
    btree.make_tree();
}

void test_BTree_auto(int tree_size, int how_many, bool report){
    bool verified = true;
    for (int i = 0; i < how_many; i++){
        if (report){
            cout<<"*********************************************************"<<endl;
            cout<<" T E S T:    "<<i<<endl;
            cout<<"*********************************************************"<<endl;
        }
        if (!test_btree_auto(tree_size, report)){
            cout<<"T E S T :   ["<<i<<"]    F A I L E D ! ! !"<<endl;
            verified = false;
            return;
        }

    }
    cout<<"**************************************************************************"<<endl;
    cout<<"**************************************************************************"<<endl;
    cout<<"             E N D     T E S T: "<<how_many<<" tests of "<<tree_size<<" items: ";
    cout<<(verified?"VERIFIED": "VERIFICATION FAILED")<<endl;
    cout<<"**************************************************************************"<<endl;
    cout<<"**************************************************************************"<<endl;
}

bool test_btree_auto(int how_many, bool report){
    const int MAX = 10000;
        assert(how_many < MAX);
        BTree<int> bpt;
        int a[MAX];
        int original[MAX];
        int deleted_list[MAX];

        int original_size;
        int size;
        int deleted_size = 0;

        //fill a[ ]
        for (int i= 0; i< how_many; i++){
            a[i] = i;
        }
        //shuffle a[ ]: Put this in a function!
        for (int i = 0; i< how_many; i++){
            int from = /*Random(0, how_many-1)*/ rand()% how_many-1;
            int to = /*Random(0, how_many -1)*/ rand()% how_many-1;
            int temp = a[to];
            a[to] = a[from];
            a [from] = temp;
        }
        //copy  a[ ] -> original[ ]:
        copy_array(original, a, original_size, how_many);
        size = how_many;
        original_size = how_many;
        for (int i=0; i<size; i++){
            bpt.insert(a[i]);

        }
        if (report){
            cout<<"========================================================"<<endl;
            cout<<"  "<<endl;
            cout<<"========================================================"<<endl;
            cout<<bpt<<endl<<endl;
        }
        for (int i= 0; i<how_many; i++){
//            cout<<"inside for "<<endl;

            int r = /*Random(1, how_many - i - 1)*/ rand()% how_many-i-1;
            if (report){
                cout<<"inside if_report "<<endl;
                cout<<"========================================================"<<endl;
                cout<<bpt<<endl;
                cout<<". . . . . . . . . . . . . . . . . . . . . . . . . . . . "<<endl;
                cout<<"deleted: "; print_array(deleted_list, deleted_size);
                cout<<"   from: "; print_array(original, original_size);
                cout<<endl;
                cout<<"  REMOVING ["<<a[r]<<"]"<<endl;
                cout<<"========================================================"<<endl;
            }
            bpt.remove(a[r]);


            delete_item(a, r, size, deleted_list[deleted_size++]);
            if (!bpt.is_valid()){
                cout<<setw(6)<<i<<" I N V A L I D   T R E E"<<endl;
                cout<<"Original Array: "; print_array(original, original_size);
                cout<<"Deleted Items : "; print_array(deleted_list, deleted_size);
                cout<<endl<<endl<<bpt<<endl<<endl;
                return false;
            }

        }
        if (report) cout<<" V A L I D    T R E E"<<endl;

        return true;
}






//void test_BTree_auto(int tree_size, int how_many, bool report){
//    bool verified = true;
//    for (int i = 0; i< how_many; i++){
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
//    BTree<int> bpt;
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
//        int from = Random(0, how_many-1);
//        int to = Random(0, how_many -1);
//        int temp = a[to];
//        a[to] = a[from];
//        a [from] = temp;
//    }
//    //copy  a[ ] -> original[ ]:
//    copy_array(a, original, size, how_many);
//    //    copy_array()
//    //    size = how_many;
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
//        int r = Random(0, how_many - i - 1);
//        if (report){
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
//        //        if (!bpt.is_valid()){
//        //            cout<<setw(6)<<i<<" I N V A L I D   T R E E"<<endl;
//        //            cout<<"Original Array: "; print_array(original, original_size);
//        //            cout<<"Deleted Items : "; print_array(deleted_list, deleted_size);
//        //            cout<<endl<<endl<<bpt<<endl<<endl;
//        //            return false;
//        //        }

//    }
//    if (report) cout<<" V A L I D    T R E E"<<endl;

//    return true;
//}


void test_auto(/*int size, int how_many*/){

    BTree<int> bpt;


//    int arr[40] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,
//                   15,26,27,89,56,67,20,33,34,30,40,50,60,70,
//                   80,81,82,83,84,85,86,87,0,88,90,100};


    int arr[100];
    for (int i= 0; i< 100; i++){

        arr[i] = Random(0,100);
    }



    for (int i=0; i<100; i++){
        bpt.insert(arr[i]);

    }


    for(int i=0; i<100; i++){
        cout<<arr[i]<<" ,";
    }
    cout<<endl;

    cout<<bpt;

}

int Random(int lo, int hi){
    int r = rand()%(hi-lo+1)+lo;

    return r;
}
