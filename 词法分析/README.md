## 1.头文件

### （1）rwfile.h

```c++
vector<string> read_file(string fname);                 // 返回文件内容
void write_file(string fname, vector<string> contents); // 将内容写入文件
void write_file(string fname, string content);
```

### （2）regexp.h

```c++
string infixExp;   // 正则表达式的中缀表达式形式
string suffixExp;  // 正则表达式的后缀表达式形式
map<char, int> mp; // 运算符的优先级
set<char> chs;     // 正则表达式中出现过的字符
```

```c++
void to_suffixExp(); // 将中缀表达式转化为后缀表达式
```

### （3）nfa.h

```c++
vector<unordered_map<char, vector<int>>> graph; // NFA图
int start, end;                                 // NFA图的起点和终点
```

```c++
NFA merge_nfa(NFA other); // 将两图放入同一NFA中，改写编号
NFA build_NFA(RegExp regExp); // 从正则表达式到NFA
```

### （4）dfa.h

```c++
vector<unordered_map<char, int>> graph; // DFA图
int start;                              // DFA的起点
vector<int> end;                        // 终点集合
```

```c++
set<int> get_e_closure(vector<unordered_map<char, vector<int>>> graph, int k);          // 返回e闭包
set<int> get_move(vector<unordered_map<char, vector<int>>> graph, int k, char ch);      // 返回经过ch弧的下一个节点
set<int> get_move(vector<unordered_map<char, vector<int>>> graph, set<int> s, char ch); // 返回集合s经过ch弧的下一个节点集合的e闭包
DFA build_DFA(RegExp regExp); // 从NFA到DFA
```

### （5）mindfa.h

```c++
vector<unordered_map<char, int>> graph; // minDFA图
int start;                              // 起点
set<int> end;                           // 终点
```

```c++
minDFA build_min_DFA(RegExp regExp); // 从DFA到最小化DFA
```

### （6）lex.h

```c++
string inputFileName;
string outputFileName;
string lexErrorFileName;
vector<string> regExps;
vector<minDFA> minDfa;
vector<string> keyWords;      // 关键字
vector<string> specialSymbol; // 特殊符号
```

```c++
Lex(string inputFileName, string outputFileName, string lexErrorFileName, string regExpsFileName, string keyWordsFileName, string specialSymbolFileName);
int is_right_string(string str); // 判断字符串是否符合正则表达式
bool is_special_symbol(char ch);
```

## 2.输入输出文件

### （1）TEST.txt：源代码文件

### （2）LEXRES.txt：经过词法分析后得到的结果文件

### （3）LEXERROR.txt：经过词法分析后得到的错误

### （4）REGEXP.txt：正则表达式文件

### （5）KEYWORD.txt：关键字文件

### （6）SPECIALSYMBOL.txt：特殊符号文件

## 3.测试

- main.cpp
- 测试源代码

<img src="C:/Users/123/AppData/Roaming/Typora/typora-user-images/image-20240319164946399.png" alt="image-20240319164946399" style="zoom:130%;" /> 

- 测试结果

![image-20240319164930072](C:/Users/123/AppData/Roaming/Typora/typora-user-images/image-20240319164930072.png) 

<img src="C:/Users/123/AppData/Roaming/Typora/typora-user-images/image-20240319160209471.png" alt="image-20240319160209471" style="zoom:130%;" /> 

## 4.说明

- 错误修正机制：遇到错误该行不再读入
- 在正则表达式中'&'表示[0-9]，'@'表示[a-zA-Z]

## 5.总结

- 关键字和特殊字符未完善，相应的词法分析程序可以进一步修改，等待自定义出自己的语言再进行改进
