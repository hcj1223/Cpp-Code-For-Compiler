#include "contextfreegrammar.h"
#include "rwfile.h"
#include <iostream>
using namespace std;

void ContextFreeGrammar::get_grammer(vector<string> grammar)
{
    // 读入
    vector<vector<string>> G;
    for (int i = 0; i < grammar.size(); i++)
    {
        string cur = "";
        G.push_back(vector<string>());
        for (int j = 0; j < grammar[i].size(); j++)
        {
            if (grammar[i][j] != ' ')
                cur = cur + grammar[i][j];
            else
            {
                G[G.size() - 1].push_back(cur);
                cur = "";
            }
        }
    }
    /*
    处理左递归:
        A -> Aa | b
        =>
        A -> bA'
        A' -> aA' | #
    */
    vector<vector<string>> G1; // 左递归结果
    for (int i = 0; i < G.size(); i++)
    {
        vector<vector<string>> a;
        vector<vector<string>> b;
        for (int j = 2; j < G[i].size(); j++)
        {
            if ((G[i][j - 1] == "->" || G[i][j - 1] == "|") && G[i][j] == G[i][0]) // 该段语句以A开头，其后就为a
            {
                a.push_back(vector<string>());
                int k = j + 1;
                while (k < G[i].size() && G[i][k] != "|")
                    a[a.size() - 1].push_back(G[i][k++]);
                j = k - 1;
            }
            else if ((G[i][j - 1] == "->" || G[i][j - 1] == "|") && G[i][j] != G[i][0]) // 该段语句以b开头
            {
                b.push_back(vector<string>());
                int k = j;
                while (k < G[i].size() && G[i][k] != "|")
                    b[b.size() - 1].push_back(G[i][k++]);
                j = k - 1;
            }
        }
        if (!a.empty()) // 需要处理左递归
        {
            // 得到处理左递归后的结果
            G1.push_back(vector<string>());
            G1[G1.size() - 1].push_back(G[i][0]), G1[G1.size() - 1].push_back("->");
            for (int ii = 0; ii < b.size(); ii++)
            {
                for (int jj = 0; jj < b[ii].size(); jj++)
                    G1[G1.size() - 1].push_back(b[ii][jj]);
                G1[G1.size() - 1].push_back(string(G[i][0] + "\'"));
                if (ii != b.size() - 1)
                    G1[G1.size() - 1].push_back("|");
            }
            G1.push_back(vector<string>());
            G1[G1.size() - 1].push_back(string(G[i][0] + "\'")), G1[G1.size() - 1].push_back("->");
            for (int ii = 0; ii < a.size(); ii++)
            {
                for (int jj = 0; jj < a[ii].size(); jj++)
                    G1[G1.size() - 1].push_back(a[ii][jj]);
                G1[G1.size() - 1].push_back(string(G[i][0] + "\'"));
                G1[G1.size() - 1].push_back("|");
            }
            G1[G1.size() - 1].push_back("#");
        }
        else
            G1.push_back(G[i]);
    }
    /*
    处理左因子:
        A -> a b | a c
        =>
        A -> a A'
        A' -> b | c
    */
    for (int i = 0; i < G1.size(); i++) // 暂时不处理左因子，希望在创建文法规则时解决
    {
        this->grammar.push_back(vector<string>());
        this->grammar[this->grammar.size() - 1].push_back(G1[i][0]);
        for (int j = 2; j < G1[i].size(); j++)
            if (G1[i][j] != "|")
                this->grammar[this->grammar.size() - 1].push_back(G1[i][j]);
            else
            {
                this->grammar.push_back(vector<string>());
                this->grammar[this->grammar.size() - 1].push_back(G1[i][0]);
            }
    }

    // 记录非终结符
    for (int i = 0; i < this->grammar.size(); i++)
        this->nonTerminal[this->grammar[i][0]] = 1;
    // 记录终结符
    for (int i = 0; i < this->grammar.size(); i++)
        for (int j = 1; j < this->grammar[i].size(); j++)
            if (this->grammar[i][j] != "#" && this->nonTerminal.find(this->grammar[i][j]) == this->nonTerminal.end())
                this->terminal[this->grammar[i][j]] = 1;
}
void ContextFreeGrammar::get_first()
{
    while (1)
    {
        bool circle = 0; // 进行下一轮
        for (int i = 0; i < this->grammar.size(); i++)
        {
            if (grammar[i][1] != grammar[i][0])
            {
                if (nonTerminal.find(this->grammar[i][1]) == nonTerminal.end()) // "->"是终结符
                    first[this->grammar[i][0]].insert(this->grammar[i][1]);
                else
                {
                    if (first.find(this->grammar[i][1]) != first.end()) // "->"是非终结符且它的first集已经求出
                        first[this->grammar[i][0]].insert(first[this->grammar[i][1]].begin(), first[this->grammar[i][1]].end());
                    else
                        circle = 1;
                }
            }
        }
        if (!circle)
            break;
    }
}
void ContextFreeGrammar::get_follow()
{
    while (1)
    {
        bool circle = 0; // 进行下一轮
        for (int i = 0; i < this->grammar.size(); i++)
        {
            for (int j = 1; j < this->grammar[i].size(); j++)
            {
                if (nonTerminal.find(this->grammar[i][j]) != nonTerminal.end()) // 是非终结符
                {
                    if (grammar[i][j] == grammar[0][0])                // 开始符
                        this->follow[this->grammar[i][j]].insert("$"); // 添加界符
                    if (j == this->grammar[i].size() - 1)              // 后面没有字符
                    {
                        // follow[B] = follow[A]
                        if (this->follow.find(this->grammar[i][0]) != this->follow.end()) // 该非终结符的follow集已经求出
                            this->follow[this->grammar[i][j]].insert(this->follow[this->grammar[i][0]].begin(), this->follow[this->grammar[i][0]].end());
                        else
                            circle = 1;
                    }
                    else if (terminal.find(this->grammar[i][j + 1]) != terminal.end()) // 后面是终结符
                    {
                        // first[terminal] = {terminal}
                        this->follow[this->grammar[i][j]].insert(this->grammar[i][j + 1]);
                    }
                    else // 后面是非终结符
                    {
                        // follow[B] = first[C]
                        // first[C]中不含有#
                        bool hasE = 0;
                        for (auto item : this->first[grammar[i][j + 1]])
                        {
                            if (item != "#")
                                this->follow[this->grammar[i][j]].insert(item);
                            else
                                hasE = 1;
                        }
                        if (hasE)                                                             // 下个非终结符号的first集中含有#
                            if (this->follow.find(this->grammar[i][0]) != this->follow.end()) // 该非终结符的follow集已经求出
                                this->follow[this->grammar[i][j]].insert(this->follow[this->grammar[i][0]].begin(), this->follow[this->grammar[i][0]].end());
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