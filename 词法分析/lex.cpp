#include <vector>
#include <string>
#include <iostream>
#include "RegExp.h"
#include "minDFA.h"
#include "Lex.h"
#include "rwfile.h"
using namespace std;

Lex::Lex(string inputFileName, string outputFileName, string lexErrorFileName, string regExpsFileName, string keyWordsFileName, string specialSymbolFileName)
{
    // 初始化
    this->inputFileName = inputFileName;
    this->outputFileName = outputFileName;
    this->lexErrorFileName = lexErrorFileName;
    regExps = read_file(regExpsFileName);
    keyWords = read_file(keyWordsFileName);
    for (auto k : regExps)
    {
        RegExp regExp(k);
        regExp.to_suffixExp();
        minDfa.push_back(build_min_DFA(regExp));
    }
    specialSymbol = read_file(specialSymbolFileName);
    // 读写文件
    vector<string> sourceFile = read_file(inputFileName);
    for (int i = 0; i < sourceFile.size(); i++)
    {
        string curStr = "";
        for (int j = 0; j < sourceFile[i].size(); j++)
        {
            if (sourceFile[i][j] == ' ' || is_special_symbol(sourceFile[i][j]))
            {
                string word = curStr;
                curStr = "";
                if (word == "" && sourceFile[i][j] == ' ')
                    continue;
                bool isKeyWord = 0;
                for (int k = 0; k < keyWords.size(); k++)
                    if (word == keyWords[k])
                    {
                        isKeyWord = 1;
                        write_file(outputFileName, word + " --- " + "keyword");
                        break;
                    }
                if (isKeyWord)
                    continue;
                if (word != "")
                {
                    int rank = is_right_string(word);
                    if (rank == 0)
                        write_file(outputFileName, string(word + " --- " + "num"));
                    else if (rank == 1)
                        write_file(outputFileName, string(word + " --- " + "id"));
                    else if (rank == -1)
                    {
                        write_file(lexErrorFileName, word + " --- " + "error");
                        break; // 错误修正机制，遇到错误该行不再读入
                    }
                }
                if (is_special_symbol(sourceFile[i][j]))
                {
                    string message;
                    message.push_back(sourceFile[i][j]);
                    message = message + +" --- " + "specialsymbol";
                    write_file(outputFileName, message);
                }
            }
            else
                curStr = curStr + sourceFile[i][j];
        }
    }
}
bool Lex::is_special_symbol(char ch)
{
    for (int i = 0; i < specialSymbol[0].size(); i++)
        if (ch == specialSymbol[0][i])
            return 1;
    return 0;
}
int Lex::is_right_string(string str)
{
    for (int k = 0; k < minDfa.size(); k++)
    {
        int state = minDfa[k].start;
        bool find_break_edge = 0;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z')
            {
                if (minDfa[k].graph[state].count('@'))
                    state = minDfa[k].graph[state]['@'];
                else
                {
                    find_break_edge = 1;
                    break;
                }
            }
            else if (str[i] >= '0' && str[i] <= '9')
            {
                if (minDfa[k].graph[state].count('&'))
                    state = minDfa[k].graph[state]['&'];
                else
                {
                    find_break_edge = 1;
                    break;
                }
            }
        }
        if (!find_break_edge)
            for (auto i : minDfa[k].end)
                if (i == state)
                    return k;
    }
    return -1;
}