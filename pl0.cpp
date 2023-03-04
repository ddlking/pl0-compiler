#include"pl0.h"
#include<bits/stdc++.h>

using namespace std;


void error(long n)
{
	if(n == 5 || n == 35 || n == 39)
		cout<<"["<<rowlast<<","<<collast<<"] ";
	else
		cout<<"["<<row<<","<<column<<"] ";
	cout<<"Error("<<n<<")-> "<<err_msg[n]<<"\n";//eg:Error(1)-> Found ':=' when expecting '='.
 	err++;
}


/* 读取下一个字符过程 getch( );*/ 
void getch()
{
	if(cc == ll)
	{
		if(feof(infile))
		{
			cout<<"Program incomplete!\n";
			
			if(err == 0)
				cout<<"There is no error in program!\n";
			else
				cout<<err<<" errors in PL/0 program!\n";
			exit(1);
		}
		
		row++;//指示行数
		column = 0;
		ll = 0;
		cc = 0;
		while( (!feof(infile)) && ( (ch=getc(infile)) != '\n') )
		{
			cout<<ch;
			ll++;
			line[ll] = ch; 
		}
		cout<<"\n";
		if(feof(infile))
			ll--;//因为EOF会读进去走最后一轮 
		
		ll++;
		line[ll] = ' ';//每一行结尾空格
	}
	//正常每次从行缓冲区调取一个字符
	
	column++;
	cc++;
	ch = line[cc];
}


/* 词法分析过程 */
void getsym()
{
	collast = coltemp;
	rowlast = rowtemp;
	
	while(ch == ' ' || ch == '\t')
		getch();
	
	l = 0;
	if(isalpha(ch)) //token为字母,则判断标识符或保留字
	{
		l = 0; //l为长度
		memset(a, 0, sizeof a);
		while(isalpha(ch) || isdigit(ch)) //可以是字母或数字
		{
			if(l < al)
			{
				a[l] = ch;
				l++;
			}
			getch();
		}
		l--;
	
		/*
		//尾部字符处理 ?
		if(l >= kk)
			kk == l;
		else
		{
			while(l < kk)
			{
				kk--;
				a[kk] = ' ';
			}
		}
		*/
		
		memset(id, 0, sizeof id);
		strcpy(id, a);//id存放当前串名
	
		long i, j, k; 
		i = 0;
		j = norw - 1;//norw为关键字数量上限 
	
		//查关键字表 
		while(i <= j)
		{
			k = (i+j)/2;
			
			if(strcmp(id, word[k]) <= 0)
				j = k - 1;
			
			if(strcmp(id, word[k]) >= 0)
				i = k + 1;
				
		}
		
		if(i - 1 > j)
			sym = wsym[k];//关键字
		else
			sym = ident;//标识符
			
		rowtemp = row;
		coltemp = column;
	}
	
	
	else if(isdigit(ch))
	{
		l = 0;
		sym = number;
		num = 0;
		
		while(isdigit(ch))
		{
			num = num*10 + (ch - '0');
			l++;
			getch();
		}
		
		if(isalpha(ch))//数字开头的标识符，省略数字 
		{
			error(28);
			getsym();
		} 
		
		if(ch == '.')//PL/0仅支持整数 
		{
			error(27);
			getch();
			while(isdigit(ch))
				getch(); //去除小数部分 
		}
		
		if(l > nmax)
			error(31);
			
		rowtemp = row;
		coltemp = column;
	}
	
	
	else if(ch == ':')
	{
		a[l] = ch;
		getch();
		
		if(ch == '=')
		{
			a[++l] = ch; 
			sym = becomes;
			getch();
		}
		else
		{
			error(29);//词法不会出现单独‘：’ 
			a[++l] = '=';
			sym = becomes;
		}
		
		rowtemp = row;
		coltemp = column;
	}
	
	
	else if(ch == '<')
	{
		a[l] = ch;
		getch();
		
		if(ch == '=')
		{
			a[++l] = ch;
			sym = leq;
			getch();
		}
		else if(ch == '>')
		{
			a[++l] = ch;
			sym = neq;
			getch();
		}
		else
			sym = lss;
			
		rowtemp = row;
		coltemp = column;
	}
	
	
	else if(ch == '>')
	{
		a[l] = ch;
		getch();
		
		if(ch == '=')
		{
			a[++l] = ch;
			sym = geq;
			getch();
		}
		else
			sym = gtr;
			
		rowtemp = row;
		coltemp = column;
	}
	
	
	else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' 
	|| ch == '(' || ch == ')' || ch == ',' || ch == ';') 
	{
		a[l] = ch;
		sym = ssym[(unsigned char)ch];
		getch();
		
		rowtemp = row;
		coltemp = column;
	}
	
	else
	{
		error(25);
		a[l] = ' ';
		getch();
		getsym();
		//sym = nul;//错误字符  其余字符不记录位置 
	}
	 
}
/* 词法分析过程 getsym 总结：从源文件中读出若干有效字符，组成一个 token 串，识别它的类型
为保留字/标识符/数字或是其它符号。如果是保留字，把 sym 置成相应的保留字类型，如果是
标识符，把 sym 置成 ident 表示是标识符，于此同时，id 变量中存放的即为保留字字符串或标识
符名字。如果是数字，把 sym 置为 number,同时 num 变量中存放该数字的值。如果是其它的操作符，
则直接把 sym 置成相应类型。经过本过程后 ch 变量中存放的是下一个即将被识别的字符 */

/* 测试当前单词是否合法过程 */
/* 参数：s1:当语法分析进入或退出某一语法单元时当前单词符合应属于的集合 */
/* s2:在某一出错状态下，可恢复语法分析正常工作的补充单词集合 */
/* n:出错信息编号，当当前符号不属于合法的 s1 集合时发出的出错信息 */
void test(unsigned long s1, unsigned long s2, long n)
{
	if(!(sym & s1))
	{
		error(n);
		s1 = s1 | s2;
	
		while(!(sym & s1))
			getsym();
	}
}


//void enter(enum object k) //登陆符号表


//long position(char* id) //查询符号表


void program(unsigned long fsys);
void block(unsigned long fsys);
void constdeclaration();
void vardeclaration();
void proc(unsigned long fsys);
void body(unsigned long fsys);
void statement(unsigned long fsys);
void condition(unsigned long fsys);
void expression(unsigned long fsys);
void term(unsigned long fsys);
void factor(unsigned long fsys);



void program(unsigned long fsys) //简单报错，入参仅传给block用 
{
	getsym();
	if(sym == programsym)
	{
		getsym();
		if(sym == ident)
		{
			getsym();
			if(sym == semicolon)
			{
				getsym();
				block(fsys);
			}
			
			else
				error(10); 
		}
		else
			error(4);
	}
	else
		error(26);
	
}


void block(unsigned long fsys)
{
	if(lev > levmax)
		error(32);
	
	if(sym == constsym)
	{
		getsym();
		constdeclaration();
	}
	
	if(sym == varsym)
	{
		getsym();
		vardeclaration();
	}
	
	if(sym == procsym)
	{
		getsym();
		proc(fsys);
	}
	//没考虑多个和错序问题 堆叠问题
	if(sym == beginsym)
	{
		getsym();
	
	}
	else
		error(33);
	
	body(fsys);
}
 

void constdeclaration()//简单恢复，const识别在block 
{
	if(sym == ident)
	{
		getsym();
		
		if(sym == eql || sym == becomes)
		{
			if(sym == eql)
				error(1);
			
			getsym();
			
			if(sym == number)
			{
				//enter(constant);
				getsym();
			}
			else
				error(2);
		}
		
		else
			error(3);
	}
	
	else
		error(4);
	
	while(sym == comma)
	{
		getsym();
		if(sym == ident)
		{
			getsym();
		
			if(sym == eql || sym == becomes)
			{
				if(sym == eql)
					error(1);
			
				getsym();
			
				if(sym == number)
				{
					//enter(constant);
					getsym();
				}
				else
					error(2);
			}
		
			else
				error(3);
		}
	
		else
			error(36);
	}
	
	if(sym==semicolon)
		getsym();
	else
		error(5);
}


void vardeclaration()//简单恢复，var识别在block 
{
	if(sym == ident)
	{
		//enter(variable);
		getsym();
		
	}
	else
		error(4);
	
	while(sym == comma)
	{
		getsym();
		
		if(sym == ident)
		{
			getsym();
		}
		else
			error(36);
	}
	
	if(sym==semicolon)
		getsym();
	else
		error(5);
}


void proc(unsigned long fsys)//procedure识别在block 
{
	if(sym == ident)
	{
		getsym();
		
	}
	
	else
		error(4);
		
	if(sym == lparen)
	{
		getsym();
		
		if(sym == ident)
		{
			getsym();
			 
			while(sym == comma)
			{
				getsym();
				if(sym == ident)
				{
					getsym();
				}
				else
					error(36);
			}
			
		}
		test(ident|rparen, fsys|comma|semicolon, 4); //此处涉及 括号内有内容但开头不对； 括号无内容且少括号
		while(sym == comma)
		{
			getsym();
			if(sym == ident)
			{
				getsym();
			}
			else
				error(36);
		}
		
		if(sym == rparen)
		{
			getsym();
		}
		else
			error(22);
	}
		
	//else 左括号 
	if(sym == semicolon)
		getsym();
	else
		error(10);
	
	block(fsys|semicolon);
	
	while(sym == semicolon)
	{
		getsym();
		
		if(sym == procsym)
		{
			getsym();
			proc(fsys);
		}
		else
			error(33);
	}
	//少了分号怎么办 
	
	lev++;
}

void body(unsigned long fsys)//begin判断在block和statement中 
{
	if(sym == endsym)//缺少statement部分 
	{
		error(39);
		getsym();
		
		return;
	}
	
	//test(statbegsys|endsym, fsys|semicolon, 42); //主要处理分号开头
	
	
	statement(fsys|semicolon|endsym);
	
	if(sym == endsym)
	{
		getsym();
	}
	 

	else
	{
		if(sym == semicolon)
		{
			getsym();
			body(fsys|semicolon|endsym);
		}
		else
		{
			test(fsys,statbegsys,17);
			
			body(fsys|semicolon|endsym);
		}
	}


}


void expression(unsigned long fsys)
{
	unsigned long addop;
	
	if(sym == plus || sym == minus)
	{
		addop = sym;
		getsym();
		
		term(fsys|plus|minus);
	}
	
	else
	{
		term(fsys|plus|minus);
	}
	
	while(sym == plus || sym == minus)
	{
		addop = sym;
		getsym();
		
		term(fsys|plus|minus);
	}
}


void factor(unsigned long fsys)
{
	long i;
	
	test(facbegsys, fsys, 24);
	
	//while?
	if(sym == ident)
	{
		//i = position(id);
		getsym();
	}
	
	else if(sym == number)
	{
		if(num > amax)
		{
			error(31);
			num = 0;
		}
		getsym();
	}
	
	else if(sym == lparen)
	{
		getsym();
		expression(rparen|fsys);
		
		if(sym == rparen)
			getsym();
		else
			error(22);
			
		test(fsys,lparen,23);
	}
	
	
}


void term(unsigned long fsys)
{
	unsigned long mulop;
	
	factor(fsys|times|slash);
	
	while(sym == times || sym == slash)
	{
		mulop = sym;
		getsym();
		
		factor(fsys|times|slash);
	}
}


void condition(unsigned long fsys)
{
	unsigned long relop;
	
	if(sym == oddsym)
	{
		getsym();
		expression(fsys);
		
	}
	
	else
	{
		expression(fsys|lopbegsys);
		
		if(!(sym&lopbegsys))//注意！优先级
		{
			error(20);
			
			expression(fsys);
		}

	
		else
		{
			relop = sym;
			getsym();
			
			expression(fsys);			
		}
	}
}


void statement(unsigned long fsys)
{
	long i;
	
	if(sym == ident)
	{
		//i = position(id); 
		
		getsym();
		
		if(sym == becomes)
			getsym();
		else
			error(13);
		
		expression(fsys);
	}
	
	else if(sym == ifsym)
	{
		getsym();
		condition(fsys|thensym|elsesym);
		
		if(sym == thensym)
			getsym();
		else
			error(16);
			
		statement(fsys);
		
		if(sym == elsesym)
		{
			getsym();
			statement(fsys);
		}
		//如果少了else怎么办 
	}
	
	else if(sym == whilesym)
	{
		getsym();
		condition(fsys|dosym);
		
		if(sym == dosym)
			getsym();
		else
			error(18);
		
		statement(fsys);
	}
	
	else if(sym == callsym)
	{
		getsym();
		
		if(sym != ident)
			error(14);
		else
		{
			//i = position(id); error(15)
			getsym();
			if(sym == lparen)
			{
				getsym();
				
				if(sym&(facbegsys|plus|minus))
				{
					expression(fsys|rparen|plus|minus|comma);
					while(sym == comma)
					{
						getsym();
						expression(fsys|rparen|plus|minus|comma);//
					}
				}
				test((facbegsys|plus|minus)|rparen, fsys|comma|semicolon, 41); //处理 缺失右符号 错误括号内开头；
				while(sym == comma)
				{
					getsym();
					expression(fsys|rparen|plus|minus|comma);//
				}
				
				if(sym == rparen)
				{
					getsym();
				}
				
				else
					error(22);
			}

			
			//else 左括号 
		}
	}
	
	else if(sym == beginsym)
	{
		getsym();
		body(fsys);
	}
	
	else if(sym == readsym)
	{
		getsym();
		if(sym == lparen)
		{
			getsym();
			if(sym == ident)
			{
				//i = position(id); 
				
				getsym();
			}
			else
			{
				test(ident, fsys|comma|rparen|semicolon, 4); //防止奇怪的东西开头
				//error(4) 
			} 
			
			while(sym == comma)
			{
				getsym();
				if(sym == ident)
				{
					//i = position(id);
					getsym(); 
				}
				else
					error(36);
			}
			
			if(sym == rparen)
			{
				getsym();
			}
			else
				error(22);
		}
		//else 左括号 
		
	}
	
	else if(sym == writesym)
	{
		getsym();
		if(sym == lparen)
		{
			getsym();
				
			expression(fsys|rparen|plus|minus|comma);
			while(sym == comma)
			{
				getsym();
				expression(fsys|rparen|plus|minus|comma);
			}
			
			if(sym == rparen)
			{
				getsym();
			}
			
			else
				error(22);
		}
		//else 左括号 
	}
	
	else if(sym == endsym)
	{
		column -= 3;//报错应提前至end前
		error(35);
		column += 3;
		getsym();
	} 
	
	//else
} 


int main()
{
	for(long i = 0; i < 256; i++)
		ssym[i] = nul;
	ssym['+'] = plus;
	ssym['-'] = minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym[';'] = semicolon;
	
	declbegsys = constsym|varsym|procsym;
	statbegsys = beginsym|callsym|ifsym|whilesym|readsym|writesym;
	facbegsys = ident|number|lparen;
	lopbegsys = eql|neq|lss|leq|gtr|geq;
	
	cout<<"Please input source program file name:";
	cin>>infilename;
	cout<<'\n';
	
	
	if((infile=fopen(infilename, "r")) == NULL)
	{
		cout<<"File "<<infilename<<" can’t be opened. \n";
		exit(1); 
	}
	
	
	err = 0;
	cc = 0;
	cx = 0;
	ll = 0;
	ch = ' ';
	kk = al;
	row = 0;
	column = 0;
	rowlast = collast = rowtemp = coltemp = 0;
	
	
	program(declbegsys|statbegsys);
	
	if(!feof(infile))
	{
		cout<<"Program incomplete!\n";
		error(34);
		
		cout<<err<<" errors in PL/0 program!\n";
	}
	
	fclose(infile);

	return 0;
}

