#ifndef BTREE_CLASS_H
#define BTREE_CLASS_H
#include <iostream>
#include <iomanip>
#include "../../B+Tree/btree_plus_tree_class/array_funcs.h"
using namespace std;

template <class T>
class BTree
{
public:
    //    BTree();
    BTree(bool dups = false);
    //big three:
    BTree(const BTree<T>& other);
    ~BTree();
    BTree<T>& operator =(const BTree<T>& RHS);

    void make_tree();

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry) const;              //true if entry can be found in the array

    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    T& get_existing(const T& entry);                     //return a reference to entry in the tree
    const T& get_existing(const T& entry) const;
    const T& get(const T& entry) const;
    T& get(const T& entry);

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    bool is_valid();

    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }


private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count;                                //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool dups_ok;                                   //true if duplicate keys may be inserted

    bool is_leaf() const {
//        cout << "is_leaf got called" << endl;
//        cout << "child_count: " << child_count << endl;
        return child_count==0;}   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry){

        /*
             int i = first_ge(data, data_count, entry);
             bool found = (i<data_count && data[i] == entry);

             three cases:
               a. found: deal with duplicates
               ! found:
               b. leaf : insert entry in data at position i
               c. !leaf: subset[i]->loose_insert(entry)
                         fix_excess(i) if there is a need
                  |   found     |   !found        |
            ------|-------------|-----------------|-------
            leaf  |  a. Deal    | b: insert entry |
                  |     with    |    at data[i]   |
            ------|  duplicates |-----------------|-------
                  |             | d: subset[i]->  |
            !leaf |             |    loose_insert |
                  |             |    fix_excess(i)|
            ------|-------------|-----------------|-------
          */

        //        cout<<"inside loose insert"<<endl;

        int i = first_ge(data, data_count, entry);
        if(i < data_count && data[i] == entry){
            return;
        }

        if(is_leaf()){
            insert_item(data, i, data_count, entry);
        }
        else if(!is_leaf()){
            subset[i]->loose_insert(entry);
            fix_excess(i);
        }

    }              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i){
        //        T hold;
        //this node's child i has one too many items: 3 steps:
        //1. add a new subset at location i+1 of this node
        //        insert_item(subset,i+1,child_count,new BTree<T>);
        //2. split subset[i] (both the subset array and the data array) and move half into
        // subset[i+1] (this is the subset we created in step 1.)
        //        split(subset[i],subset[i]->data_count,subset[i+1],subset[i+1]->data_count);
        //        split(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);

        //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
        // //Note that this last step may cause this node to have too many items. This is OK. This will be
        // dealt with at the higher recursive level. (my parent will fix it!)

        //        detach_item(subset[i]->data,subset[i]->data_count,hold);
        //        ordered_insert(data,data_count,hold);

        if(subset[i] == nullptr){
            return; //assert
        }
        if(subset[i]->data_count == 0)
            return;

        if(subset[i]->data_count > MAXIMUM){
            insert_item(subset, i + 1, child_count, new BTree<T>());

            split(subset[i]->data, subset[i]->data_count
                  ,subset[i+1]->data, subset[i+1]->data_count);

            split(subset[i]->subset, subset[i]->child_count
                  ,subset[i+1]->subset, subset[i+1]->child_count);

            T item;
            detach_item(subset[i]->data, subset[i]->data_count, item);
            insert_item(data, i, data_count, item);

        }
        else
            return;

    }                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry){
        /* four cases:
                 a. leaf && not found target: there is nothing to do
                 b. leaf && found target: just remove the target
                 c. not leaf and not found target: recursive call to loose_remove
                 d. not leaf and found: replace target with largest child of subset[i]

                    |   !found    |   found       |
              ------|-------------|---------------|-------
              leaf  |  a: nothing | b: delete     |
                    |     to do   |    target     |
              ------|-------------|---------------|-------
              !leaf | c: loose_   | d: replace    |
                    |    remove   |    w/ biggest |
              ------|-------------|---------------|-------

            */

        int i = first_ge(data, data_count, entry);

        T temp;

        if(i < data_count && data[i] == entry){
            if(is_leaf()){
                //case b:
                // found && leaf
                //delete target
                delete_item(data, i, data_count, temp);

                return;
            }else{
                //case d:
                //remove the biggest
                //replace with the number

                subset[i]->remove_biggest(temp);

                data[i] = temp;

                if(!is_leaf() && subset[i]->data_count < MINIMUM)

                    fix_shortage(i);
                return;
            }
        }else{
            if(is_leaf())
                //case a:
                // !found && leaf
                // do nothing
                return;
            else{
                //case c:
                // !found && !leaf
                //recursive
                //fix_shortage
                subset[i]->loose_remove(entry);

                if(subset[i]->data_count < MINIMUM)
                    fix_shortage(i);
            }
        }
    }              //allows MINIMUM-1 data elements in the root


    void fix_shortage(int i){
        /*
             * fix shortage in subtree i:
             * if child i+1 has more than MINIMUM, rotate left
             * elif child i-1 has more than MINIMUM, rotate right
             * elif there is a right child, merge child i with next child
             * else merge child i with left child
             */
        if(i < child_count - 1 && i < MAXIMUM
                && subset[i + 1]->data_count > MINIMUM && child_count > 0){
            cout << "--HERE rotate_left--"<<endl;
            rotate_left(i);
        }else if(i > 0 && subset[i - 1]->data_count > MINIMUM){
            cout << "--HERE rotate_right--"<<endl;
            rotate_right(i - 1);
        }else if(i == 0){
            cout << "--HERE merge i-- i: "<<i<<endl;
            merge_with_next_subset(i);  //merge i with the next
        }else{
            cout << "--HERE merge i-1-- i: "<<i<<endl;
            merge_with_next_subset(i - 1);  //merge i with the left
        }
    }                      //fix shortage of data elements in child i

    void remove_biggest(T& entry){
        // Keep looking in the last subtree (recursive)
        //  until you get to a leaf.
        // Then, detach the last (biggest) data item
        // after the recursive call, fix shortage.
        if(is_leaf()){
            detach_item(data, data_count, entry);     //when get to a leaf, detach
            //        return;
        }else{
            subset[child_count - 1]->remove_biggest(entry); //keep searching biggest

        }
        if(!is_leaf() && subset[child_count - 1]->data_count < MINIMUM){  //when not leaf and nothing in subset
            //fix
            fix_shortage(child_count - 1);
        }
    }                  //remove the biggest child of this tree->entry


    void rotate_left(int i){
        /*
            * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
            * subset[i-1] has only MINIMUM - 1 entries.
            *
            * item transfers from child[i] to child [i-1]
            *
            * FIRST item of subset[i]->data moves up to data to replace data[i-1],
            * data[i-1] moves down to the RIGHT of subset[i-1]->data
            *
            *  i = 1:
            *              [50 100]
            *  [  ]        [65 75]       ....
            *            [a]  [b]  [c]
            *
            *  65 move up to replace 50 (data[i])
            *  65's child (its child 0) moves over to be the child of 50
            *  50 moves down to the right of subset[i]->data
            *
            *              [65 100]
            *  [50]         [ 75 ]       ....
            *     [a]      [b]  [c]
            *
            * last item of subset[i-1]->data moves up to data,
            * data[i] moves down to the RIGHT of subset[i]->data
            */

        if(i < child_count && subset[i + 1]->data_count > MINIMUM
                && subset[i]->data_count == MINIMUM - 1){
            T temp = data[i];

            insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, temp);//insert itself as a child
            T temp2;
            delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp2);    //delete itself
            data[i] = temp2;

            if(subset[i + 1]->child_count > subset[i + 1]->data_count + 1){
                BTree<T>* temp3 = nullptr;
                delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp3);       //delete i+1's child
                insert_item(subset[i]->subset, subset[i]->child_count, subset[i]->child_count, temp3);    //insert the child to i
            }
        }
    }                        //transfer one element LEFT from child i



    void rotate_right(int i){
        /* (i < child_count - 1) and (subset[i]->data_count > MINIMUM)
             * subset[i + 1] has only MINIMUM - 1 entries.
             *
             * item transfers from child[i] to child [i+1]
             *
             * LAST item of subset[i]->data moves up to data to replace data[i],
             * data[i] moves down to the LEFT of subset[i+1]->data
             *
             * i = 1
             *                     [50 100]
             *      [20 30]        [65 75]          [ ]
             *  [..] [..] [..]   [a] [b] [c]        [..]
             *
             *  75 moves up to replace 100 (data[i])
             *  75's child (its last child) moves over to be the (child 0) child of 100
             *  100 moves down to subset[i]->data
             *
             *                     [50 75]
             *      [20 30]          [65]          [100]
             *  [..] [..] [..]     [a] [b]        [c] [..]
             *
             *
             * last item of subset[i-1]->data moves up to data,
             * data[i] moves down to the RIGHT of subset[i]->data
             */

        if(i < child_count - 1 && subset[i]->data_count > MINIMUM
                && subset[i + 1]->data_count == MINIMUM - 1){
            T temp = data[i];
            insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);

            T temp2;
            delete_item(subset[i]->data, subset[i]->data_count - 1, subset[i]->data_count, temp2);
            data[i] = temp2;

            if(subset[i]->child_count > subset[i]->data_count - 1){
                BTree<T>* temp3 = nullptr;
                detach_item(subset[i]->subset, subset[i]->child_count , temp3);
                insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp3);
            }
        }

    }                      //transfer one element RIGHT from child i
    void merge_with_next_subset(int i){
        /*
            *  Merge subset[i] with subset [i+1] with data[i] in the middle
            *
            *   1. remove data[i] from this object
            *   2. append it to child[i]->data
            *   3. Move all data items from subset[i+1]->data to subset[i]->data
            *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
            *   5. delete subset[i+1] (store in a temp ptr)
            *   6. delete temp ptr
            */

        T temp;
        delete_item(data, i, data_count, temp);  //remove data[i]
        insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, temp);  //append child

        while(subset[i + 1]->data_count > 0){
            T temp2;
            delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp2); //delete i+1->data
            insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, temp2);  //move to i->data
        }

        while(subset[i + 1]->child_count > 0){
            BTree<T>* temp_ptr = nullptr;
            delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp_ptr); //delete i+1->subset
            insert_item(subset[i]->subset, subset[i]->child_count, subset[i]->child_count, temp_ptr); //move to i->subset
        }

        BTree<T>* temp3 = nullptr;
        delete_item(subset, i + 1, child_count, temp3); //delete i+1
        delete temp3;   //delete temp ptr

    }             //merge subset i with subset i+1

};


//template <class T>
//BTree<T>::BTree(){
//    data_count = 0;
//    data[MAXIMUM + 1] = {};
//    child_count = 0;
//}


template <class T>
BTree<T>::BTree(bool dups){
    //    dups = false;
    //    dups_ok = dups;

    //    data_count = 0;
    //    child_count = 0;

    //    for(int i = 0; i < MAXIMUM+1; i++){
    //        subset[i] = nullptr;
    //    }

    dups_ok = dups;
    data_count = 0;
    child_count = 0;
    for(int i = 0; i < MAXIMUM+2; i++){
        //        data[i] = T();
        subset[i] = nullptr;
    }
    //    subset[MAXIMUM +1] = nullptr;

}

template <class T>
BTree<T>::BTree(const BTree<T>& other){
    copy_tree(other);
}

template <class T>
BTree<T>::~BTree(){
    clear_tree();
}

template <class T>
BTree<T>& BTree<T>::operator =(const BTree<T>& RHS){
    if(this != &RHS){
        clear_tree();
        copy_tree(RHS.data);
    }
    return *this;

}


template <class T>
void BTree<T>::insert(const T& entry){
    //in order for this class to be able to keep track of the number of the keys, this function (and the functions
    //  it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values) then the success
    //  code must distinguish between inserting a new key, or adding a new key to the existing key.
    //  (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns, all the subtrees are valid
    //  btree subtrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])

    //    cout<<"inside insert"<<endl;


    loose_insert(entry);
    //    cout<<"data_count: "<<data_count<<endl;
    //    cout<<"MAX: "<<MAXIMUM<<endl;
    if(data_count > MAXIMUM){
        //        cout<<"inside data_count >MAXIMUM"<<endl;
        BTree* node = new BTree<T>();

        copy_array(node->data,data,node->data_count,data_count);     //copy data
        copy_array(node->subset,subset,node->child_count,child_count);  //copy children


        subset[0] = node;
        data_count = 0;
        child_count = 1;

        //                child_count ++;
        fix_excess(0);
    }                //insert entry into the tree
}

template <class T>//insert entry into the tree
void BTree<T>::remove(const T& entry){
    //Loose_remove the entry from this tree.
    //once you return from loose_remove, the root (this object) may have no data and only a single subset
    //now, the tree must shrink:
    //  point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the child node we delete


    loose_remove(entry);

    if(data_count < MINIMUM && child_count == data_count + 1){ //have no data and only one child
        BTree<T>* shrink_ptr = nullptr;
        shrink_ptr = subset[0];   // point to the only subset

        copy_array(data, shrink_ptr->data, data_count, subset[0]->data_count); //copy data
        copy_array(subset, shrink_ptr->subset, child_count, shrink_ptr->child_count);  //copy children

        //delete shrink_ptr
        shrink_ptr->child_count = 0;
        delete shrink_ptr;
        shrink_ptr = nullptr;
    }
}               //remove entry from the tree

template <class T>
void BTree<T>::clear_tree(){
    if(empty())
        return;

    //if child is leaf, delete all children
    if(child_count && subset[0]->is_leaf()){
        data_count = 0;
        for(int i = 0; i < child_count; i++)
            delete subset[i];
        child_count = 0;
        return;
    }
    //else, let children clear(recursion)
    for(int i=0; i<child_count; i++)
        subset[i]->clear_tree();
}

template <class T>//clear this object (delete all nodes etc.)
void BTree<T>::copy_tree(const BTree<T>& other){

    clear_tree();

    copy_array(data,other.data,data_count,other.data_count);

    copy_array(subset,other.subset,child_count,other.child_count);
}     //copy other into this object


template <class T>
bool BTree<T>::contains(const T& entry) const{

    int i = first_ge(data, data_count, entry);
    if(i < data_count && data[i] == entry){
        return true;
    }
    else if(child_count == 0){
        return false;
    }else{
        return subset[i]->contains(entry);
    }
}

template <class T>//true if entry can be found in the array
T& BTree<T>::get_existing(const T& entry){
    //If entry is not in the tree, ASSERT
    //assert(contains(entry));

//    cout<<"get_existing being called"<<endl;



    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (found)
        return data[i];
    //    if (is_leaf()){
    //        cout<<"get called with non-existent entry: "<<entry<<endl;
    //        insert(entry);
    //        return get(entry);

    //        //exit(-1);
    //    }
    if (!found){  //yet...
        if (!is_leaf()){
            return subset[i]->get_existing(entry);
        }else{ //leaf
            assert(found);
        }
    }
}

template <class T>
const T& BTree<T>::get_existing(const T& entry) const{

    cout<<"CONST get_existing being called"<<endl;

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (found)
        return data[i];
    //    if (is_leaf()){
    //        cout<<"get called with non-existent entry: "<<entry<<endl;
    //        insert(entry);
    //        return get(entry);

    //        //exit(-1);
    //    }
    if (!found){  //yet...
        if (!is_leaf()){
            return subset[i]->get_existing(entry);
        }else{ //leaf
            assert(found);
        }
    }
}


template <class T>
const T& BTree<T>::get(const T& entry) const{


    cout<<"get being called"<<endl;


    //    int i = first_ge(data, data_count, entry);
    //    bool found = (i<data_count && data[i]==entry);
    //    if (found)
    //        return data[i];
    //    //    if (is_leaf()){
    //    //        cout<<"get called with non-existent entry: "<<entry<<endl;
    //    //        insert(entry);
    //    //        return get(entry);

    //    //        //exit(-1);
    //    //    }
    //    if (!found)  //yet...
    //        if (!is_leaf()){
    //            return subset[i]->get_existing(entry);
    //        }else{ //leaf
    //            assert(found);
    //        }

    return get_existing(entry);
}
template <class T>
T& BTree<T>::get(const T& entry){

//    cout<<"CONST get being called"<<endl;

    //if item does not exsit
    //insert it
    //return get_existing(item)
    //    int i = first_ge(data, data_count, entry);
    //    bool found = (i<data_count && data[i]==entry);
    //    if (!found){

    if (!contains(entry))
        insert(entry);

    //        if (is_leaf()){
    //            cout<<"get called with non-existent entry: "<<entry<<endl;
    //            insert(entry);
    //            return get(entry);
    //        }else
    return get_existing(entry);
    //    }

}


template <class T>//return a reference to entry in the tree
T* BTree<T>::find(const T& entry){

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);

    if(found){
        return &data[i];
    }else{
        if(!is_leaf())
            return  subset[i]->find(entry);
    }
    return nullptr;

}                    //return a pointer to this key. NULL if not there.

template <class T>
int BTree<T>::size() const{
    return data_count;
}


template <class T>
bool BTree<T>::empty() const{
    if(data_count == 0){
        return true;
    }else{
        return false;
    }
}                         //true if the tree is empty

template <class T>
void BTree<T>::print_tree(int level, ostream &outs) const{
    //    cout<<"---inside print---"<<endl;

    if (is_leaf()){
        for(int i = data_count - 1; i >= 0; i--)
            outs << setw(level * 6) << "[" << data[i] <<"]"<< endl;
        return;
    }

    //print the last child:
    subset[child_count - 1]->print_tree(level + 1);

    //print items and their children from last one to 0
    for(int i = data_count - 1; i >= 0; i--){
        cout << setw(level * 6) << "["<< data[i] <<"]"<< endl;
        subset[i]->print_tree(level + 1);
    }
}


template <class T>
bool BTree<T>::is_valid(/*bool is_root*/){
//    if(empty()){
//        return true;
//    }

    //    if(!is_root){

//    cout<<"inside is_valid"<<endl;


    if(data_count < MINIMUM){
//        cout<<"--too few datas--"<<endl;
        return false;
    }else if(child_count < MINIMUM + 1 && !is_leaf()){
//        cout<<"--too few children--"<<endl;
//        cout << "child_count" << child_count << endl;
        return false;
    }else if(child_count != data_count + 1 && !is_leaf()){
//        cout<<"--children is not data+1--"<<endl;
//        cout << *this << endl;
        return false;
    }else if(data_count > MAXIMUM){
//        cout<<"--too many datas--"<<endl;
        return false;
    }else if(child_count > MAXIMUM + 1){
//        cout<<"--too many children--"<<endl;
        return false;
    }

//    cout << "after if else statements in is_valid" << endl;

    //check order (is !sorted)
    if(!is_leaf())
    {
        for(int i = 0; i < data_count-1; i++){
//            cout<<"--data not sorted--"<<endl;
            if(data[i] > data[i+1]) {
//                cout << "data[i]: " << data[i] << "data[i+1]: " << data[i + 1] << endl;
                return false;
            }
        }
    }


    if(!is_leaf()){
        //check biggest
        for(int i = 0; i < data_count; i++){
            BTree<T>* p = subset[child_count-1];

            if(data[i] >= p->data[p->data_count - 1]){
//                cout<<"--right child is not the biggest--"<<endl;
//                cout<<"data[i]: "<< data[i] << endl;
//                cout<<"subset: "<< endl << *subset[child_count - 1] <<endl;
//                cout<<"subset[i]->data[data_count - 1]: " << p->data[p->data_count - 1] << endl;
                return false;
            }

            if(data[data_count - 1] <= subset[data_count - 1]->data[0]){
//                cout << "--left chiled is bigger than the 1st of the subset" << endl;
//                cout << "data[data_count - 1]: " << data[data_count - 1] << endl;
//                cout << "subset[data_count - 1]->data[0]: " << subset[data_count - 1]->data[0] << endl;
                return false;
            }
        }

        if(subset[child_count - 1]->data[0] <= data[data_count - 1]){
//            cout<<"--last left child is not the biggest--"<<endl;
            return false;
        }

        //call recursion
        for(int i = 0; i < child_count; i++){
//            cout << "for loop of recursion got called: " << i << endl;
            if(!subset[i]->is_valid(/*false*/)){
//                cout << "recursion got called: " << i << endl;
                return false;
            }
        }

    }// if !is_leaf






    //check counts

    //    if(data_count < MAXIMUM){
    //        cout<<"--too few datas--"<<endl;
    //        return false;
    //    }
    //    if(child_count > MAXIMUM + 1){
    //        cout<<"--too many children--"<<endl;
    //        return false;
    //    }
    //    if(child_count < MAXIMUM + 1){
    //        cout<<"--too few children--"<<endl;
    //        return false;
    //    }


    //    if(is_leaf()){
    //        return true;
    //    }

//    cout << "\n\nend of is_valid" << endl;
    return true;
}

template <class T>
void BTree<T>::make_tree(){
    BTree<T>* btree = new BTree<T>();
    BTree<T>* btree2 = new BTree<T>();
    //    BTree<T>* walker = nullptr;

    btree->data_count = 2;
    btree->data[0] = 50;
    btree->data[1] = 81;
    btree->child_count = 3;


    BTree<T>* a = new BTree<T>();
    BTree<T>* b = new BTree<T>();
    BTree<T>* c = new BTree<T>();

    btree->subset[0] = a;
    btree->subset[1] = b;
    btree->subset[2] = c;

    a->data_count = 1;
    b->data_count = 1;
    c->data_count = 1;

    a->data[0] = 20;
    //    a->data[1] = 50;
    b->data[0] = 70;
    //    b->data[1] = 100;
    c->data[0] = 100;


    //     walker = btree.find(95);
    //     cout<<"walker: "<<*walker<<endl;


    a->child_count = 2;
    b->child_count = 2;
    c->child_count = 2;



    BTree<T>* e = new BTree<T>();
    e->data_count = 1;
    e->child_count = 0;
    e->data[0] = 12;

    BTree<T>* f = new BTree<T>();
    f->data_count = 1;
    f->child_count = 0;
    f->data[0] = 30;

    BTree<T>* g = new BTree<T>();
    g->data_count = 1;
    g->child_count = 0;
    g->data[0] = 60;
    //    g->data[1] = 70;

    BTree<T>* h = new BTree<T>();
    h->data_count = 2;
    h->child_count = 0;
    h->data[0] = 75;
    h->data[1] = 80;

    BTree<T>* i = new BTree<T>();
    i->data_count = 1;
    i->child_count = 0;
    i->data[0] = 90;

    BTree<T>* j = new BTree<T>();
    j->data_count = 1;
    j->child_count = 0;
    j->data[0] = 110;

    a->subset[0] = e;
    a->subset[1] = f;
    b->subset[0] = g;
    b->subset[1] = h;
    c->subset[0] = i;
    c->subset[1] = j;



    //    cout<<"--before print--"<<endl;

    cout<<"--origin--"<<endl;
    btree->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;


    //    cout<<"test for = :"<<endl;
    //    btree2 = btree;
    //    btree2->print_tree();
    //    cout<<endl<<"---------------------------------------"<<endl;


    //    cout<<"test for clear :"<<endl;
    //    btree2->clear_tree();
    //    btree2->print_tree();
    //    cout<<endl<<"---------------------------------------"<<endl;


    //    cout<<"================================"<<endl;


    //    cout<<"--insert 80--"<<endl;
    //    btree->insert(80);
    //    btree->print_tree();
    //    cout<<endl<<"---------------------------------------"<<endl;

    //    cout<<"--insert 91--"<<endl;
    //    btree->insert(91);
    //    btree->print_tree();
    //    cout<<endl<<"---------------------------------------"<<endl;


    //    cout<<"--insert 109--"<<endl;
    //    btree->insert(109);
    //    btree->print_tree();

    //    cout<<endl<<"---------------------------------------"<<endl;


    //    cout<<"--insert 108--"<<endl;
    //    btree->insert(108);
    //    btree->print_tree();
    //    cout<<endl<<"---------------------------------------"<<endl;


    cout<<"--remove 50--"<<endl;
    btree->remove(50);
    btree->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;

    cout<<"--remove 81--"<<endl;
    btree->remove(81);
    btree->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;


    cout<<"--remove 100--"<<endl;
    btree->remove(100);
    btree->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;


    //    cout<<a->data[0];


    //        if(b->contains(75) == true){
    //            cout<<"contains"<<endl;
    //        }else{
    //            cout<<"do not contain"<<endl;
    //        }

}
#endif // BTREE_H
