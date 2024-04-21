#include "LR0.h"
#include <iostream>
#include <queue>
using namespace std;

void LR0::init_LR0(vector<string> grammar)
{
    ContextFreeGrammar cfg;
    cfg.get_grammer(grammar);
    this->cfg = cfg;
}
void LR0::get_LR0_dfa()
{
    vector<vector<string>> G = this->cfg.grammar;

    LR0DfaNode exNode; // 拓广文法
    exNode.LR0_node_insert({"newS", G[0][0]}, 1);
    for (int i = 0; i < G.size(); i++)
        if (G[i][0] == G[0][0])
            exNode.LR0_node_insert(G[i], 1);
    exNode.nodeId = 0;
    lr0DfaNode.push_back(exNode); // 状态0

    queue<LR0DfaNode> q; // 队列
    q.push(exNode);

    while (!q.empty())
    {
        LR0DfaNode node = q.front();
        q.pop();

        for (int i = 0; i < node.sentence.size(); i++) // 对于每个句子
        {
            if (node.sentence[i].second != node.sentence[i].first.size()) // S->.aA | S->a.A的形式
            {
                LR0DfaNode newNode;
                newNode.LR0_node_insert(node.sentence[i].first, node.sentence[i].second + 1); // 将S->.aA变成S->a.A

                string edge = node.sentence[i].first[node.sentence[i].second]; // 边为a

                if (node.sentence[i].second + 1 != node.sentence[i].first.size())
                {
                    string A = node.sentence[i].first[node.sentence[i].second + 1]; // 寻找A的产生式
                    if (cfg.nonTerminal.find(A) != cfg.nonTerminal.end())           // A是非终结符，有产生式
                    {
                        for (int j = 0; j < G.size(); j++)
                            if (G[j][0] == A)
                                newNode.LR0_node_insert(G[j], 1); // 插入A->.cA和A->.d
                    }
                }
                else
                    newNode.reduce = 1; // S->aA. 遇到需要规约

                bool isExist = 0; // 判断新节点是否已经存在
                int p = -1;       // 若存在，节点的编号
                for (int k = 0; k < lr0DfaNode.size(); k++)
                    if (newNode.LR0_node_is_equal(lr0DfaNode[k]))
                    {
                        isExist = 1;
                        p = k;
                        break;
                    }

                if (!isExist) // 确实是新节点
                {
                    newNode.nodeId = lr0DfaNode.size();
                    lr0DfaNode.push_back(newNode);
                    lr0Dfa[node.nodeId][edge] = newNode.nodeId; // 插入边和节点
                    q.push(newNode);
                }
                else
                    lr0Dfa[node.nodeId][edge] = p; // 插入边和节点
            }
        }
    }

    // 测试DFA的节点信息
    /* for (int i = 0; i < lr0DfaNode.size(); i++)
    {
        cout << "Node " << i << ": " << endl;
        for (int j = 0; j < lr0DfaNode[i].sentence.size(); j++)
        {
            cout << lr0DfaNode[i].sentence[j].first[0] << "->";
            for (int k = 1; k < lr0DfaNode[i].sentence[j].first.size(); k++)
            {
                if (k == lr0DfaNode[i].sentence[j].second)
                    cout << ".";
                cout << lr0DfaNode[i].sentence[j].first[k];
            }
            if (lr0DfaNode[i].sentence[j].second == lr0DfaNode[i].sentence[j].first.size())
                cout << ".";
            cout << endl;
        }
        cout << "REDUCE = " << lr0DfaNode[i].reduce << endl;
        cout << endl;
    } */

    // 测试DFA的图
    /* for (auto it : lr0Dfa)
    {
        for (auto it2 : it.second)
            cout << it.first << " -> " << it2.first << " -> " << it2.second << endl;
        cout << endl;
    } */
}
void LR0::get_LR0_table()
{
    for (int k = 0; k < lr0DfaNode.size(); k++)
    {
        if (lr0DfaNode[k].reduce) // 需要规约
        {
            if (lr0DfaNode[k].sentence[0].first[0] == "newS")
                action[k]["$"] = "acc"; // S'->S
            else
            {
                // 寻找产生式
                int grammarId = -1;
                for (int i = 0; i < cfg.grammar.size(); i++)
                    if (cfg.grammar[i] == lr0DfaNode[k].sentence[0].first)
                    {
                        grammarId = i;
                        break;
                    }
                for (auto it2 : cfg.terminal)
                    action[k][it2.first] = "r" + to_string(grammarId);
                action[k]["$"] = "r" + to_string(grammarId);
            }
        }
        else
        {
            for (auto it2 : lr0Dfa[k]) // 边和后继节点
            {
                if (cfg.nonTerminal.find(it2.first) != cfg.nonTerminal.end()) // 边是非终结符
                {
                    // 插入goto表
                    goTo[k][it2.first] = it2.second;
                }
                else // 终结符
                {
                    // 插入action表
                    action[k][it2.first] = "s" + to_string(it2.second);
                }
            }
        }
    }

    // 测试LR(0)分析表
    for (int i = 0; i < lr0DfaNode.size(); i++)
    {
        cout << "STATE " << i << " --- ACTION --- ";
        for (auto it : cfg.terminal)
        {
            cout << it.first << ":";
            if (action[i].find(it.first) != action[i].end())
                cout << action[i][it.first];
            cout << " ";
        }
        cout << "$"
             << ":";
        if (action[i].find("$") != action[i].end())
            cout << action[i]["$"];
        cout << " ";

        cout << "--- GOTO --- ";
        for (auto it : cfg.nonTerminal)
        {
            cout << it.first << ":";
            if (goTo[i].find(it.first) != goTo[i].end())
                cout << goTo[i][it.first];
            cout << " ";
        }
        cout << endl;
    }
}