#ifndef LL1_H
#define LL1_H

#include "contextfreegrammar.h"
using namespace std;

class LL1
{
public:
    ContextFreeGrammar cfg;                                     // 上下无关文法
    unordered_map<string, set<string>> first;                   // first集
    unordered_map<string, set<string>> follow;                  // follow集
    unordered_map<string, unordered_map<string, int>> LL1Table; // LL(1)分析表

public:
    void get_first();                      // 获得first集
    void get_follow();                     // 获得follow集
    void init_LL1(vector<string> grammar); // 初始化，产生上下无关文法
    void get_LL1_table();                  // 产生LL(1)分析表
};

#endif