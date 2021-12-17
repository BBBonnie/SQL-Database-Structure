#include "table.h"

Table::Table()
{
    _dbname[0] = NULL;
    _db_fields_list[0] = NULL;
    _filename[0] = NULL;
    _recno = 0;

    set_precedence();
}

Table::Table(string name){
    _dbname = name;
    string temp = name + "_db.bin";
    _db_fields_list = _dbname + "_fields.txt";
    strcpy(_filename, temp.c_str());
    strcpy(_filename_fields, _db_fields_list.c_str());

    fstream _f;
    open_fileRW(_f, _filename);

    if(file_exists(_filename_fields))
        loadFields();

    _mmapVect.clear();
    for(int i = 0; i < _field_list.size(); i++){
        MMap<string, int> mmap;
        _mmapVect.push_back(mmap);
    }

    _rec.read(_f, _recno);
    _recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);


    if(f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH) > 0)
        index_this();

    set_precedence();
    _f.close();
}

Table::Table(string name, vector<string> field_list){

    fstream _f;
    _field_list = field_list;
    for(int i = 0; i < field_list.size(); i++){
        _field_list_map.insert(field_list[i], i);
    }

    //    cout<<_field_list_map;

    _dbname = name;
    string temp = _dbname + "_db.bin";
    _db_fields_list = _dbname + "_fields.txt";

    strcpy(_filename, temp.c_str());
    strcpy(_filename_fields, _db_fields_list.c_str());

    open_fileRW(_f, _filename);

    _rec.read(_f, _recno);
    _recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);

    _mmapVect.clear();
    for(int i = 0; i < field_list.size(); i++){
        MMap<string, int> mmap;
        _mmapVect.push_back(mmap);
    }

    writeFields(field_list);

    if(f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH) > 0)
        index_this();

    _f.close();
}

void Table::insert(vector<string> fields_item){
    //    cout<<"inside of table::insert"<<endl;
    fstream _f;
    open_fileRW(_f, _filename);

    for(int i = 0; i <fields_item.size(); i++){
        strcpy(_rec.record[i], fields_item[i].c_str());
    }

    _f.seekg(0, _f.end);       //set position
    _recno = _rec.write(_f);

    if(_mmapVect.size() == 0){
        for(int i = 0; i < fields_item.size(); i++){
            MMap<string, int> mmap;
            _mmapVect.push_back(mmap);
        }
    }

    re_index(fields_item);

    _f.close();
}

void Table::index_this(){
    fstream _f;
    open_fileRW(_f, _filename);

    int file_rec_no = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);

    if(file_rec_no == 0)
        return;
    if(_mmapVect.size() == 0){
        for(int i = 0; i < _field_list.size(); i++){
            MMap<string, int> mmap;
            _mmapVect.push_back(mmap);
        }
    }

    for(int i = 0; i < file_rec_no; i++){
        _rec.read(_f, i);

        for(int j = 0; j < _field_list.size(); j++){
            _mmapVect[j].insert(_rec.record[j], i);
        }
    }
    _f.close();
}

void Table::re_index(vector<string>& entries){
    if(entries.size() == 0)
        return;
    if(_mmapVect.size() == 0){
        for(int i = 0; i < _field_list.size(); i++){
            MMap<string, int> mmap;
            _mmapVect.push_back(mmap);
        }
    }

    for(int i = 0; i < _field_list.size(); i++){
        //        cout<<"_recno: "<<_recno<<endl;
        _mmapVect[i].insert(entries[i], _recno);
    }
}

Table Table::create(string name, vector<string> fields_list){
    fstream _f;
    _field_list = fields_list;

    _dbname = name + "_db.txt";
    _db_fields_list = _dbname  + "_fields.txt";

    strcpy(_filename, _dbname.c_str());

    open_fileRW(_f, _filename);
    _rec.read(_f, _recno);
    _recno = f_size(_f, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);
    _mmapVect.clear();

    for(int i = 0; i < fields_list.size(); i++){
        MMap<string, int> mmap;
        _mmapVect.push_back(mmap);
    }

    index_this();
    _f.close();
    return *this;
}

Table Table::select(vector<string>& fields_list){

    //    cout<<"===inside of table::select()================"<<endl;
    if(fields_list[0] == "*"){
        Table temp(*this);

        return temp;
    }else{

        vector<int> v;
        vector<string> v_str;

        for(int i = 0; i < _field_list.size(); i++){
            for(int j = 0; j < fields_list.size(); j++){
                if(fields_list.at(j) == _field_list.at(i))
                    v.push_back(i);
            }
        }

        fstream _f;
        fstream _f_source;
        int recno = 0;
        Record f_temp(recno);

        string new_db_name = _dbname + to_string(rand());
        string f_temp_name = new_db_name + "_db.txt";

        Table temp(new_db_name, fields_list);

        open_fileRW(_f, f_temp_name.c_str());
        open_fileRW(_f_source, _filename);

        _f_source.seekg(0, _f_source.end); //set position

        recno = f_size(_f_source, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);

        for(int i = 0; i < recno; i++){
            _rec.read(_f_source, i);
            for(int j = 0; j < v.size(); j++){
                v_str.push_back(_rec.record[v[j]]);
            }
            temp.insert(v_str);
            v_str.clear();
        }


        cout<<f_temp<<endl;
        f_temp.write(_f_source);



        //        cout<<"------selected table: "<<temp<<endl;

        return temp;
    }

}

Table Table::select(vector<string> &fields_list, vector<string> &conditions){

    //        cout<<"===inside of table::select()================"<<endl;


    vector<int> v;
    vector<string> v_str;

    if(fields_list[0] == "*"){
        //        cout<<"inside of if::fields_list[0] == *"<<endl;

        for(int i = 0; i < _field_list.size(); i++)
            v.push_back(i);
    }else{
        for(int i = 0; i < _field_list.size(); i++){
            for(int j = 0; j < fields_list.size(); j++){
                if(fields_list.at(j) == _field_list.at(i))
                    v.push_back(i);
            }
        }
    }

    vector<int> vec1;
    vector<int> vec2;
    vector<int> result;

    Queue<string> queue_temp;
    Stack<string> stack_temp;

    //    Stack<vector<int>> vec_num;
    vector<vector<int>> vec_num;
    vector<int> v2;
    queue_temp = shuntingYard(conditions);

    for(int i = 0; i < _field_list.size(); i++)
        _field_list_map.insert(_field_list[i], i);


    //evaluation for Shuangting Yard
    while(!queue_temp.empty()){
        if(precedence.contains(queue_temp.front())){
            switch(precedence[queue_temp.front()][0]){

            case TTEN: //operators
                if(queue_temp.front() == "="){
                    string right = stack_temp.top();
                    stack_temp.pop();
                    string left = stack_temp.top();
                    stack_temp.pop();

                    MMap<string, int> v = _mmapVect[_field_list_map[left][0]];
                    v2 = v[right];
                    //                    vect.push_back(v2);
                    vec_num.push_back(v2);
                    //                    vec_num.push(v2);


                }else if(queue_temp.front() == ">"){
                    //                    cout<<"in >"<<endl;
                    string right = stack_temp.top();
                    stack_temp.pop();
                    string left = stack_temp.top();
                    stack_temp.pop();

                    //                    cout<<"1"<<endl;
                    MMap<string, int>::Iterator it_up = _mmapVect[_field_list_map[left][0]].upper_bound(right);
                    MMap<string, int>::Iterator it_end = _mmapVect[_field_list_map[left][0]].end();
                    //                    cout<<"1-"<<endl;
                    if(it_up){
                        if(_mmapVect[_field_list_map[left][0]].contains(right)){
                            //                            cout<<"in if"<<endl;
                            it_up++;
                        }
                    }
                    //                    cout<<"2"<<endl;

                    while(it_up != it_end){
                        v2 += (*it_up).value_list;
                        it_up++;
                    }
                    //                    cout<<"3"<<endl;

                    //                    vect.push_back(v2);
                    vec_num.push_back(v2);
                    //                    vec_num.push(v2);


                }else if(queue_temp.front() == "<"){
                    string right = stack_temp.top();
                    stack_temp.pop();
                    string left = stack_temp.top();
                    stack_temp.pop();


                    MMap<string, int>::Iterator it_low = _mmapVect[_field_list_map[left][0]].lower_bound(right);
                    MMap<string, int>::Iterator it_begin = _mmapVect[_field_list_map[left][0]].begin();
                    while(it_low != it_begin){
                        v2 += (*it_begin).value_list;
                        it_begin++;
                    }
                    //                    vect.push_back(v2);
                    vec_num.push_back(v2);
                    //                    vec_num.push(v2);



                }else if(queue_temp.front() == ">="){
                    string right = stack_temp.top();
                    stack_temp.pop();
                    string left = stack_temp.top();
                    stack_temp.pop();

                    MMap<string, int>::Iterator it_low = _mmapVect[_field_list_map[left][0]].lower_bound(right);
                    MMap<string, int>::Iterator it_end = _mmapVect[_field_list_map[left][0]].end();

                    while(it_low != it_end){
                        v2 += (*it_low).value_list;
                        it_low++;
                    }
                    //                    vect.push_back(v2);
                    vec_num.push_back(v2);
                    //                    vec_num.push(v2);
                    //                    cout<<">="<<endl;

                }else if(queue_temp.front() == "<="){
                    string right = stack_temp.top();
                    stack_temp.pop();
                    string left = stack_temp.top();
                    stack_temp.pop();
                    MMap<string, int>::Iterator it_up = _mmapVect[_field_list_map[left][0]].upper_bound(right);
                    MMap<string, int>::Iterator it_begin = _mmapVect[_field_list_map[left][0]].begin();
                    while(it_up != it_begin){
                        v2 += (*it_begin).value_list;
                        it_begin++;
                    }
                    //                    vect.push_back(v2);
                    vec_num.push_back(v2);
                    //                    vec_num.push(v2);
                    //                    cout<<"<="<<endl;

                }
                break;

            case TSEVEN:{   //and

                //                cout<<"and::vec_num: "<<vec_num<<endl;
                vector<int> vect_temp1 = vec_num.back();
                //                                cout<<"and::vect_temp1: "<<vect_temp1<<endl;
                vec_num.pop_back();
                vector<int> vect_temp2 = vec_num.back();
                //                                cout<<"and::vect_temp2: "<<vect_temp2<<endl;
                vec_num.pop_back();
                //                sort(vect_temp1.begin(),vect_temp1.end());
                //                sort(vect_temp2.begin(),vect_temp2.end());

                //                set_intersection(vect_temp1.begin(), vect_temp1.end(), vect_temp2.begin(),
                //                                 vect_temp2.end(), v2.begin());
                v2 = Intersection(vect_temp1,vect_temp2);


                //                                cout<<"and:: v2 :"<<v2<<endl;
                vec_num.push_back(v2);


                //                if(vec_num.get_size() >= 2){
                //                    vec1 = vec_num.pop();
                //                    vec2 = vec_num.pop();
                //                    sort(vec1.begin(),vec1.end());
                //                    sort(vec2.begin(),vec2.end());
                //                    //get intersection v1 and v2 and put it into result
                //                    set_intersection(vec1.begin(), vec1.end(),
                //                                     vec2.begin(),vec2.end(),
                //                                     v2.begin());
                //                    if(!result.empty()){
                //                        vec_num.push(v2);
                //                    }


                //                }

            }
                break;
            case TTWO:{ //or

                //                cout<<"====inside case or===="<<endl;


                //                cout<<"or::vec_num: "<<vec_num<<endl;

                vector<int> vect_temp1 = vec_num.back();
                //                cout<<"or::vect_temp1: "<<vect_temp1<<endl;
                vec_num.pop_back();
                vector<int> vect_temp2 = vec_num.back();
                //                cout<<"or::vect_temp2: "<<vect_temp2<<endl;
                vec_num.pop_back();

                //                for(int i = 0; i < vect_temp2.size(); i++){
                //                    vect_temp1.push_back(vect_temp2[i]);
                //                }

                //                if(vec_num.get_size() >= 2){
                //                    vec1 = vec_num.pop();
                //                    vec2 = vec_num.pop();

                //                    result += vec1;
                //                    result += vec2;

                //                    sort(result.begin(),result.end());

                //                    result.erase( unique(result.begin(), result.end() ), result.end());
                //                    vec_num.push(result);
                //                }

                //                sort(vect_temp1.begin(), vect_temp1.end());

                //                //remove duplicates
                //                vect_temp1.erase(unique (vect_temp1.begin(), vect_temp1.end()), vect_temp1.end());
                //                v2 = vect_temp1;

                v2 = Union(vect_temp1,vect_temp2);

                //                  cout<<"or:: v2 :"<<v2<<endl;
                vec_num.push_back(v2);

            }
                break;
            default:

                break;
            }//switch
            queue_temp.pop();

        }//if contains
        else{
            stack_temp.push(queue_temp.front());
            queue_temp.pop();

        }//else (!contains)
    }

    fstream _f;
    fstream _f_source;
    int recno = 0;
    Record f_temp(recno);

    string new_db_name = _dbname + to_string(rand());
    string f_temp_name = new_db_name + "_db.txt";
    Table temp(new_db_name, fields_list);

    open_fileRW(_f, f_temp_name.c_str());
    open_fileRW(_f_source, _filename);

    _f_source.seekg(0, _f_source.end);
    recno = f_size(_f_source, MAX_RECORD_WIDTH * MAX_RECORD_WIDTH);

    for(int i = 0; i < v2.size(); i++){
        for(int j = 0; j < v.size(); j++){

            _rec.read(_f_source, v2[i]);
            v_str.push_back(_rec.record[v[j]]);

        }
        temp.insert(v_str);
        v_str.clear();
    }

    return temp;
}

vector<int> Table::Intersection(vector<int> left, vector<int> right){

    vector<int> result;
    sort(left.begin(),left.end());
    sort(right.begin(),right.end());



    set_intersection(left.begin(), left.end(), right.begin(),
                     right.end(), inserter(result,result.begin()));

    //    cout<<"left: "<<left<<endl;
    //    cout<<"right: "<<right<<endl;
    //    cout<<"result: "<<result<<endl;

    return result;

}
vector<int> Table::Union(vector<int> left, vector<int> right){ 

    vector<int>  result;



    sort(left.begin(),left.end());
    sort(right.begin(),right.end());
    set_union(left.begin(), left.end(), right.begin(),
              right.end(), inserter(result,result.begin()));

    //    cout<<"left: "<<left<<endl;
    //    cout<<"right: "<<right<<endl;
    //    cout<<"result: "<<result<<endl;

//    result+=left;
//    result+=right;
//    sort(result.begin(),result.end());
//    result.erase(unique(result.begin(),result.end(),result.end()));

    return result;

}

void Table::loadFields(){
    ifstream file(_filename_fields);
    string temp;

    int i = 0;

    if(file.is_open()){
        while(getline(file, temp)){
            _field_list.push_back(temp);
            _field_list_map.insert(temp, i);
            i++;
        }
    }

    file.close();
}

void Table::writeFields(vector<string> fields){
    ofstream file;
    file.open(_filename_fields);

    for(auto &f : fields){
        file << f << "\n";
    }
    file.close();
}

void Table::set_precedence(){
    precedence.insert("=", 10);
    precedence.insert("<", 10);
    precedence.insert(">", 10);
    precedence.insert("<=", 10);
    precedence.insert(">=", 10);
    precedence.insert("and", 7);
    precedence.insert("or", 2);

}

Queue<string> Table::shuntingYard(vector<string> &conditions){
    Queue<string> output_q;
    Stack<string> stk;

    //    cout<<"1"<<endl;
    set_precedence();

    for(int i = 0; i < conditions.size(); i++){
        if(precedence.contains(conditions[i])){
            if(!stk.empty()){
                while(!stk.empty() && precedence[conditions[i]][0] <= precedence[stk.top()][0]){
                    output_q.push((stk.pop()));
                }
                stk.push(conditions[i]);
            }else{
                stk.push(conditions[i]);
            }

        }else{
            output_q.push(conditions[i]);
        }
    }
    //    cout<<"2"<<endl;

    while(!stk.empty()){
        output_q.push(stk.pop());
    }


    cout<<"output_q:  "<<output_q<<endl;

    return output_q;

}

