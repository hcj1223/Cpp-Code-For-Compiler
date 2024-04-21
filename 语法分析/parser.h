#ifndef PARSE_H
#define PARSE_H

#include "LL1.h"
#include "LR0.h"
using namespace std;

class Parser
{
public:
    vector<LL1> LL1s;
    vector<LR0> LR0s;

public:
    void init_parser(string fileName, int k); // 从文件中读取文法，k=0初始化LL1，k=1初始化LR0
    int run_parser_LL1(string fileName);      // 使用LL(1)进行语法分析
    int run_parser_LR0(string fileName);      // 使用LR(0)进行语法分析
};

#endif