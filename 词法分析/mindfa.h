#ifndef MINDFA_H
#define MINDFA_H

#include <vector>
#include <unordered_map>
#include "regexp.h"
using namespace std;

class minDFA
{
public:
    vector<unordered_map<char, int>> graph; // minDFA图
    int start;                              // 起点
    set<int> end;                           // 终点
};
minDFA build_min_DFA(RegExp regExp); // 从DFA到最小化DFA

#endif