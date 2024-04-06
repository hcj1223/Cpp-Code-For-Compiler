#ifndef RWFILE_H
#define RWFILE_H

#include <string>
#include <vector>
using namespace std;

vector<string> read_file(string fname);                 // 返回文件内容
void write_file(string fname, vector<string> contents); // 将内容写入文件
void write_file(string fname, string content);

#endif