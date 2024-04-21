#include "parser.h"
#include "rwfile.h"
#include <iostream>
#include <stack>
using namespace std;

void Parser::init_parser(string fileName, int k)
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

    switch (k)
    {
    case 0:
        for (int i = 0; i < grammars.size(); i++)
        {
            LL1 aLL1;
            aLL1.init_LL1(grammars[i]);
            aLL1.get_LL1_table();
            this->LL1s.push_back(aLL1);
        }
        break;

    case 1:
        for (int i = 0; i < grammars.size(); i++)
        {
            LR0 aLR0;
            aLR0.init_LR0(grammars[i]);
            aLR0.get_LR0_dfa();
            aLR0.get_LR0_table();
            this->LR0s.push_back(aLR0);
        }
        break;
    }
}
int Parser::run_parser_LL1(string fileName)
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
int Parser::run_parser_LR0(string fileName)
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

    for (int i = 0; i < exp.size(); i++)
        cout << exp[i] << " ";
    cout << endl;

    for (int i = 0; i < LR0s.size(); i++)
    {
        stack<string> parserStack;
        parserStack.push("$");
        parserStack.push("0");

        ContextFreeGrammar cfg = LR0s[i].cfg;
        map<int, map<string, string>> action = LR0s[i].action;
        map<int, map<string, int>> goTo = LR0s[i].goTo;

        int ip = 0;
        while (1)
        {
            auto f = parserStack.top();

            // 将string类型的状态转换为int类型
            int s = 0;
            for (int j = 0; j < f.size(); j++)
                s = s * 10 + (f[j] - '0');

            if (action[s].find(exp[ip]) != action[s].end()) // action[s,a]存在
            {
                string str = action[s][exp[ip]];
                if (str[0] == 's') // shift s'
                {
                    parserStack.push(exp[ip++]);     // 将a压栈
                    parserStack.push(str.substr(1)); // 将s'压栈
                }
                else if (str[0] == 'r') // reduce A -> B
                {
                    int grammarId = str[1] - '0';
                    int len = 2 * (cfg.grammar[grammarId].size() - 1); // 两倍的产生式右部长度
                    for (int j = 0; j < len; j++)
                        parserStack.pop(); // 弹出元素

                    auto ff = parserStack.top();
                    int ss = 0; // 当前栈顶状态s'
                    for (int j = 0; j < ff.size(); j++)
                        ss = ss * 10 + (ff[j] - '0');

                    parserStack.push(cfg.grammar[grammarId][0]);                      // 将A压栈
                    parserStack.push(to_string(goTo[ss][cfg.grammar[grammarId][0]])); // 将s'压栈
                }
                else if (str == "acc") // accept
                    return i;          // 符合语法
            }
            else
                break;
        }
    }

    return -1; // 不符合任何一个语法
}