#include "contextfreegrammar.h"
#include "rwfile.h"
using namespace std;

void ContextFreeGrammar::get_grammer(vector<string> grammar)
{
    for (int i = 0; i < grammar.size(); i++)
    {
        bool findNonTerminal = false;
        string nonTerminalForRaw;
        string cur = "";
        for (int j = 0; j < grammar[i].size(); j++)
        {
            if (grammar[i][j] != ' ')
                cur = cur + grammar[i][j];
            else
            {
                if (!findNonTerminal)
                {
                    findNonTerminal = true;
                    nonTerminalForRaw = cur;
                    this->grammar.push_back(vector<string>());
                    this->grammar[this->grammar.size() - 1].push_back(cur);
                    this->nonTerminal.insert(make_pair(cur, 1)); // 记录非终结符
                }
                else
                {
                    if (cur != "|" && cur != "->")
                        this->grammar[this->grammar.size() - 1].push_back(cur);
                    else if (cur == "|") // 遇到'|'将语句拆分成两句
                    {
                        this->grammar.push_back(vector<string>());
                        this->grammar[this->grammar.size() - 1].push_back(nonTerminalForRaw);
                    }
                }
                cur = "";
            }
        }
    }
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
    while (this->follow.size() < this->nonTerminal.size())
    {
        for (int i = 0; i < this->grammar.size(); i++)
        {
            for (int j = 1; j < this->grammar[i].size(); j++)
            {
                if (nonTerminal.find(this->grammar[i][j]) != nonTerminal.end()) // 是非终结符
                {
                    if (grammar[i][j] == grammar[0][0])                                                                                                                                                         // 开始符
                        this->follow[this->grammar[i][j]].insert("$");                                                                                                                                          // 添加'$'
                    if (j == this->grammar[i].size() - 1 || nonTerminal.find(this->grammar[i][j + 1]) != nonTerminal.end() && this->first[grammar[i][j + 1]].find("#") != this->first[grammar[i][j + 1]].end()) // 后面没有字符或者下一个非终结符的first集含有空
                    {
                        if (this->follow.find(this->grammar[i][0]) != this->follow.end()) // 该非终结符的follow集已经求出
                            this->follow[this->grammar[i][j]].insert(this->follow[this->grammar[i][0]].begin(), this->follow[this->grammar[i][0]].end());
                    } // 添加该非终结符的follow集
                    else if (nonTerminal.find(this->grammar[i][j]) == nonTerminal.end())
                    {
                        this->follow[this->grammar[i][j]].insert(this->grammar[i][j + 1]);
                    }
                    else
                    {
                        for (auto item : this->first[grammar[i][j + 1]])
                            this->follow[this->grammar[i][j]].insert(item);
                    }
                }
            }
        }
    }
}