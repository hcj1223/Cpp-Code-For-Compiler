#ifndef PARSE_H
#define PARSE_H

#include "LL1.h"
using namespace std;

class Parser
{
public:
    vector<LL1> LL1s;

public:
    void init_parser(string fileName);
    int run_parser(string fileName);
};

#endif