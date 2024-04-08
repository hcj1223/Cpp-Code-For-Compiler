#include "LL1.h"
#include <iostream>
using namespace std;

void LL1::init_LL1(vector<string> grammar)
{
    ContextFreeGrammar cfg;
    cfg.get_grammer(grammar);
    cfg.get_first();
    cfg.get_follow();
    this->cfg = cfg;

    cout << "Grammar: " << endl;
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
    for (auto it : cfg.first)
    {
        cout << it.first << ": { ";
        for (auto it2 : it.second)
            cout << it2 << " ";
        cout << "}" << endl;
    }
    cout << endl;

    cout << "Follow Set: " << endl;
    for (auto it : cfg.follow)
    {
        cout << it.first << ": { ";
        for (auto it2 : it.second)
            cout << it2 << " ";
        cout << "}" << endl;
    }
}
void LL1::run_LL1()
{
}