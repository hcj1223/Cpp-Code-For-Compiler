#ifndef REGEXP_H
#define REGEXP_H

#include <string>
#include <map>
#include <set>
using namespace std;

class RegExp
{
public:
    string infixExp;   // 中缀表达式
    string suffixExp;  // 后缀表达式
    map<char, int> mp; // 优先级
    set<char> chs;     // 出现过的字符

public:
    RegExp(string exp);
    void to_suffixExp(); // 将中缀表达式转化为后缀表达式
};

#endif