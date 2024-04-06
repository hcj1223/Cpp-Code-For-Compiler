#include "dfa.h"
#include "nfa.h"
#include <set>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iostream>
using namespace std;

set<int> DFA::get_e_closure(vector<unordered_map<char, vector<int>>> graph, int k)
{
    if (eClosures.find(k) != eClosures.end()) // 已经计算过
        return eClosures[k];

    set<int> eClosure;
    eClosure.insert(k);
    queue<vector<int>> q;
    if (graph[k].find('#') != graph[k].end())
        q.push(graph[k]['#']);
    while (!q.empty()) // bfs遍历图
    {
        vector<int> tmp = q.front();
        q.pop();
        for (int i = 0; i < tmp.size(); i++)
        {
            if (eClosures.find(tmp[i]) != eClosures.end()) // 已经计算过
            {
                eClosure.insert(eClosures[tmp[i]].begin(), eClosures[tmp[i]].end());
                continue;
            }
            eClosure.insert(tmp[i]);
            if (graph[tmp[i]].find('#') != graph[tmp[i]].end())
                q.push(graph[tmp[i]]['#']);
        }
    }
    eClosures.insert(make_pair(k, eClosure));
    return eClosure;
}
set<int> DFA::get_move(vector<unordered_map<char, vector<int>>> graph, int k, char ch)
{
    if (moves.size() > k && moves[k].find(ch) != moves[k].end()) // 已经计算过
        return moves[k][ch];

    set<int> move;
    for (int i = 0; i < graph[k][ch].size(); i++)
        move.insert(graph[k][ch][i]);

    if (moves.size() <= k)
        for (int i = moves.size(); i <= k; i++)
            moves.push_back(unordered_map<char, set<int>>());

    moves[k].insert(make_pair(ch, move));
    return move;
}
set<int> DFA::get_move(vector<unordered_map<char, vector<int>>> graph, set<int> s, char ch)
{
    set<int> tmp, move;
    for (auto &i : s)
    {
        set<int> st1 = get_move(graph, i, ch);
        tmp.insert(st1.begin(), st1.end());
    }
    for (auto &i : tmp)
    {
        set<int> st2 = get_e_closure(graph, i);
        move.insert(st2.begin(), st2.end());
    }
    return move;
}
DFA build_DFA(RegExp regExp)
{
    DFA dfa;
    dfa.graph.push_back(unordered_map<char, int>());
    NFA nfa = build_NFA(regExp);
    dfa.start = nfa.start;
    set<int> I = dfa.get_e_closure(nfa.graph, nfa.start);
    vector<set<int>> dfaVector; // 出现过的集合
    dfaVector.push_back(I);
    queue<set<int>> q;
    q.push(I);
    int k = 0;
    while (!q.empty())
    {
        auto tmp = q.front();
        q.pop();
        for (auto &ch : regExp.chs)
        {
            set<int> I_ch = dfa.get_move(nfa.graph, tmp, ch);
            if (!I_ch.empty() && find(dfaVector.begin(), dfaVector.end(), I_ch) != dfaVector.end()) // 出现过
            {
                for (int i = 0; i < dfaVector.size(); i++)
                    if (dfaVector[i] == I_ch)
                        dfa.graph[k][ch] = i;
            }
            else if (!I_ch.empty() && find(dfaVector.begin(), dfaVector.end(), I_ch) == dfaVector.end()) // 没出现
            {
                dfa.graph.push_back(unordered_map<char, int>());
                dfa.graph[k][ch] = dfaVector.size();
                dfaVector.push_back(I_ch);
                q.push(I_ch);
            }
        }
        k++;
    }
    for (int i = 0; i < dfaVector.size(); i++) // 添加终点集合
        for (auto &j : dfaVector[i])
            if (j == nfa.end)
            {
                dfa.end.push_back(i);
                break;
            }

    // 打印对应编号的集合
    /* for (int i = 0; i < dfaVector.size(); i++)
    {
        cout << "{ ";
        for (auto &j : dfaVector[i])
            cout << j << " ";
        cout << "}" << endl;
    } */

    return dfa;
}