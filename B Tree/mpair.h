#ifndef MPAIR_H
#define MPAIR_H
#include <iostream>
using namespace std;
template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key = k;
    }

    MPair(const K& k, const V& v){
        key = k;
        value_list.push_back(v);
    }

    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list = vlist;

    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
//        outs<<"(key: "<<print_me.key;

//        for(int i = 0; i < print_me.value_list.size(); i++){
//            outs<<"   value list: "<<print_me.value_list[i]<<")";
//        }

//        return outs;

        outs << "{" <<print_me.key << ":[";
        for (auto i = print_me.value_list.begin(); i != print_me.value_list.end(); i++)
            outs << *i << ",";
        outs<<"]}";

        return outs;
    }

    MPair& operator =(const MPair<K, V>& rhs){
           key = rhs.key;
           value_list = rhs.value_list;

           return *this;
       }


    MPair<K, V>& operator += (const MPair<K, V>& other)
    {
        assert(key == other.key);
        value_list += other.value_list;
        return *this;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key == rhs.key;
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key < rhs.key;
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }

    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key >= rhs.key;
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key > rhs.key;
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){

        assert(lhs.key == rhs.key);
        return rhs;
    }
};

#endif // MPAIR_H
