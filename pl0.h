#ifndef __pl0_H
#define __pl0_H

#include <bits/stdc++.h>
#include <stack>
#include <string>


#define norw 15 /* 保留字的数量 */ 
#define txmax 1000 /* 标识符表的长度 */ 
#define nmax 14 /* 数字允许的最长位数 */ 
#define al 10 /* 标识符最大长度 */
#define amax 2047 /* 寻址空间 */
#define levmax 3 /* 最大允许的嵌套层数 */


#define nul 0x1 /* 空 */
#define ident 0x2 /* 标识符 */
#define number 0x4 /* 数值 */
#define plus 0x8 /* + */
#define minus 0x10 /* - */
#define times 0x20 /* * */
#define slash 0x40 /* / */
#define oddsym 0x80 /* 奇数判断 */
#define eql 0x100 /* = */
#define neq 0x200 /* <> */
#define lss 0x400 /* < */
#define leq 0x800 /* <= */
#define gtr 0x1000 /* > */
#define geq 0x2000 /* >= */
#define lparen 0x4000 /* ( */
#define rparen 0x8000 /* ) */
#define comma 0x10000 /* , */
#define semicolon 0x20000 /* ; */
#define readsym 0x40000 
#define becomes 0x80000 /* := */ 
#define beginsym 0x100000
#define endsym 0x200000
#define ifsym 0x400000
#define thensym 0x800000
#define whilesym 0x1000000
#define dosym 0x2000000
#define callsym 0x4000000
#define constsym 0x8000000
#define varsym 0x10000000
#define procsym 0x20000000
#define elsesym 0x40000000
#define programsym 0x80000000
#define writesym 0x100000000 /* writesym超过long范围，需要long long型 */ 


char* err_msg[] =
{
/* 0 */ "",
/* 1 */ "Found '=' when expecting ':='.",
/* 2 */ "There must be a number to follow ':='.",
/* 3 */ "There must be an '=' to follow the identifier.",
/* 4 */ "There must be an identifier to follow 'const', 'var', 'program', 'read', 'procedure' or unexpected ','.",
/* 5 */ "Missing ',' or ';'.",
/* 6 */ "Incorrect procedure name.",
/* 7 */ "Statement expected.",
/* 8 */ "Follow the statement is an incorrect symbol.",
/* 9 */ "'.' expected.",
/* 10 */ "';' expected.",
/* 11 */ "Undeclared identifier.",
/* 12 */ "Illegal assignment.",
/* 13 */ "':=' expected.",
/* 14 */ "There must be an identifier to follow the 'call'.",
/* 15 */ "A constant or variable can not be called.",
/* 16 */ "'then' expected.",
/* 17 */ "';' or 'end' expected.",
/* 18 */ "'do' expected.",
/* 19 */ "Incorrect symbol.",
/* 20 */ "Relative operators expected.",
/* 21 */ "Procedure identifier can not be in an expression.",
/* 22 */ "Missing ')'.",
/* 23 */ "The symbol can not be followed by a factor.",
/* 24 */ "The symbol can not be as the beginning of an expression.",
/* 25 */ "Unexpected unrecognized characters.",//新加入 
/* 26 */ "Missing 'program'.",
/* 27 */ "Unsupported non-integer.",
/* 28 */ "Expected unqualified-id before numeric constant.",
/* 29 */ "Found ':' when expecting ':='.",
/* 30 */ "There must be an identifier to follow 'const'.",
/* 31 */ "The number is too great.",
/* 32 */ "There are too many levels.",
/* 33 */ "Missing 'begin'.",
/* 34 */ "Extra ending.",
/* 35 */ "Unexpected ';'.",
/* 36 */ "Unexpected ',' or missing identifier to follow ','." ,
/* 37 */ "Unexpected ',' or missing expression to follow ','." ,
/* 38 */ "Found number when expecting identifier.", //主要表达怎么处理多的不是ident内容
/* 39 */ "Missing statement part.",
/* 40 */ "Missing '('.",
/* 41 */ "There must be an expression to follow 'call', 'write' or unexpected ','." ,
/* 42 */ "The symbol can not be as the beginning of an statement.", //主要处理 ；开头和其余奇怪问题 
/* 43 */ "Missing 'procedure'.",
/* 44 */ "Unexpected constant description follows variable description.",
/* 45 */ "Unexpected constant description or variable description.",
/* 46 */ "Duplicate definition.",
/* 47 */ "There are too many symbols.",
/* 48 */ "Incorrect number of parameters"
};


/* P代码相关*/
/*
lit 0,a 取常量a放入栈顶
opr 0,a 执行运算（详见解释程序）
lod L,a 取变量a放入栈顶
sto L,a 将栈顶内容存入变量
cal L,a 调用过程a
Int 0,a 栈顶指针增加a
jmp 0,a 无条件跳转至a
jpc 0,a 条件跳转至a（为假跳转）
red L,a 读数据并存入变量
wrt 0,0 输出栈顶内容
*/
enum com
{
	lit, opr, lod, sto, cal, Int, jmp, jpc, red, wrt
};

typedef struct
{
	enum com f;	//伪操作码
	long l;	//层差
	long a;	//位移量
}command;

char codename[10][3 + 1] = {
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "RED", "WRT"
};//用于输出指令名


/*符号表相关*/
#define txmax 50000 //符号表最大容量
#define dxmax 5000 // 层次表最大容量
enum symtype
{
	con, var, pro, cav//cav = con and var
};
//常量永远不变 不可输入，变量仅在运行时才赋值，编译符号表阶段不管

struct t//静态表，无哈希
{
	symtype kind;
	char name[al+1];
	long val; //pro 用于传过程CAL后转向的传参STO代码地址
	long level;
	long addr;
	long size;
	long previous;
};

t table[txmax + 1];

long display[dxmax + 1];//显示层次关系表
long tx = 0;//符号表栈顶寄存器(但不一定是最新)
long sx;//同层最初符号表基地址寄存器 用于恢复
long dx;//层次表栈顶寄存器
long lastx = 0;//previous
long llastx = 0;//previous of previous



char ch; // 用于词法分析器，存放最近一次从文件中读出的字符
unsigned long long sym; // 词法分析器输出结果之用，存放最近一次识别出来的 token 的类型
char id[al+1]; // 词法分析器输出结果之用，存放最近一次识别出来的标识符的名字
long num; // 词法分析器输出结果之用，存放最近一次识别出来的数字的值
long cc; // 行缓冲区的列指针
long ll; // 行缓冲区长度
long err; /* 出错总次数 */
long row;//出错指示行数
long column;//出错指示列数
long l;//目前词法分析中的词长度
long lev;//当前层深
long rowtemp,coltemp,rowlast,collast;//用于暂时存储上一个词语位置变量用于报错
long ddx;//用于计算当前变量运行栈中相对于过程基地址的偏移量


char line[81]; /* 行缓冲区，用于从文件读出一行，供词法分析获取单词时之用 */
char a[al + 1]; /* 词法分析器中用于临时存放正在分析的词 */ 
//instruction code[cxmax+1];/* 生成的类 PCODE 代码表，存放编译得到的类 PCODE 代码 */ 
char word[norw][al + 1] = {
	"begin", "call", "const", "do", "else", "end", "if", "odd",
	"procedure", "program", "read", "then", "var", "while", "write"
};/* 保留字表 */
unsigned long long wsym[norw] = {
	beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym,
	procsym, programsym, readsym, thensym, varsym, whilesym, writesym
};/* 保留字表中每一个保留字对应的 symbol 类型 */
unsigned long long ssym[256];/*一些符号对应的 symbol 类型表*/
char mnemonic[8][3 + 1]; /* 类 PCODE 指令助记符表 */
unsigned long long declbegsys, statbegsys, facbegsys, lopbegsys; /* 声明开始、表达式开始和项开始符号集合 */

char infilename[256];
FILE* infile;


/*假想机结构*/
#define pcmax 2000 /* 目标代码数组的长度 */
#define stacksize 5000
command code[pcmax + 1];
long datastk[stacksize];

command index;//指令寄存器
long sp;//栈顶寄存器（书中top）从0开始，指向已有的栈顶
long bp;//基地址寄存器(书中sp) 从0开始
long pc;//程序地址寄存器 从0开始，指向下一条没填的程序


#endif 
