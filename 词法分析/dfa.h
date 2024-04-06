#ifndef DFA_H
#define DFA_H

#include <set>
#include <unordered_map>
#include "nfa.h"
#include "regExp.h"
using namespace std;
class DFA
{
public:
    vector<unordered_map<char, int>> graph;      // DFA图
    int start;                                   // DFA的起点
    vector<int> end;                             // 终点集合
    unordered_map<int, set<int>> eClosures;      // 每个节点e闭包
    vector<unordered_map<char, set<int>>> moves; // 每个节点经过一个字符的下一个节点集合

public:
    set<int> get_e_closure(vector<unordered_map<char, vector<int>>> graph, int k);          // 返回e闭包
    set<int> get_move(vector<unordered_map<char, vector<int>>> graph, int k, char ch);      // 返回经过ch弧的下一个节点
    set<int> get_move(vector<unordered_map<char, vector<int>>> graph, set<int> s, char ch); // 返回集合s经过ch弧的下一个节点集合的e闭包
};
DFA build_DFA(RegExp regExp); // 从NFA到DFA

#endif