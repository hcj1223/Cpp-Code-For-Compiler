#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "rwfile.h"
using namespace std;

vector<string> read_file(string fname)
{
    vector<string> contents; // 将文件的内容保存在contents数组中
    ifstream infile;
    infile.open(fname);
    string line;
    while (getline(infile, line)) // 从文件中按每行读取，读入到contents数组中
        contents.push_back(line);
    infile.close();
    return contents;
}
void write_file(string fname, vector<string> contents)
{
    ofstream outfile;
    outfile.open(fname, ios::app);
    for (int i = 0; i < contents.size(); i++) // 将需要写入的内容按每行写入文件中
        outfile << contents[i] << endl;
    outfile.close();
}
void write_file(string fname, string content)
{
    ofstream outfile;
    outfile.open(fname, ios::app);
    outfile << content << endl;
    outfile.close();
}