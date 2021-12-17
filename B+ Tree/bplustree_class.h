#ifndef BPLUSTREE_CLASS_H
#define BPLUSTREE_CLASS_H
#include"../../../CS 8/stack_and_queue/stack.h"
#include"array_funcs.h"

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it = nullptr, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *(){                     //dereference oprator
//            assert(key_ptr < it->data_count);
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used){      //it++  return a copy
            Iterator hold;
            hold = it;
            key_ptr++;
            if(key_ptr >= it->data_count){
                it = it->next;
                key_ptr = 0;
            }
            return hold;
//            Iterator hold(it);
//            ++it;
//            return hold;
        }

        Iterator operator++(){              //++it   return a refrence
//            if(key_ptr < it->data_count-1){
//                key_ptr++;
//            }else{
//                it = it->next;
//                key_ptr = 0;
//            }
            key_ptr++;
            return *this;

        }
        operator bool ()const{
            return (it && key_ptr < it->data_count);
        }


        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            if(lhs.it==rhs.it && lhs.key_ptr==rhs.key_ptr){
                return true;
            }else{
                return false;
            }
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs==rhs);
        }

        void print_Iterator(){
            if(it){
                print_array(it->data, it->data_count, key_ptr);
            }
        }

        bool is_null(){return !it;}

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };



    void make_tree();

    bool has_left_child(int i, BPlusTree<T> *&left) {
        if(i < 0 || i >= child_count)
            return false;
        left = subset[i-1];
        if(i-1 >= 0)
            return true;
    }

    bool has_right_child(int i, BPlusTree<T> *&right) {
        if(i < 0 || i+1 >= child_count)
            return false;
        right = subset[i+1];
        if(i+1 < child_count)
            return true;
    }


    bool helper_insert_child(BPlusTree<T> *child, int index)
    {
        insert_to_data(subset, child_count, child,index);
    }


    BPlusTree(bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, vector<BPlusTree<T> *> &s);
    bool contains(const T& entry) const;        //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    const T& get(const T& entry)const;          //return a reference to entry in the tree
    T& get_existing(const T& entry);            //return a reference to entry in the tree


    T find_next(const T& entry);


    Iterator find(const T& entry){
        BPlusTree* walker = this;

        int i = first_ge(data, data_count, entry);

        if(is_leaf()){
            if(i < data_count && data[i] == entry){
                return Iterator(this,i);
            }else{
                if(i == data_count){
                    return ++ Iterator(this,i-1);
                }
                return Iterator(this,i);
            }
        }

        if(i < data_count && data[i] == entry){
            return subset[i+1]->find(entry);
        }else{
            return subset[i]->find(entry);
        }
//        if(i < data_count && data[i] == entry){

////            cout<<"--FOUND--"<<endl;
//            return Iterator(walker, i);
//        }else if(child_count == 0){

////            cout<<"--NOT FOUND--"<<endl;
//            return nullptr;
//        }else{

//            return subset[i]->find(entry);
//        }
    }              //return an iterator to this key. NULL if not there.



    //---------------------------------------------------------------
    //      S I Z E  /  E M P T Y
    //---------------------------------------------------------------
    int size() const{
        //this function counts the number of keys in the btree


    }                          //count the number of elements in the tree


    bool empty() const{
        return (data_count ==0);
    }                        //true if the tree is empty


    //---------------------------------------------------------------
    //             P R I N T
    //---------------------------------------------------------------

    void print_tree(int level = 0,
                    ostream &outs=cout) const{
        if (is_leaf()){
            for(int i = data_count - 1; i >= 0; i--){
                outs<<setw(level*6)<<" "<<data[i]<<endl;
            }
            return;
        }

        //print the very last child:
        subset[child_count - 1]->print_tree(level + 1);

        //print items and their corresponding child from last to 0
        for(int i = data_count - 1; i >= 0; i--){
            outs<<setw(level*6)<<" "<<data[i]<<endl;
            subset[i]->print_tree(level + 1);
        }



    } //print a readable version of the tree


    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }



    //---------------------------------------------------------------
    //        I S      V A L I D
    //---------------------------------------------------------------

    bool is_valid(){
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
                BPlusTree<T>* p = subset[child_count-1];

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





    //---------------------------------------------------------------
    //        B E G I N  /  E N D
    //---------------------------------------------------------------

    Iterator begin(){
        return Iterator(get_smallest_node());
    }

    Iterator end(){
        return Iterator(nullptr);
    }

    void list_keys();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const{return child_count==0;}        //true if this is a leaf node

    T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    //insert element functions



    //------------------------------------------------
    //       L O O S E       I N S E R T
    //------------------------------------------------

    void loose_insert(const T& entry){
        /*
               int i = first_ge(data, data_count, entry);
               bool found = (i<data_count && data[i] == entry);

               three cases:
                 found
                 a. found/leaf: deal with duplicates: call +
                 b. found/no leaf: subset[i+1]->loose_insert(entry)
                                   fix_excess(i+1) if there is a need

                 ! found:
                 c. !found / leaf : insert entry in data at position i
                 c. !found / !leaf: subset[i]->loose_insert(entry)
                                    fix_excess(i) if there is a need

                    |   found          |   !found         |
              ------|------------------|------------------|-------
              leaf  |a. dups? +        | c: insert entry  |
                    |                  |    at data[i]    |
              ------|------------------|------------------|-------
                    | b.               | d.               |
                    |subset[i+1]->     | subset[i]->      |
              !leaf | loose_insert(i+1)|  loose_insert(i) |
                    |fix_excess(i+1)   | fix_excess(i)    |
                    |                  |                  |
              ------|------------------|------------------|-------
            */

        int i = first_ge(data, data_count, entry);

        //found
        if(i < data_count && data[i] == entry){

            //found && !leaf
            if(!is_leaf()){
                //                data[i] += entry;
                subset[i+1]->loose_insert(entry);
                fix_excess(i+1);
            }else{

                //found && leaf
                data[i] = data[i] + entry;
            }
            return;
        }

        if(is_leaf()){
            // !found && leaf

            //            cout<<"loose_insert()  -> is a leaf"<<endl;
            insert_item(data, i, data_count, entry);

        }else if(!is_leaf()){
            // !found && !leaf

            //            cout<<"loose_insert()  -> not a leaf"<<endl;
            subset[i]->loose_insert(entry);
            fix_excess(i);
        }
    }  //allows MAXIMUM+1 data elements in the root




    //------------------------------------------------
    //       F I X        E X C E S S
    //------------------------------------------------

    void fix_excess(int i){
        //this node's child i has one too many items: 3 steps:
        //1. add a new subset at location i+1 of this node
        //2. split subset[i] (both the subset array and the data array) and move half into
        //      subset[i+1] (this is the subset we created in step 1.)
        //3. detach the last data item of subset[i] and bring it and insert
        //      it into this node's data[]
        //
        //Note that this last step may cause this node to have too many item.
        //  This is OK. This will be
        //  dealt with at the higher recursive level. (my parent will fix it!)
        //
        //NOTE: ODD/EVENNESS
        // when there is an excess, data_count will always be odd
        //  and child_count will always be even.
        //  so, splitting is always the same.

        //  000 B_PLUS_TREE
        //  if (subset[i]->is_leaf())
        //  transfer the middle entry to the right and...
        //  Deal with next pointers. just like linked list insert

        /*fix_excess: when you split, insert middle item up to  data[i].
 *                  [20]
 *                /      \              + 25
 *           [0|10] -> [20|30] ->|||
 *
 *                   [20]
 *                 /      \
 *           [0|10] -> [20|25|30] ->|||

 *
 *    insert 25: split [20, 25, 30]: COPY middle item up to data[i]
 *                     [20|25]
 *                /       |      \
 *           [0|10] -> [20|25] -> [30] ->|||
 *
 *     is_leaf(): Detach the last item (25) from the left subset (subset[i]) and insert it
 *                into the left of subset[i+1]
 *
 *                     [20|25]
 *                /       |      \
 *           [0|10] ->  [20] -> [25|30] ->|||
 */

        //        if(subset[i]->data_count == 0)
        //            return;

        if(subset[i]->data_count > MAXIMUM){
            //1. add a new subset at location i+1
            insert_item(subset, i + 1, child_count, new BPlusTree<T>());

            //2. split subset[i] data
            split(subset[i]->data, subset[i]->data_count,
                  subset[i+1]->data, subset[i+1]->data_count);
            //split subset[i] subset
            split(subset[i]->subset, subset[i]->child_count,
                  subset[i+1]->subset, subset[i+1]->child_count);



            T item;
            //3. detach the last data item of subset[i]
            detach_item(subset[i]->data, subset[i]->data_count, item);

            //4. insert it into this node's data[]
            insert_item(data, i, data_count, item);



            //            subset[i]->next = subset[i+1];

            //5. if subset[i] is a leaf
            if(subset[i]->is_leaf()){
                ordered_insert(subset[i+1]->data, subset[i+1]->data_count, item);

                //Deal with next pointers
                subset[i+1]->next = subset[i]->next;
                subset[i]->next = subset[i+1];
            }
        }
        else
            return;
    }             //fix excess of data elements in child i

    //remove element functions:


    //------------------------------------------------
    //       L O O S E     R E M O V E
    //------------------------------------------------


    void loose_remove(const T& entry){
        /* four cases:
                   leaves:
                        a. not found: there is nothing to do
                        b. found    : just remove the target
                   non leaf:
                        c. not found: subset[i]->loose_remove, fix_shortage(i)
                        d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                            (No More remove_biggest)

                     |   !found               |   found                 |
               ------|------------------------|-------------------------|-------
               leaf  |  a: nothing            | b: delete               |
                     |     to do              |    target               |
               ------|------------------------|-------------------------|-------
               !leaf | c:                     | d: B_PLUS_TREE          |
                     |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
                     |  fix_shortage(i)       | fix_shortage(i+1) [...] |
               ------|------------------------|-------------------------|-------


             */

        //        cout<<"loose_remove got called"<<endl;


        //        int index;

        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);

        T item;
        int hold;
        T smallest = entry;

        if (is_leaf()){
            if(!found){
                //[a.] nothing to do
                //entry not in this tree
                return;
            }else{
                //[b.] just delete the item
                //   my parent will take care of my shortage


                cout<<endl<<"====inside leaf && found====="<<endl;

                //                T item;
                delete_item(data, i, data_count, item);
                //                insert_item(data,data_count,i,subset[i+1]->data);
            }

        }else{
            //not leaf:
            // ---- 000 B_PLUS_TREE: no more remove_biggest
            if (found){
            //[c.] Not here: subset[i]->loose_remove
            //not found yet. search in the next level:

            assert(i < child_count-1);

//            cout<<"i: "<<i<<endl;
//            cout<<"child_count: "<<child_count<<endl;

            subset[i+1]->loose_remove(entry);

//            if(subset[i+1]->data_count < MINIMUM){
//                fix_shortage(i+1);
//            }

//            if(!is_leaf()){
//                i = first_ge(data, data_count, entry);
//                found = (i < data_count && data[i] == entry);

//                if(found){
//                    subset[i+1]->get_smallest(smallest);
//                    data[i] = smallest;
//                }else{
//                    hold = first_ge(subset[i]->data, subset[i]->data_count, entry);
//                    found = (hold < subset[i]->data_count && subset[i]->data[hold] == entry);

//                    if(found){
//                        subset[i]->subset[hold+1]->get_smallest(smallest);
//                        subset[i]->data[hold] = smallest;
//                    }
//                }

//            }


            //            }else{
            //not leaf && found
            //[d.] found key in an inner node:subset[i+1]->loose_remove


//            cout<<endl<<"====inside not leaf && found====="<<endl;


//            BPlusTree<T>* fixed;

            //                cout<<"-------i: "<<i<<endl<<endl;

//            subset[i+1]->loose_remove(entry);



            BPlusTree<T>* fixed = subset[i+1]->get_smallest_node();
            if(fixed->data_count >= MINIMUM){
                data[i] = fixed->data[0];
            }else if(fixed->next != nullptr){
                data[i] = fixed->next->data[0];
            }



            if(subset[i+1]->data_count < MINIMUM)
                fix_shortage(i+1);


            }else{
                subset[i]->loose_remove(entry);

                if(subset[i]->data_count < MINIMUM)
                fix_shortage(i);
            }
        }


            //                BPlusTree<T>* fixed;
            //                subset[i+1]->loose_remove(entry);
            //                if(!subset[i+1]->empty())
            //                    subset[i+1]->get_smallest(data[i]);
            //                if(subset[i+1]->data_count < MINIMUM){
            //                    fixed = fix_shortage(i+1);
            //                }

            //                //--------------
            //                index = find_in_arr(data, data_count, entry);
            //                if(index != -1){
            //                    //-----  FOUND   ------
            //                    subset[i+1]->get_smallest(data[i]);

            //                }else{
            //                    //----- NOT FOUND -----
            //                    index = find_in_arr(subset[i]->data, subset[i]->data_count,entry);
            //                    if(index != -1)
            //                        subset[i+1] -> get_smallest(subset[i]->data[index]);
            //                }


            //remember. it's possible that data[i] will be gone by the time
            //      fix_shortage returns.
            //key was removed from subset[i+1]:
            //  1. shortage: find entry in data or subset[i+1]->data
            //              and replace it with subset[i+1]->smallest
            //  2. no shortage: replace data[i] with subset[i+1]->smallest
            //            }
            // --------------------------------------------------------

        //        if(subset[i]->data_count < MINIMUM)
        //            fix_shortage(i);
    }  //allows MINIMUM-1 data elements in the root





    BPlusTree<T>* fix_shortage(int i){
        /*
            * fix shortage in subtree i:
            * if child i+1 has more than MINIMUM,
            *          (borrow from right) transfer / rotate left(i+1)
            * elif child i-1 has more than MINIMUM,
            *          (borrow from left) transfer /rotate right(i-1)
            * elif there is a left sibling,
            *          merge with prev child: merge(i-1)
            * else
            *          merge with next (right) child: merge(i)
            *
            *
            * returns a pointer to the fixed_subset
            */

        cout<<"fix_shorage got called"<<endl;


        //        BPlusTree<T>* fix_ptr = subset[i];

        //        BPlusTree<T>* left_child;
        //        BPlusTree<T>* right_child;
        //        bool left = has_left_child(i,left_child);
        //        bool right = has_right_child(i, right_child);

        //case 1
        //        if(left && subset[i-1]->data_count > MINIMUM/*i > 0 && subset[i - 1]->data_count > MINIMUM*/){
        if(i>0 && subset[i-1]->data_count > MINIMUM){
            cout<<"--transfer right HERE--"<<endl;
            transfer_right(i-1);
            return subset[i]; //return a pointer to the fixed subset.

            //case 2
        }
        //            else if(right && subset[i+1]->data_count > MINIMUM/*i < child_count - 1 && i < MAXIMUM
        //                          && subset[i + 1]->data_count > MINIMUM && child_count > 0*/){


        else if(i < child_count-1 && subset[i+1]->data_count > MINIMUM){
            cout<<"--transfer left HERE--"<<endl;
            transfer_left(i+1);
            return subset[i]; //return a pointer to the fixed subset.
            //case 3
        }else if(i > 0){
            cout<<"--merge left HERE--"<<endl;
            /*fix_ptr = */merge_with_next_subset(i-1);
            cout<<"*subset[i-1]->data: "<<*subset[i-1]->data<<endl<<endl;
            return subset[i-1]; //return a pointer to the fixed subset.
            //case 4
        }else{
            cout<<"--merge right HERE--"<<endl;
            /* fix_ptr =*/ merge_with_next_subset(i);
            return subset[i]; //return a pointer to the fixed subset.
        }



    }  //fix shortage of data elements in child i
    // and return the smallest key in this subtree




    BPlusTree<T>* get_smallest_node(){
        BPlusTree<T>* walker = this; //subset[0];
        //        if(data_count == 0)
        //            return nullptr;
        while(!walker->is_leaf()){
            //            cout<<"print array in get smallest"<<endl;
            print_array(walker->data,walker->data_count);
            walker = walker->subset[0];
        }
        return walker;
    }

    T get_smallest(){
        if(is_leaf()){
            // return the smallest(leftmost) key,
            //            assert(data_count > 0);
            return data[0];
        }else{
            //------   recurse down  a level -----
            assert(child_count > 0);
            return subset[0]->get_smallest();
        }
    }


    void get_smallest(T& entry){
        //replace entry with the left_most leaf of this tree
        // Keep looking in the first subtree (recursive)
        //  until you get to a leaf.
        // Then, replace entry with data[0] of the leaf node





        entry = get_smallest();
        //        BPlusTree<T>* p = this->get_smallest_node();
        //        entry = p->data[0];




    }        //entry := leftmost leaf
    void get_biggest(T& entry){


    }         //entry := rightmost leaf



    void remove_biggest(T& entry){


        if(!child_count){
            entry = data[data_count-1];
            --data_count;
        }else{
            subset[child_count-1]->remove_biggest(entry);
            if(subset[child_count-1]->data_count < MINIMUM)
                fix_shortage(child_count-1);
        }

    }      //remove the biggest child of this tree->entry



    void transfer_left(int i){
        /*
             * --------------------------------------------------------------
             * transfer_left: from subset[i] to subset[i-1]
             *      non-leaf: same as BTree
             *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
             *                  leftmost key after you borrowed goes to replace data[i-1]
             * --------------------------------------------------------------
             *
             * non-leaf keys: BTree's rotate_left
             * ---------------
             *
             * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
             * subset[i-1] has only MINIMUM - 1 entries.
             *
             * item transfers from child[i] to child [i-1]
             *
             * FIRST item of subset[i]->data moves up to data to replace data[i-1],
             * data[i-1] moves down to the RIGHT of subset[i-1]->data
             *
             *  i = 1:
             *              [50  100]
             *  [  ]         [65 75]       ....
             *   [a]      [b]  [c]  [d]
             *
             *  65 move up to replace 50 (data[i])
             *  65's child (its child 0) moves over to be the future child of 50
             *  50 moves down to the right of subset[i]->data
             *
             *               [65 100]
             *   [50]         [ 75 ]       ....
             * [a]  [b]      [c]  [d]
             *
             * ****This does not belong here:
             * last item of subset[i-1]->data moves up to data to replace data[i],
             * data[i] moves down to the RIGHT of subset[i]->data
             *
             * leaf keys:
             * -------------
             *  fix_shortage(0) calls transfer_left(1) so, i = 1
             *          [7 | 10]
             *        /    |     \
             * [( )]<=[(7) | 8]  [10]
             *
             * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
             *        [7 | 10]
             *      /    |     \
             *   [7]    [8]   [10]
             *
             * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
             *
             *        [(8) | 10]
             *      /      |     \
             *   [7]     [(8)]   [10]
             *
             */

        //non-leaf   same as BTree:

        T temp;

        if(!is_leaf()){
            if(i < child_count && subset[i + 1]->data_count > MINIMUM
                    && subset[i]->data_count == MINIMUM - 1){
                T temp = data[i];

                insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, temp);//insert itself as a child
                T temp2;
                delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp2);    //delete itself
                data[i] = temp2;

                if(subset[i + 1]->child_count > subset[i + 1]->data_count + 1){
                    BPlusTree<T>* temp3 = nullptr;
                    delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp3);       //delete i+1's child
                    insert_item(subset[i]->subset, subset[i]->child_count, subset[i]->child_count, temp3);    //insert the child to i
                }

            }

            //leaf
        }else{


            //            T hold = subset[i]->data[0];
            //            insert_item(subset[i-1]->data, subset[i-1]->data_count, subset[i-1]->data_count, hold);
            //            T temp4;

            //            delete_item(subset[i + 1]->data, 0, subset[i + 1]->child_count, temp4);
            //            data[i] = temp4;

            //            data[i-1] = subset[i]->data[0];
            temp =subset[i]->detach_from_data(0);
            subset[i-1]->helper_insert(temp);
            data[i-1] = subset[i]->data[0];


        }


    }         //transfer one element LEFT from child i




    void transfer_right(int i){
        /*
            * --------------------------------------------------------------
            * transfer right from subset[i] to subset[i+1]
            *      non-leaf: same as BTree
            *      leaves  : rotate and leave a 'trace':
            *              data[i] is replaced with subset[i+1]->data[0]
            *              after you borrowed the key,
            * --------------------------------------------------------------
            *
            * non-leaf keys: BTree's rotate_right
            * ---------------
            * (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
            * subset[i+ 1] has only MINIMUM - 1 entries.
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
            * last item of subset[i-1]->data moves up to data,
            * data[i] moves down to the RIGHT of subset[i]->data
            *
            * leaf keys:
            * -------------
            *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
            *  subset[i+1] is the one being fixed.
            *        [7 | 10]
            *      /    |     \
            * [5|6]->[7 | (8)]=>[()]
            *
            * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
            *        [7 | (10)]
            *      /    |     \
            *   [5|6]->[7]-> [8]
            *
            * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
            *
            *        [7 | 8]
            *      /    |    \
            *   [5|6]->[7]-> [8]
            *
            *
            */

        T temp;

        if(!is_leaf()){
            if(i < child_count - 1 && subset[i]->data_count > MINIMUM
                    && subset[i + 1]->data_count == MINIMUM - 1){
                T temp = data[i];
                insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);

                T temp2;
                delete_item(subset[i]->data, subset[i]->data_count - 1, subset[i]->data_count, temp2);
                data[i] = temp2;

                if(subset[i]->child_count > subset[i]->data_count - 1){
                    BPlusTree<T>* temp3 = nullptr;
                    detach_item(subset[i]->subset, subset[i]->child_count , temp3);
                    insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp3);
                }
            }

            //is leaf
        }else{


            //            assert(i+1 < child_count);
            //            temp = subset[i]->detach_from_data(subset[i]->data_count-1);
            //            subset[i+1]->helper_insert(temp);
            //            assert(subset[i+1]->data[0] == temp);
            //            //2. update parent of the transfered child
            //            data[i] = temp;



            detach_item(subset[i-1]->data,subset[i-1]->data_count,data[i-1]);
            insert_item(subset[i]->data,0,subset[i]->data_count,data[i-1]);


        }







    }         //transfer one element RIGHT from child i

    T detach_from_data(int i){
        T item = data[i];
        shift_left(data,data_count,i);
        return item;
    }

    bool helper_insert(const T &item){

        if(contains_this(item))
            return false;

        int i = first_ge(data,data_count,item);
        insert_to_data(data,data_count,item,i);
        return true;
    }


    bool contains_this(const T& k){
        for(int i=0; i<data_count; i++)
            if(data[i] == k)
                return true;
        return false;
    }

    BPlusTree<T>* merge_with_next_subset(int i){
        /*
            * ----------------------------------------------------------------------
            *  Merge subset[i] with subset [i+1] REMOVE data[i];
            *  non leaf: same as BTree
            *  leaves  : delete but do not bring down data[i]
            * ----------------------------------------------------------------------
            *
            *   1. remove data[i] from this object
            *   2. if not a leaf, append it to child[i]->data:
            *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
            *   4. Move all subset pointers from subset[i+1]->subset to
            *          right of subset[i]->subset
            *   5. delete subset[i+1] (store in a temp ptr)
            *   6. if a leaf, point subset[i]->next to temp_ptr->next
            *   6. delete temp ptr
            *
            *
            * non-leaf nodes: (same as BTree)
            * ------------------
            *  i = 0:
            *             [50  100]
            *          /      |     \
            *  [  ]         [75]       ....
            *    |         /    \
            *   [a]      [b]    [c]
            *
            *  bring down data[i], merge it with subset[i] and subset[i+1]:
            *      then, get rid of subset[i+1]
            *             [100]
            *               |
            *            [50|75]       ....
            *            /  |  \
            *         [a]  [b]  [c]
            *
            * leaf node:
            * ----------------
            * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
            *
            *  i = 0 : merge 5 and [()]
            *        [7 | 10]
            *      /    |     \
            *  [5]->   [()]->  [10]
            *
            *  Delete data[i] (7), merge subset[i] and subset[i+1]
            *      WITHOUT bringing down data[i]
            *
            *        [10]
            *      /      \
            *  [5]->      [10]
            *
            *
            * i = 1 merge 7 and [()]
            *        [7 | 10]
            *      /    |     \
            *  [5]->   [7]->  [()]
            *
            *  Delete data[i] (10), merge subset[i] and subset[i+1]
            *      WITHOUT bringing down data[i]
            *
            *        [7]
            *      /     \
            *  [5]->      [7]
            *
            */


        cout<<"merge_with_next_subset got called"<<endl;
        T hold;
        delete_item(data, i, data_count, hold);
        //        BPlusTree<T>* this_subset = subset[i];
        //        BPlusTree<T>* next_subset = subset[i+1];
        //--------



        //1. remove data[i] from this object
        //        assert(i < data_count);
        //        hold = data[i];

        //            shift_left(data, data_count, i);


        //2. if this is not a leaf, append it to child[i]->keys[]
        if(!subset[i]->is_leaf()){
            attach_item(subset[i]->data, subset[i]->data_count,hold);
        }

        //3. move all keys from child_after_i to the end of child_i
        merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);

        //4. move all children from child_after_i to the end of child_i
        merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);


        //5. if this is a leaf, point child_i's next to temp's next
        subset[i]->next = subset[i+1]->next;

        //6. delete child_after_i, store in a temp pointer
        BPlusTree<T>* temp;
        delete_item(subset,i+1,child_count,temp);




        //7. delete temp ptr
        delete temp;

        //--------
        return subset[i];


    }    //merge subset i with subset i+1

};




//---------------------------------------------------------------------
//            C T O R  /  B I G   T H R E E
//---------------------------------------------------------------------
template <class T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
    child_count(0), next(nullptr){
//    for(int i = 0; i < MAXIMUM + 2; i++){
//        subset[i] = nullptr;
//    }
    //left emtpy
}

template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):
    data_count(0), child_count(0){

    vector<BPlusTree<T>*> s;
    copy_tree(other,s);
}

template <class T>
BPlusTree<T>::~BPlusTree(){
    const bool debug = false;
    if (debug) cout<<"** destructor fired!"<<endl;
    clear_tree();
}

template <class T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    const bool debug = false;
    if (debug) cout<<"** assignment operator fired!"<<endl;
    vector<BPlusTree<T>*> s;

    copy_tree(RHS, s);
    return *this;
}


//------------------------------------------------
//          I N S E R T
//------------------------------------------------

template <class T>
void BPlusTree<T>::insert(const T& entry){
    //in order for this class to be able to keep track of the number of the keys,
    //      this function (and the functions it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values)
    //  then the success code must distinguish between inserting a new key, or
    //  adding a new key to the existing key. (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns,
    //  all the subtrees are valid
    //  btree subtrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])


    loose_insert(entry);

    if(data_count > MAXIMUM){

        BPlusTree<T>* node = new BPlusTree<T>();

        copy_array(node->data, data, node->data_count, data_count);
        copy_array(node->subset, subset, node->child_count, child_count);

        subset[0] = node;
        data_count = 0;
        child_count = 1;

        fix_excess(0);
    }

}                //insert entry into the tree
template <class T>
void BPlusTree<T>::remove(const T& entry){
    /*
         * ---------------------------------------------------------------------------------
         * Same as BTree:
         * Loose_remove the entry from this tree.
         * Shrink if you have to
         * ---------------------------------------------------------------------------------
         * once you return from loose_remove, the root (this object) may have no data and
         * only a single subset:
         * now, the tree must shrink:
         *
         * point a temporary pointer (shrink_ptr) and point it to this root's only subset
         * copy all the data and subsets of this subset into the root (through shrink_ptr)
         * now, the root contains all the data and poiners of it's old child.
         * now, simply delete shrink_ptr, and the tree has shrunk by one level.
         * Note, the root node of the tree will always be the same, it's the
         * child node we delete
         *
         *
         */
    cout<<"inside of remove"<<endl;
    loose_remove(entry);


    if (data_count < MINIMUM && child_count == 1){
        cout<<"inside of remove: if"<<endl;


        BPlusTree<T>* shrink_ptr = subset[0];

        copy_array(data, shrink_ptr->data, data_count, subset[0]->data_count); //copy data
        copy_array(subset, shrink_ptr->subset, child_count, shrink_ptr->child_count);  //copy children

        //delete shrink_ptr
        shrink_ptr->child_count = 0;
        delete shrink_ptr;
        shrink_ptr = nullptr;
    }

}               //remove entry from the tree





//---------------------------------------------------------------
//      C L E A R   /  C O P Y
//---------------------------------------------------------------


template <class T>
void BPlusTree<T>::clear_tree(){
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
}
//clear this object (delete all nodes etc.)



template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    //assumes this btree is empty. [memory leaks otherwise]
    assert(empty());
    //        vector<BPlusTree<T>*> s;
    //        copy_array(data, other.data, data_count, other.data_count);

    //        child_count = other.child_count;

    //        for(int i = child_count - 1; i >= 0; i--){
    //            subset[i] = new BPlusTree<T>();
    //            subset[i]->copy_tree(*other.subset[i]);
    //            if(subset[i]->is_leaf() && s.empty() && i <= child_count - 1){
    //                subset[i]->next = subset[i + 1];
    //                if(i == 0)
    //                    s.push_back(subset[i]);
    //            }else if(subset[i]->is_leaf() && i == child_count - 1){
    //                if(s.size()){
    //                    subset[i]->next = s.front();
    //                    s.pop_back();
    //                }
    //            }
    //        }



    clear_tree();
    //copy keys
    data_count = 0;
    for(int i=0; i<other.data_count; i++)
        insert_to_data(data, data_count, other.data[i]);

    child_count =0; //zero out child count
    //create children, and copy each children
    BPlusTree<T>* old_child;
    for(int i=0; i<other.child_count; i++)
    {
        auto new_child = new BPlusTree<T>;
        old_child->next = new_child;    //whenever a new child is inserted, old child is made to point to this new child.
        new_child->copy_tree(*(other.subset[i]));
        helper_insert_child(new_child,child_count);
        old_child = new_child;
    }
    child_count = other.child_count;




}  //copy other intxo this object

template <class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, vector<BPlusTree<T>*>& s){
    copy_array(this->data, other.data, this->data_count, other.data_count);

    child_count = other.child_count;

    for(int i = child_count - 1; i >= 0; i--){
        subset[i] = new BPlusTree<T>();
        subset[i]->copy_tree(*other.subset[i], s);
        if(subset[i]->is_leaf() && s.empty() && i < child_count - 1){
            subset[i]->next = subset[i + 1];
            if(i == 0)
                s.push_back(subset[i]);
        }else if(subset[i]->is_leaf() && i == child_count - 1){
            if(s.size()){
                subset[i]->next = s.front();
                s.pop_back();
            }
        }
    }
}

template <class T>
bool BPlusTree<T>::contains(const T& entry) const{


    int i = first_ge(data, data_count, entry);
    if(i < data_count && data[i] == entry){
//        cout<<"--CONTAINS--"<<endl;
        return true;
    }else if(child_count == 0){
//        cout<<"--NOT CONTAINS--"<<endl;
        return false;
    }else{
        //call recursive
        return subset[i]->contains(entry);
    }
}        //true if entry can be found in the array


template <class T>
T& BPlusTree<T>::get(const T& entry){
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    //        const bool debug = false;
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);
}                     //return a reference to entry in the tree

template <class T>
const T& BPlusTree<T>::get(const T& entry)const{
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);
}          //return a reference to entry in the tree
template <class T>
T& BPlusTree<T>::get_existing(const T& entry){
    /*
 * ---------------------------------------------------------------
 * This routing explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 */
    //assert that entry is not in the tree.
    //    assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if (found) //inner node
        return subset[i+1]->get(entry);
    //or just return true?
    else //not found yet...
        return subset[i]->get(entry);
}            //return a reference to entry in the tree



template <class T>
void BPlusTree<T>::make_tree(){


    BPlusTree<T>* bpt = new BPlusTree<T>();
    bpt->data_count = 1;
    bpt->data[0] = 50;
    bpt->child_count = 2;

    BPlusTree<T>* a = new BPlusTree<T>();
    BPlusTree<T>* b = new BPlusTree<T>();


    bpt->subset[0] = a;
    bpt->subset[1] = b;

    a->data_count = 1;
    b->data_count = 1;


    a->data[0] = 20;
    b->data[0] = 70;

    a->child_count = 0;
    b->child_count = 0;

    //    BPlusTree<T>* e = new BPlusTree<T>();
    //    e->data_count = 1;
    //    e->child_count = 0;
    //    e->data[0] = 12;

    //    BPlusTree<T>* f = new BPlusTree<T>();
    //    f->data_count = 1;
    //    f->child_count = 0;
    //    f->data[0] = 30;

    //    BPlusTree<T>* g = new BPlusTree<T>();
    //    g->data_count = 1;
    //    g->child_count = 0;
    //    g->data[0] = 60;
    //    //    g->data[1] = 70;

    //    BPlusTree<T>* h = new BPlusTree<T>();
    //    h->data_count = 2;
    //    h->child_count = 0;
    //    h->data[0] = 75;
    //    h->data[1] = 80;


    //    a->subset[0] = e;
    //    a->subset[1] = f;
    //    b->subset[0] = g;
    //    b->subset[1] = h;

    cout<<"--origin--"<<endl;
    bpt->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;


    cout<<"--insert 81--"<<endl;
    bpt->insert(81);
    bpt->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;

    cout<<"--insert 75--"<<endl;
    bpt->insert(75);
    bpt->print_tree();
    cout<<endl<<"---------------------------------------"<<endl;


}

template <class T>
T BPlusTree<T>::find_next(const T& entry){
    int i = first_ge(data,data_count,entry);


    T item;
    Iterator itor;

    if(i<data_count && data[i] == entry){
        if(is_leaf()){
            if(i == 0){
                itor = BPlusTree<T>::Iterator(this,i+1);
            }else{
                itor = BPlusTree<T>::Iterator(this,i);
                itor++;
            }
            item = itor.it->data[itor.key_ptr];
            return item;
        }else{
            return subset[i+1]->find_next(entry);
        }
    }else{
        if(!is_leaf()){
            return subset[i]->find_next(entry);
        }
    }
    //    return;
}

template <class T>
void BPlusTree<T>::list_keys(){
    BPlusTree<T>* p = get_smallest_node();
    cout<<"{";
    while(p != nullptr){
        //cout<<endl<<"array: "<<endl;
        print_array(p->data,p->data_count);
        p = p->next;
    }
    cout<<"}"<<endl;

}
#endif // BPLUSTREE_CLASS_H
