#include "parser.h"
#include "rwfile.h"
#include <iostream>
#include <stack>
using namespace std;

void Parser::init_parser(string fileName)
{
    vector<string> contents = read_file(fileName);
    vector<vector<string>> grammars;
    grammars.push_back(vector<string>());
    for (int i = 0; i < contents.size(); i++)
    {
        if (contents[i] == "---")
            grammars.push_back(vector<string>());
        else
            grammars.back().push_back(contents[i]);
    }
    for (int i = 0; i < grammars.size(); i++)
    {
        LL1 aLL1;
        aLL1.init_LL1(grammars[i]);
        aLL1.run_LL1();
        this->LL1s.push_back(aLL1);
    }
}
int Parser::run_parser(string fileName)
{
    vector<string> contents = read_file(fileName);
    vector<string> exp;
    for (int i = 0; i < contents.size(); i++)
    {
        string cur = "";
        for (int j = 0; j < contents[i].size(); j++)
            if (contents[i][j] == '-' && contents[i][j + 1] == '-' && contents[i][j + 2] == '-')
            {
                for (int k = j + 4; k < contents[i].size(); k++)
                    cur = cur + contents[i][k];
                exp.push_back(cur);
                break;
            }
    }
    exp.push_back("$");

    /* for (int i = 0; i < exp.size(); i++)
        cout << exp[i] << " ";
    cout << endl; */

    for (int i = 0; i < LL1s.size(); i++)
    {
        stack<string> parserStack;
        parserStack.push("$");
        parserStack.push(LL1s[i].cfg.grammar[0][0]);
        int itExp = 0;
        while (parserStack.top() != "$")
        {
            string top = parserStack.top();
            parserStack.pop();

            // cout << top << endl;

            if (top == "#")
                continue;
            if (LL1s[i].cfg.terminal.find(top) != LL1s[i].cfg.terminal.end()) // 终结符
            {
                if (top == exp[itExp]) // 和表达式匹配上
                    itExp++;
                else
                    break; // 匹配失败
            }
            else if (LL1s[i].LL1Table[top].find(exp[itExp]) != LL1s[i].LL1Table[top].end()) // 存在推导
            {
                int gammarId = LL1s[i].LL1Table[top][exp[itExp]];
                for (int ii = LL1s[i].cfg.grammar[gammarId].size() - 1; ii >= 1; ii--)
                    parserStack.push(LL1s[i].cfg.grammar[gammarId][ii]);
            }
            else
                break;
        }
        if (parserStack.top() == "$")
            return i; // 符合语法
    }

    return -1; // 不符合任何一个语法
}