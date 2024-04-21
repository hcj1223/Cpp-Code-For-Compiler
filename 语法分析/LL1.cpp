#include "LL1.h"
#include <iostream>
using namespace std;

void LL1::get_first()
{
    vector<vector<string>> grammar = this->cfg.grammar;
    while (1)
    {
        bool circle = 0; // 进行下一轮
        for (int i = 0; i < grammar.size(); i++)
        {
            if (grammar[i][1] != grammar[i][0])
            {
                if (cfg.nonTerminal.find(grammar[i][1]) == cfg.nonTerminal.end()) // "->"是终结符
                    first[grammar[i][0]].insert(grammar[i][1]);
                else
                {
                    if (first.find(grammar[i][1]) != first.end()) // "->"是非终结符且它的first集已经求出
                        first[grammar[i][0]].insert(first[grammar[i][1]].begin(), first[grammar[i][1]].end());
                    else
                        circle = 1;
                }
            }
        }
        if (!circle)
            break;
    }
}
void LL1::get_follow()
{
    vector<vector<string>> grammar = this->cfg.grammar;
    while (1)
    {
        bool circle = 0; // 进行下一轮
        for (int i = 0; i < grammar.size(); i++)
        {
            for (int j = 1; j < grammar[i].size(); j++)
            {
                if (cfg.nonTerminal.find(grammar[i][j]) != cfg.nonTerminal.end()) // 是非终结符
                {
                    if (grammar[i][j] == grammar[0][0])          // 开始符
                        this->follow[grammar[i][j]].insert("$"); // 添加界符
                    if (j == grammar[i].size() - 1)              // 后面没有字符
                    {
                        // follow[B] = follow[A]
                        if (this->follow.find(grammar[i][0]) != this->follow.end()) // 该非终结符的follow集已经求出
                            this->follow[grammar[i][j]].insert(this->follow[grammar[i][0]].begin(), this->follow[grammar[i][0]].end());
                        else
                            circle = 1;
                    }
                    else if (cfg.terminal.find(grammar[i][j + 1]) != cfg.terminal.end()) // 后面是终结符
                    {
                        // first[terminal] = {terminal}
                        this->follow[grammar[i][j]].insert(grammar[i][j + 1]);
                    }
                    else // 后面是非终结符
                    {
                        // follow[B] = first[C]
                        // first[C]中不含有#
                        bool hasE = 0;
                        for (auto item : this->first[grammar[i][j + 1]])
                        {
                            if (item != "#")
                                this->follow[grammar[i][j]].insert(item);
                            else
                                hasE = 1;
                        }
                        if (hasE)                                                       // 下个非终结符号的first集中含有#
                            if (this->follow.find(grammar[i][0]) != this->follow.end()) // 该非终结符的follow集已经求出
                                this->follow[grammar[i][j]].insert(this->follow[grammar[i][0]].begin(), this->follow[grammar[i][0]].end());
                            else
                                circle = 1;
                    }
                }
            }
        }
        if (!circle)
            break;
    }
}
void LL1::init_LL1(vector<string> grammar)
{
    ContextFreeGrammar cfg;
    cfg.get_grammer(grammar);
    this->cfg = cfg;
    get_first();
    get_follow();

    /* cout << "Grammar: " << endl;
    for (int ii = 0; ii < cfg.grammar.size(); ii++)
    {
        cout << cfg.grammar[ii][0] << " -> ";
        for (int jj = 1; jj < cfg.grammar[ii].size(); jj++)
            cout << cfg.grammar[ii][jj] << " ";
        cout << endl;
    }
    cout << endl;

    cout << "Non-Terminal: " << endl;
    for (auto it : cfg.nonTerminal)
        cout << it.first << endl;
    cout << endl;

    cout << "Terminal: " << endl;
    for (auto it : cfg.terminal)
        cout << it.first << endl;
    cout << endl;

    cout << "First Set: " << endl;
    for (auto it : first)
    {
        cout << it.first << ": { ";
        for (auto it2 : it.second)
            cout << it2 << " ";
        cout << "}" << endl;
    }
    cout << endl;

    cout << "Follow Set: " << endl;
    for (auto it : follow)
    {
        cout << it.first << ": { ";
        for (auto it2 : it.second)
            cout << it2 << " ";
        cout << "}" << endl;
    }
    cout << endl; */
}
void LL1::get_LL1_table()
{
    for (int i = 0; i < cfg.grammar.size(); i++)
    {
        if (cfg.nonTerminal.find(cfg.grammar[i][1]) != cfg.nonTerminal.end())
        {
            for (auto item : first[cfg.grammar[i][0]])
                if (item != "#")
                    if (LL1Table[cfg.grammar[i][0]].find(item) == LL1Table[cfg.grammar[i][0]].end())
                        LL1Table[cfg.grammar[i][0]][item] = i;
        }
        else if (cfg.terminal.find(cfg.grammar[i][1]) != cfg.terminal.end())
        {
            if (LL1Table[cfg.grammar[i][0]].find(cfg.grammar[i][1]) == LL1Table[cfg.grammar[i][0]].end())
                LL1Table[cfg.grammar[i][0]][cfg.grammar[i][1]] = i;
        }
        else if (cfg.grammar[i][1] == "#")
        {
            for (auto item : follow[cfg.grammar[i][0]])
                if (LL1Table[cfg.grammar[i][0]].find(item) == LL1Table[cfg.grammar[i][0]].end())
                    LL1Table[cfg.grammar[i][0]][item] = i;
        }
    }

    /* cout << "LL1 Table: " << endl;
    for (auto it : LL1Table)
    {
        for (auto it2 : it.second)
            cout << it.first << " " << it2.first << " " << it2.second << endl;
    }
    cout << endl; */
}