#include "LR0.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

void LR0::init_LR0(vector<string> grammar)
{
    ContextFreeGrammar cfg;
    cfg.get_grammer(grammar);
    cfg.get_first();
    cfg.get_follow();
    this->cfg = cfg;

    for (int i = 0; i < cfg.grammar.size(); i++)
    {
        cout << cfg.grammar[i][0] << "->";
        for (int j = 1; j < cfg.grammar[i].size(); j++)
            cout << cfg.grammar[i][j] << " ";
        cout << endl;
    }

    for (auto it : cfg.first)
    {
        cout << "first{" << it.first << "} = { ";
        for (auto it2 : it.second)
            cout << it2 << " ";
        cout << "}" << endl;
    }

    for (auto it : cfg.follow)
    {
        cout << "follow{" << it.first << "} = { ";
        for (auto it2 : it.second)
            cout << it2 << " ";
        cout << "}" << endl;
    }
}
void LR0::get_LR0_dfa()
{
    vector<vector<string>> G = this->cfg.grammar;

    LR0DfaNode exNode; // 拓广文法
    exNode.LR0_node_insert({"newS", G[0][0]}, 1, {"$"});

    stack<string> needSeek; // "·"后跟着非终结符，需要找产生式
    set<string> symbols = {"$"};
    needSeek.push(G[0][0]);
    while (!needSeek.empty())
    {
        auto t = needSeek.top();
        needSeek.pop();

        for (int i = 0; i < G.size(); i++)
            if (G[i][0] == t)
            {
                exNode.LR0_node_insert(G[i], 1, symbols);
                if (this->cfg.nonTerminal.find(G[i][1]) != this->cfg.nonTerminal.end()) // "·"后是非终结符
                {
                    needSeek.push(G[i][1]);
                    if (2 < G[i].size())
                    {
                        if (this->cfg.nonTerminal.find(G[i][2]) != this->cfg.nonTerminal.end())
                            symbols = this->cfg.first[G[i][2]];
                        else
                            symbols = {G[i][2]};
                    }
                }
            }
    }
    exNode.nodeId = 0;
    lr0DfaNode.push_back(exNode); // 状态0

    queue<LR0DfaNode> q; // 队列
    q.push(exNode);
    while (!q.empty())
    {
        LR0DfaNode node = q.front();
        q.pop();

        map<string, LR0DfaNode> Map;

        for (int i = 0; i < node.sentence.size(); i++) // 对于每个句子
        {
            if (node.sentence[i].second.first < node.sentence[i].first.size()) // S->.aA | S->a.A的形式
            {
                string edge = node.sentence[i].first[node.sentence[i].second.first]; // 边为a

                vector<string> production = node.sentence[i].first;
                int dot = node.sentence[i].second.first + 1;
                set<string> st = node.sentence[i].second.second;

                Map[edge].LR0_node_insert(production, dot, st); // 将S->.aA,$变成S->a.A,$
            }
        }

        for (auto it : Map)
        {
            string edge = it.first;
            LR0DfaNode newNode = it.second;
            int Len = newNode.sentence.size();

            for (int i = 0; i < Len; i++)
            {
                vector<string> production = newNode.sentence[i].first;
                int dot = newNode.sentence[i].second.first;

                if (dot < production.size() && this->cfg.nonTerminal.find(production[dot]) != this->cfg.nonTerminal.end()) //"·"后面是非终结符
                {
                    stack<string> stk;
                    set<string> str;
                    if (dot + 1 < production.size())
                    {
                        if (this->cfg.nonTerminal.find(production[dot + 1]) != this->cfg.nonTerminal.end())
                            str = this->cfg.first[production[dot + 1]];
                        else
                            str = {production[dot + 1]};
                    }
                    else
                        str = {"$"};

                    stk.push(production[dot]);
                    while (!stk.empty())
                    {
                        auto t = stk.top();
                        stk.pop();

                        for (int ii = 0; ii < G.size(); ii++)
                            if (G[ii][0] == t) // 找到产生式
                            {
                                newNode.LR0_node_insert(G[ii], 1, str);
                                if (this->cfg.nonTerminal.find(G[ii][1]) != this->cfg.nonTerminal.end()) // "·"后是非终结符
                                {
                                    stk.push(G[ii][1]);
                                    if (2 < G[ii].size()) // 非终结符后还有内容
                                    {
                                        if (this->cfg.nonTerminal.find(G[ii][2]) != this->cfg.nonTerminal.end())
                                            str = this->cfg.first[G[ii][2]];
                                        else
                                            str = {G[ii][2]};
                                    }
                                }
                            }
                    }
                }
            }

            // 判断规约项
            set<string> X, follows;
            for (int i = 0; i < newNode.sentence.size(); i++)
            {
                if (newNode.sentence[i].second.first == newNode.sentence[i].first.size())
                {
                    newNode.reduce = 1;
                    if (newNode.reduce == 1)
                        follows.insert(newNode.sentence[i].first[0]);
                }
                else if (this->cfg.nonTerminal.find(newNode.sentence[i].first[newNode.sentence[i].second.first]) != this->cfg.nonTerminal.end() &&
                         newNode.reduce == 1)
                    X.insert(newNode.sentence[i].first[newNode.sentence[i].second.first]);
            }

            // 判断LR的类型
            if (!X.empty() || follows.size() > 1)
                this->type = "SLR1";

            set<string> U; // 出现过的非终结符的follow集的并集
            for (auto it2 : follows)
            {
                set<string> F = this->cfg.follow[it2];
                set<string> J; // 交集
                set_intersection(F.begin(), F.end(), U.begin(), U.end(), inserter(J, J.begin()));
                if (!J.empty())
                {
                    this->type = "LR1";
                    break;
                }
                U.insert(F.begin(), F.end());
            }
            if (this->type == "SLR1")
            {
                for (auto itX : X)
                    if (U.find(itX) != U.end())
                    {
                        this->type = "LR1";
                        break;
                    }
            }

            bool isExist = 0; // 判断新节点是否已经存在
            int p = -1;       // 若存在，节点的编号
            for (int k = 0; k < lr0DfaNode.size(); k++)
                if (newNode.LR0_node_is_equal(lr0DfaNode[k], this->type))
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

    // 测试DFA的节点信息
    for (int i = 0; i < lr0DfaNode.size(); i++)
    {
        cout << "Node " << i << ": " << endl;
        for (int j = 0; j < lr0DfaNode[i].sentence.size(); j++)
        {
            cout << lr0DfaNode[i].sentence[j].first[0] << "->";
            for (int k = 1; k < lr0DfaNode[i].sentence[j].first.size(); k++)
            {
                if (k == lr0DfaNode[i].sentence[j].second.first)
                    cout << ".";
                cout << lr0DfaNode[i].sentence[j].first[k];
            }
            if (lr0DfaNode[i].sentence[j].second.first == lr0DfaNode[i].sentence[j].first.size())
                cout << ".";
            if (this->type == "LR1")
            {
                cout << ",";
                for (auto it : lr0DfaNode[i].sentence[j].second.second)
                    cout << it << "/";
            }
            cout << endl;
        }
        cout << "REDUCE = " << lr0DfaNode[i].reduce << endl;
        cout << endl;
    }

    // 测试DFA的图
    for (auto it : lr0Dfa)
    {
        for (auto it2 : it.second)
            cout << it.first << " -> " << it2.first << " -> " << it2.second << endl;
        cout << endl;
    }
}
void LR0::get_LR0_table()
{
    for (int k = 0; k < lr0DfaNode.size(); k++)
    {
        for (int i = 0; i < lr0DfaNode[k].sentence.size(); i++)
        {
            vector<string> production = lr0DfaNode[k].sentence[i].first;
            int dot = lr0DfaNode[k].sentence[i].second.first;
            set<string> st = lr0DfaNode[k].sentence[i].second.second;

            if (dot < production.size())
            {
                string edge = production[dot];
                if (cfg.nonTerminal.find(edge) != cfg.nonTerminal.end()) // 边是非终结符
                {
                    // 插入goto表
                    goTo[k][edge] = lr0Dfa[k][edge];
                }
                else // 终结符
                {
                    // 插入action表
                    action[k][edge] = "s" + to_string(lr0Dfa[k][edge]);
                }
            }
            else
            {
                if (production[0] == "newS")
                    action[k]["$"] = "acc"; // S'->S
                else
                {
                    int grammarId = -1;
                    for (int i = 0; i < this->cfg.grammar.size(); i++)
                        if (this->cfg.grammar[i] == production)
                        {
                            grammarId = i;
                            break;
                        }
                    if (this->type == "LR0")
                    {
                        for (auto it2 : cfg.terminal)
                            action[k][it2.first] = "r" + to_string(grammarId);
                        action[k]["$"] = "r" + to_string(grammarId);
                    }
                    else if (this->type == "SLR1")
                    {
                        set<string> aFollow = this->cfg.follow[production[0]];
                        for (auto it2 : aFollow)
                            action[k][it2] = "r" + to_string(grammarId);
                    }
                    else if (this->type == "LR1")
                    {
                        for (auto it2 : st)
                            action[k][it2] = "r" + to_string(grammarId);
                    }
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