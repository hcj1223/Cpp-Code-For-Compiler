#ifndef LL1_H
#define LL1_H

#include "contextfreegrammar.h"
using namespace std;

struct LL1Node // 表中对应的一项
{
    string nonTerminal; // 非终结符
    string terminal;    // 终结符
    string grammarId;   // 产生式编号
};
class LL1
{
public:
    ContextFreeGrammar cfg;   // 上下无关文法
    vector<LL1Node> LL1Table; // LL(1)分析表

public:
    void init_LL1(vector<string> grammar); // 初始化，产生上下无关文法
    void run_LL1();                        // 产生LL(1)分析表
};

#endif