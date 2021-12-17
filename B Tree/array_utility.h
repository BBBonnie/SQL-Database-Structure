#ifndef ARRAY_UTILITY_H
#define ARRAY_UTILITY_H
#include <iostream>
#include <vector>
using namespace std;

template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap(T& a, T& b);  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry);        //insert entry into the sorted array
//data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry);   //return the first element in data that is
//not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void clear_array(T data[ ], int& n);


template <class T>
void detach_item(T data[ ], int& n, T& entry);          //remove the last element in data and place
//it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[], int& n1, T data2[ ], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   //move n/2 elements from the right of data1
//and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

template <class T>
void shift_left(T*& a, int& size, int index);

template <class T>
void shift_right(T*& a, int& size, int index);
template <class T>
void insert_to_data(T*& data, int& size, T item, int i=-1);


//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme




template <class T>
void shift_left(T*& a, int& size, int index){
    int i;
    for(i = index+1; i<size; ++i){
        a[i-1] = a[i];
    }
    size --;
}

template <class T>
void shift_right(T*& a, int& size, int index){

    if(index == size){
        T* new_a = new T[size+1];
        for(int i =0; i<size; i++)
            new_a[i] = a[i];
        a = new_a;
        size++;
        return;
    }else if(index == (size-1)){
        shift_right(a,size,size);
        a[size-1] = a[size-2];
        return;
    }
    int i;
    for(i=size-1; i>=index; --i){
        a[i+1] = a[i];
    }

    size ++;
}


template <class T>
void insert_to_data(T*& data, int& size, T item, int i){
    //the last item
    if(i==-1){
        shift_right(data, size, size);
        data[size-1] = item;
    }else{
        shift_right(data, size, i);
        data[i] = item;
    }
}


template <class T>
void clear_array(T data[ ], int& n){

    for(int i = n; i > 0; i++){
        delete_item(data,i,n,data[i]);
    }

    //    n = 0;
}


template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){

    entry = data[i];
    for(int j = i; j < n - 1; j++){
        data[j] = data[j+1];
    }
    n--;
}   //delete item at index i and place it in entry



template <class T>
T maximal(const T& a, const T& b){
    if(a > b)
        return a;
    return b;
}                    //return the larger of the two items

template <class T>
void swap(T& a, T& b){
    T temp = b;
    b = a;
    a = temp;
}  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    int max = 0;
    for(int i = 0; i < n; i++){
        if(data[max] < data[i]){
            max = i;
        }
    }
    return max;
}                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    //    int hold = 0;
    //    for(int i = 0; i < n; i++){
    //        if(entry < data[i]){
    //            if(entry > data[i-1]){
    //                hold = i;
    //                data[i+1] = data[i];
    //            }
    //        }
    //    }
    //    data[hold] = entry;

    int i = first_ge(data, n, entry);
    insert_item(data, i, n, entry);


}//insert entry into the sorted array
//data with length n

template <class T>
int first_ge(const T data[], int n, const T& entry){
    for(int i = 0; i < n; i++){
        if(data[i] >= entry){
            return i;
        }
    }
    return n;
}   //return the first element in data that is
//not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data[n-1] = entry;
    n++;
}    //append entry to the right of data

template <class T>
void insert_item(T data[], int i, int& n, T entry){
    for(int j = n; j > i; j--){
        data[j]=data[j-1];
    }
    data[i] = entry;
    n++;

}    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    entry = data[n - 1];
    n--;
}          //remove the last element in data and place
//it in entry


template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    for(int i = n1, j = 0; j < n2; )
        for(int i = 0; i<n2; i++){
            data1[n1+i] =data2[i];
        }
    n1 +=n2;
    //    for(int i=0; i<n2; i++){
    //        insert_to_array(data1, n1, data2[i], n1); //push to the end of first array
    //    }
    //    n2 = 0;
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){

    n2 = n1 / 2;
    int j = 0;
    for(int i = n1 - n2; i < n1; i++){
        data2[j] = data1[i];
        j++;
    }
    n1 -= n2;


}   //move n/2 elements from the right of data1
//and move to data2
template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){

    for(int i = 0; i < src_size; i++){
        dest[i] = src[i];
    }

    dest_size = src_size;
}             //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos){
    cout << "[";
    for(int i = pos; i < n; i++){
        cout<< data[i] <<" " ;
    }
    cout << "*] ";
    cout << endl;

}  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item){
    for(int i = 0; i < n; i++){
        if(item > data[i]){
            return true;
        }
        return false;
    }
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    for(int i = 0; i < n; i++){
        if(item <= data[i]){
            return true;
        }
        return false;
    }
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){    
    for(int i = 0; i < list.size(); i++){
        outs << list[i] << ", ";
    }
    return outs;

} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){

    list.push_back(addme);


} //list.push_back addme








#endif // ARRAY_UTILITY_H
