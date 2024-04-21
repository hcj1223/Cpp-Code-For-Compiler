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
    vector<vector<string>> G1; // 处理左递归结果
    for (int i = 0; i < G.size(); i++)
    {
        vector<vector<string>> a;
        vector<vector<string>> b;
        for (int j = 2; j < G[i].size(); j++)
        {
            if ((G[i][j - 1] == "->" || G[i][j - 1] == "|") && G[i][j] == G[i][0]) // 该段语句以A开头，其后就加入a中
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