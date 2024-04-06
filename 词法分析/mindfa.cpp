#include "mindfa.h"
#include "dfa.h"
#include <algorithm>
#include <iostream>

minDFA build_min_DFA(RegExp regExp)
{
    minDFA minDfa;
    DFA dfa = build_DFA(regExp);
    vector<int> v;          // 一个编号对应一个区间
    vector<vector<int>> vV; // 一个区间包含哪些编号
    v.resize(dfa.graph.size());
    vV.push_back(vector<int>()), vV.push_back(vector<int>());
    for (int i = 0; i < dfa.graph.size(); i++)
        if (find(dfa.end.begin(), dfa.end.end(), i) == dfa.end.end())
        {
            v[i] = 0;
            vV[0].push_back(i);
        }
        else
        {
            v[i] = 1;
            vV[1].push_back(i);
        }
    bool have_diff = 1; // 整个区间里有不同
    while (have_diff)
    {
        have_diff = 0;
        for (int i = 0; i < vV.size(); i++)
        {
            int flag = vV[i][0]; // 以每个区间的第一个为标准
            for (int j = 1; j < vV[i].size(); j++)
            {
                for (auto k : dfa.graph[vV[i][j]])
                    if (v[k.second] != v[dfa.graph[flag][k.first]])
                    {
                        if (have_diff == 0)
                        {
                            have_diff = 1;
                            for (int t = 0; t < v.size(); t++)
                                if (v[t] > v[vV[i][j]])
                                    v[t]++;
                            v[vV[i][j]]++;
                            break;
                        }
                        else
                        {
                            v[vV[i][j]]++;
                            break;
                        }
                    }
            }
            if (have_diff)
            {
                for (int j = 0; j < vV.size(); j++)
                    vV[j].clear();
                vV.push_back(vector<int>());
                for (int j = 0; j < v.size(); j++)
                    vV[v[j]].push_back(j);
                break;
            }
        }
        /* 检查每一步操作
        for (int i = 0; i < v.size(); i++)
            cout << v[i] << " ";
        cout << endl;
        for (int i = 0; i < vV.size(); i++)
        {
            for (int j = 0; j < vV[i].size(); j++)
                cout << vV[i][j] << " ";
            cout << endl;
        }
        cout << endl; */
    }
    minDfa.start = dfa.start;
    for (int i = 0; i < dfa.end.size(); i++)
        minDfa.end.insert(v[dfa.end[i]]);
    minDfa.graph.resize(vV.size());
    for (int i = 0; i < vV.size(); i++)
        for (auto ch : regExp.chs)
            if (dfa.graph[vV[i][0]].count(ch))
                minDfa.graph[i][ch] = v[dfa.graph[vV[i][0]][ch]];
    return minDfa;
}