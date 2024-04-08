#ifndef LL1_H
#define LL1_H

#include "contextfreegrammar.h"
using namespace std;

class LL1
{
public:
    ContextFreeGrammar cfg;                                     // 上下无关文法
    unordered_map<string, unordered_map<string, int>> LL1Table; // LL(1)分析表

public:
    void init_LL1(vector<string> grammar); // 初始化，产生上下无关文法
    void run_LL1();                        // 产生LL(1)分析表
};

#endif