#ifndef LEX_H
#define LEX_H

#include <string>
#include <vector>
#include "rwfile.h"
#include "minDFA.h"
using namespace std;

class Lex
{
public:
    string inputFileName;
    string outputFileName;
    string lexErrorFileName;
    vector<string> regExps;
    vector<minDFA> minDfa;
    vector<string> keyWords;      // 关键字
    vector<string> specialSymbol; // 特殊符号

public:
    Lex(string inputFileName, string outputFileName, string lexErrorFileName, string regExpsFileName, string keyWordsFileName, string specialSymbolFileName);
    int is_right_string(string str); // 判断字符串是否符合正则表达式
    bool is_special_symbol(char ch);
};

#endif