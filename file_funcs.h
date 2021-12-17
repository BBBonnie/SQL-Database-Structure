#ifndef FILE_FUNCS_H
#define FILE_FUNCS_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

bool f_read_info(const char filename[], vector<string>& v);
bool f_write_info(const char filename[], const vector<string> &v);
long f_size(fstream& f, int max_size);


#endif // FILE_FUNCS_H
