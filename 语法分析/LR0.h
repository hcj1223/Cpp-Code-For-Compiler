#ifndef LR0_H
#define LR0_H

#include "contextfreegrammar.h"
#include <map>
using namespace std;

class LR0
{
public:
    class LR0DfaNode
    {
    public:
        vector<pair<vector<string>, int>> sentence; // 每个节点的各产生式，和"·"的位置在pos之前
        int nodeId;                                 // 节点编号
        bool reduce = 0;                            // 遇到该节点需要规约

    public:
        void LR0_node_insert(vector<string> g, int pos)
        {
            sentence.push_back(make_pair(g, pos));
        }
        bool LR0_node_is_equal(LR0DfaNode &node)
        {
            for (int i = 0; i < sentence.size(); i++)
            {
                if (node.sentence[i].first == this->sentence[i].first && node.sentence[i].second == this->sentence[i].second)
                    continue;
                return 0;
            }
            return 1;
        }
    };
    ContextFreeGrammar cfg;
    vector<LR0DfaNode> lr0DfaNode;
    map<int, map<string, int>> lr0Dfa;
    map<int, map<string, string>> action; // LR(0)分析表的action部分
    map<int, map<string, int>> goTo;      // LR(0)分析表的goto部分

public:
    void init_LR0(vector<string> grammar); // 初始化，产生上下无关文法
    void get_LR0_dfa();                    // 产生LR(0)的DFA图
    void get_LR0_table();                  // 产生LR(0)分析表
};

#endif