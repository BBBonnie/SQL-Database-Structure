#ifndef MMAP_H
#define MMAP_H
#include <iostream>
#include "mpair.h"
#include"../../B+Tree/btree_plus_tree_class/bplustree_class.h"

using namespace std;
template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = nullptr):_it(it){}
        Iterator operator ++(int unused){
            Iterator hold;
            hold = _it;
            _it++;
            return hold;
        }
        Iterator operator ++(){
            _it++;
            return *this;
        }
        MPair<K, V> operator *(){
            //            assert(_it);
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it != rhs._it;
        }
        operator bool() const{
            return _it.operator bool();
        }

        //        MMap& operator +=(BPlusTree<MPair<K, V> >& list, const BPlusTree<MPair<K, V> >& addme){

        //            list.push_back(addme);


        //        } //list.push_back addme



    private:
        typename map_base::Iterator _it;
    };

    MMap(){}


    MMap(const MMap<K, V>& other){
        mmap = other.mmap;
    }


    //  Iterators
    Iterator begin(){

        return mmap.begin();

    }

    Iterator end(){

        return mmap.end();
    }

    //  Capacity
    int size() const{

    }
    bool empty() const{

    }

    //  Element Access
    const vector<V>& operator[](const K& key) const{
        return mmap.get(MPair<K,V>(key)).value_list;
    }
    vector<V>& operator[](const K& key){
        return mmap.get(MPair<K,V>(key)).value_list;
    }

    //  Modifiers
    void insert(const K& k, const V& v){
        if(contains(k)){
            get(k).push_back(v);
        }else{
            MPair<K,V>m(k,v);
            mmap.insert(m);
        }
    }
    void erase(const K& key);
    void clear();

    //  Operations:
    bool contains(const K& key){
        return mmap.contains(MPair<K,V>(key,V()));
    }
    vector<V> &get(const K& key) const{
        MPair<K,V>temp(key,V());
        assert(mmap.contains(temp));
        return mmap.get(temp).value_list;
    }

    vector<V> &get(const K& key){
        MPair<K,V>temp(key,V());

        return mmap.get(temp).value_list;
    }

    Iterator find(const K& key);
    int count(const K& key);

    Iterator lower_bound(const K & key){

        //        if(contains(key)){
        //            return mmap.find(MPair<K,V>(key));
        //        }else{
        //            Iterator walker = mmap.begin();
        //            string temp = (*walker).key;
        //            while(walker&&(*walker).key <= key){
        //                temp = (*walker).key;
        //                walker++;
        //            }
        //            return walker;
        //        }


        if(contains(key)){
            return mmap.find(MPair<K,V>(key));
        }else{
            Iterator walker = mmap.begin();

            while(walker && *walker < key){

                walker++;
            }
            return walker;
        }


    }

    Iterator upper_bound(const K & key){
        //        if(contains(key)){
        //            return mmap.find(MPair<K,V>(key));
        //        }else{
        //            Iterator walker = mmap.begin();
        //            string temp = (*walker).key;
        //            while(walker &&!(temp >= key)){
        //                temp = (*walker).key;
        //                  walker++;
        //            }
        //            return walker;
        //        }

        if(contains(key)){
            return mmap.find(MPair<K,V>(key));
        }else{
            Iterator walker = mmap.begin();
            while(walker && *walker <= key){
                walker++;
            }
            return walker;
        }

    }


    //    bool is_valid();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};
#endif // MMAP_H
