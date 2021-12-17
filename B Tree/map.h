#ifndef MAP_H
#define MAP_H
#include"btree_class.h"
#include"pair.h"


template <class K, class V>
class Map
{
public:
    typedef BTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it = nullptr):_it(it){}
        Iterator operator ++(int unused){
            Iterator hold;
            hold = _it;
            _it._ptr++;
            return hold;
        }
        Iterator operator ++(){
            _it++;
            return *this;
        }
        Pair<K, V> operator *(){
            assert(_it);
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it != rhs._it;
        }
    private:
        typename map_base::Iterator _it;

    };

    Map(){
        key_count = 0;

    }
//  Iterators
    Iterator begin(){

    }
    Iterator end(){

    }

//  Capacity
    int size() const{

    }
    bool empty() const{

    }

//  Element Access
    V& operator[](const K& key){
        return _map.get(Pair<K,V>(key,V())).value;
    }


    const V& operator[](const K& key) const{
        return _map.get(Pair<K,V>(key,V())).value;
    }


    V& at(const K& key){
        return _map.get(Pair<K,V>(key,V())).value;
    }
    const V& at(const K& key) const{
        return _map.get(Pair<K,V>(key,V())).value;
    }


//  Modifiers
    void insert(const K& k, const V& v){
        _map.insert(Pair<K,V>(k,v));
        key_count = _map.size();
    }
    void erase(const K& key){
        V hold = V();
        _map.remove(Pair<K,V>(key,hold));

    }
    void clear(){
        _map.clear_tree();
    }
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const{
        if(_map.contains(target))
            return true;
        return false;
    }


    bool is_valid(){return _map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me._map<<endl;
        return outs;
    }
private:
    int key_count;
    BTree<Pair<K, V> > _map;
};
#endif // MAP_H
