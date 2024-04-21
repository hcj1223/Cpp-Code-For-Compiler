#ifndef CONTEXTFREEGRAMMAR_H
#define CONTEXTFREEGRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
using namespace std;

class ContextFreeGrammar
{
public:
    vector<vector<string>> grammar;          // 上下文无关文法
    unordered_map<string, bool> nonTerminal; // 非终结符号集
    unordered_map<string, bool> terminal;    // 终结符号集

public:
    void get_grammer(vector<string> grammar); // 获得语法并消除左递归和提取左因子
};

#endif