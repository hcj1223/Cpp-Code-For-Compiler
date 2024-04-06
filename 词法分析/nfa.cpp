#include "nfa.h"
#include "regexp.h"
#include <stack>
#include <iostream>
using namespace std;

NFA::NFA()
{
    graph.resize(1);
    start = end = 0;
}
NFA NFA::merge_nfa(NFA other)
{
    int tSize = graph.size();
    for (int i = 0; i < other.graph.size(); i++)
    {
        // 将两图放入同一NFA中
        graph.push_back(other.graph[i]);
        // 改变各点编号
        for (auto &it : graph.back())
            for (auto &point : it.second)
                point += tSize;
    }
    return *this;
}
NFA build_NFA(RegExp regExp)
{
    // '#'表示空字符
    stack<NFA> stk;
    for (auto c : regExp.suffixExp)
    {
        if (c == '*')
        {
            NFA tmp;
            int tSize1 = tmp.graph.size();
            NFA a = stk.top();
            stk.pop();
            tmp.merge_nfa(a);
            tmp.graph[tmp.start]['#'].push_back(a.start + tSize1);
            tmp.graph[a.end + tSize1]['#'].push_back(a.start + tSize1);
            int tSize2 = tmp.graph.size();
            tmp.graph[a.end + tSize1]['#'].push_back(tSize2);
            tmp.end = tSize2;
            tmp.graph.push_back(unordered_map<char, vector<int>>());
            tmp.graph[tmp.start]['#'].push_back(tSize2);
            stk.push(tmp);
        }
        else if (c == '.')
        {
            NFA a, b;
            b = stk.top(), stk.pop();
            a = stk.top(), stk.pop();
            a.graph.pop_back();
            int aSize = a.graph.size();
            a.merge_nfa(b);
            a.end = b.end + aSize;
            stk.push(a);
        }
        else if (c == '|')
        {
            NFA tmp, a, b;
            b = stk.top(), stk.pop();
            a = stk.top(), stk.pop();
            int tSize1 = tmp.graph.size();
            tmp.merge_nfa(a);
            tmp.graph[tmp.start]['#'].push_back(a.start + tSize1);
            int tSize2 = tmp.graph.size();
            tmp.merge_nfa(b);
            tmp.graph[tmp.start]['#'].push_back(b.start + tSize2);
            tmp.graph[a.end + tSize1]['#'].push_back(tmp.graph.size());
            tmp.graph[b.end + tSize2]['#'].push_back(tmp.graph.size());
            tmp.end = tmp.graph.size();
            tmp.graph.push_back(unordered_map<char, vector<int>>());
            stk.push(tmp);
        }
        else // 字符
        {
            NFA tmp;
            tmp.graph.resize(2);
            tmp.end = 1;
            tmp.graph[0][c].push_back(tmp.end);
            stk.push(tmp);
        }
    }
    return stk.top();
}