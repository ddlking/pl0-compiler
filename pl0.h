#ifndef __pl0_H
#define __pl0_H

#include <bits/stdc++.h>
#include <stack>
#include <string>


#define norw 15 /* �����ֵ����� */ 
#define txmax 1000 /* ��ʶ����ĳ��� */ 
#define nmax 14 /* ����������λ�� */ 
#define al 10 /* ��ʶ����󳤶� */
#define amax 2047 /* Ѱַ�ռ� */
#define levmax 3 /* ��������Ƕ�ײ��� */


#define nul 0x1 /* �� */
#define ident 0x2 /* ��ʶ�� */
#define number 0x4 /* ��ֵ */
#define plus 0x8 /* + */
#define minus 0x10 /* - */
#define times 0x20 /* * */
#define slash 0x40 /* / */
#define oddsym 0x80 /* �����ж� */
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
#define writesym 0x100000000 /* writesym����long��Χ����Ҫlong long�� */ 


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
/* 25 */ "Unexpected unrecognized characters.",//�¼��� 
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
/* 38 */ "Found number when expecting identifier.", //��Ҫ�����ô�����Ĳ���ident����
/* 39 */ "Missing statement part.",
/* 40 */ "Missing '('.",
/* 41 */ "There must be an expression to follow 'call', 'write' or unexpected ','." ,
/* 42 */ "The symbol can not be as the beginning of an statement.", //��Ҫ���� ����ͷ������������� 
/* 43 */ "Missing 'procedure'.",
/* 44 */ "Unexpected constant description follows variable description.",
/* 45 */ "Unexpected constant description or variable description.",
/* 46 */ "Duplicate definition.",
/* 47 */ "There are too many symbols.",
/* 48 */ "Incorrect number of parameters"
};


/* P�������*/
/*
lit 0,a ȡ����a����ջ��
opr 0,a ִ�����㣨������ͳ���
lod L,a ȡ����a����ջ��
sto L,a ��ջ�����ݴ������
cal L,a ���ù���a
Int 0,a ջ��ָ������a
jmp 0,a ��������ת��a
jpc 0,a ������ת��a��Ϊ����ת��
red L,a �����ݲ��������
wrt 0,0 ���ջ������
*/
enum com
{
	lit, opr, lod, sto, cal, Int, jmp, jpc, red, wrt
};

typedef struct
{
	enum com f;	//α������
	long l;	//���
	long a;	//λ����
}command;

char codename[10][3 + 1] = {
	"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "RED", "WRT"
};//�������ָ����


/*���ű����*/
#define txmax 50000 //���ű��������
#define dxmax 5000 // ��α��������
enum symtype
{
	con, var, pro, cav//cav = con and var
};
//������Զ���� �������룬������������ʱ�Ÿ�ֵ��������ű�׶β���

struct t//��̬���޹�ϣ
{
	symtype kind;
	char name[al+1];
	long val; //pro ���ڴ�����CAL��ת��Ĵ���STO�����ַ
	long level;
	long addr;
	long size;
	long previous;
};

t table[txmax + 1];

long display[dxmax + 1];//��ʾ��ι�ϵ��
long tx = 0;//���ű�ջ���Ĵ���(����һ��������)
long sx;//ͬ��������ű����ַ�Ĵ��� ���ڻָ�
long dx;//��α�ջ���Ĵ���
long lastx = 0;//previous
long llastx = 0;//previous of previous



char ch; // ���ڴʷ���������������һ�δ��ļ��ж������ַ�
unsigned long long sym; // �ʷ�������������֮�ã�������һ��ʶ������� token ������
char id[al+1]; // �ʷ�������������֮�ã�������һ��ʶ������ı�ʶ��������
long num; // �ʷ�������������֮�ã�������һ��ʶ����������ֵ�ֵ
long cc; // �л���������ָ��
long ll; // �л���������
long err; /* �����ܴ��� */
long row;//����ָʾ����
long column;//����ָʾ����
long l;//Ŀǰ�ʷ������еĴʳ���
long lev;//��ǰ����
long rowtemp,coltemp,rowlast,collast;//������ʱ�洢��һ������λ�ñ������ڱ���
long ddx;//���ڼ��㵱ǰ��������ջ������ڹ��̻���ַ��ƫ����


char line[81]; /* �л����������ڴ��ļ�����һ�У����ʷ�������ȡ����ʱ֮�� */
char a[al + 1]; /* �ʷ���������������ʱ������ڷ����Ĵ� */ 
//instruction code[cxmax+1];/* ���ɵ��� PCODE �������ű���õ����� PCODE ���� */ 
char word[norw][al + 1] = {
	"begin", "call", "const", "do", "else", "end", "if", "odd",
	"procedure", "program", "read", "then", "var", "while", "write"
};/* �����ֱ� */
unsigned long long wsym[norw] = {
	beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym,
	procsym, programsym, readsym, thensym, varsym, whilesym, writesym
};/* �����ֱ���ÿһ�������ֶ�Ӧ�� symbol ���� */
unsigned long long ssym[256];/*һЩ���Ŷ�Ӧ�� symbol ���ͱ�*/
char mnemonic[8][3 + 1]; /* �� PCODE ָ�����Ƿ��� */
unsigned long long declbegsys, statbegsys, facbegsys, lopbegsys; /* ������ʼ�����ʽ��ʼ���ʼ���ż��� */

char infilename[256];
FILE* infile;


/*������ṹ*/
#define pcmax 2000 /* Ŀ���������ĳ��� */
#define stacksize 5000
command code[pcmax + 1];
long datastk[stacksize];

command index;//ָ��Ĵ���
long sp;//ջ���Ĵ���������top����0��ʼ��ָ�����е�ջ��
long bp;//����ַ�Ĵ���(����sp) ��0��ʼ
long pc;//�����ַ�Ĵ��� ��0��ʼ��ָ����һ��û��ĳ���


#endif 
