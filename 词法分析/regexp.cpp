#include "regexp.h"
#include <map>
#include <stack>
using namespace std;

RegExp::RegExp(string exp)
{
    this->infixExp = exp;
    mp['('] = mp[')'] = 4;
    mp['*'] = 3; // 闭包
    mp['.'] = 2; // 连接
    mp['|'] = 1; // 选择
    for (int i = 0; i < exp.size(); i++)
        if (!(exp[i] == '|' || exp[i] == '*' || exp[i] == '.' || exp[i] == '(' || exp[i] == ')'))
            chs.insert(exp[i]);
}
void RegExp::to_suffixExp()
{
    stack<char> stk; // 操作符栈
    string suffix;
    for (int i = 0; i < infixExp.size(); i++)
    {
        if (infixExp[i] == '*' || infixExp[i] == '.' || infixExp[i] == '|')
        {
            if (!stk.empty() && mp[infixExp[i]] <= mp[stk.top()]) // 操作符号栈不空且栈顶元素优先级低于当前操作符 弹栈
            {
                while (!stk.empty() && stk.top() != '(')
                {
                    suffix.push_back(stk.top());
                    stk.pop();
                }
            }
            stk.push(infixExp[i]);
        }
        else if (infixExp[i] == '(') // 遇到左括号就压栈
            stk.push(infixExp[i]);
        else if (infixExp[i] == ')') // 遇到右括号就出栈，直到栈顶元素为左括号
        {
            while (stk.top() != '(')
            {
                suffix.push_back(stk.top());
                stk.pop();
            }
            stk.pop();
        }
        else // 字符
            suffix.push_back(infixExp[i]);
    }
    while (!stk.empty())
    {
        suffix.push_back(stk.top());
        stk.pop();
    }
    this->suffixExp = suffix;
}