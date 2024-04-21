# 待优化：

- 修改语法之提取左因子
- 完善语言规则
- 完善语法分析器

## 1. LL(1)

### （1）测试元素: 

- 语法: GRAMMAR.txt

```
expr -> expr addop term | term 
addop -> + | - 
term -> term mulop factor | factor 
mulop -> * 
factor -> ( expr ) | number 
```
- 输入串: test1.txt

```
( --- (
46 --- number
+ --- +
32 --- number
) --- )
* --- *
2 --- number
```

## 2. LR(0)

### （1）测试元素: 

- 语法: lr0test.txt

```
S -> a A | b B 
A -> c A | d 
B -> c B | d 
```
- 输入串: test2.txt

```
a --- a
c --- c
c --- c
d --- d
```

