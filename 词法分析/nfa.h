#ifndef NFA_H
#define NFA_H

#include <vector>
#include <unordered_map>
#include "regexp.h"
using namespace std;

class NFA
{
public:
    vector<unordered_map<char, vector<int>>> graph; // NFA图
    int start, end;                                 // NFA图的起点和终点

public:
    NFA();
    NFA merge_nfa(NFA other); // 将两图放入同一NFA中，改写编号
};
NFA build_NFA(RegExp regExp); // 从正则表达式到NFA

#endif