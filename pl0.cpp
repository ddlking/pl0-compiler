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
		
		if (l > nmax)
		{
			error(31);
			num = 0;
		}
			
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
void test(unsigned long long s1, unsigned long long s2, long n)
{
	if(!(sym & s1))
	{
		error(n);
		s1 = s1 | s2;
	
		while(!(sym & s1))
			getsym();
	}
}


/*登录查询符号表*/
void enter(enum symtype k, char* sym_name); //登陆符号表
long position(enum symtype k, char* sym_name); //查询符号表


/*中间代码产生和翻译*/
long findbase(long bp, long l); //静态链查询基地址
void gen(enum com x, long y, long z);
void interpret();


/* 递归下降语法分析过程 */
void program(unsigned long long fsys);
void block(unsigned long long fsys);
void constdeclaration();
void vardeclaration();
void proc(unsigned long long fsys);
void body(unsigned long long fsys);
void statement(unsigned long long fsys);
void condition(unsigned long long fsys);
void expression(unsigned long long fsys);
void term(unsigned long long fsys);
void factor(unsigned long long fsys);


void program(unsigned long long fsys) //简单报错，入参仅传给block用,program名未处理
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


void block(unsigned long long fsys)
{
	if(lev > levmax)
		error(32);
	
	if(sym == constsym)
	{
		getsym();
		constdeclaration();
	}
	
	while(sym == constsym)//出错处理 
	{
		error(45);
		getsym();
		constdeclaration();
	}
	
	if(sym == varsym)
	{
		getsym();
		vardeclaration();
	}
	
	while(sym&(constsym|varsym))//出错处理 
	{
		if(sym == constsym)
		{
			error(44);
			getsym();
			constdeclaration();
		}
		else if(sym == varsym)
		{
			error(45);
			getsym();
			vardeclaration();			
		}
	}
	
	if(sym == procsym)
	{
		getsym();
		proc(fsys);
	}
	
	while(sym&(constsym|varsym))//出错处理 
	{
		if(sym == constsym)
		{
			error(45);
			getsym();
			constdeclaration();
		}
		else if(sym == varsym)
		{
			error(45);
			getsym();
			vardeclaration();			
		}
	}

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
				enter(con, id);
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
					enter(con, id);
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
		enter(var, id);
		getsym();
		
	}
	else
		error(4);
	
	while(sym == comma)
	{
		getsym();
		
		if(sym == ident)
		{
			enter(var, id);
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


void proc(unsigned long long fsys)//procedure识别在block 
{
	if(sym == ident) //考虑多重定义问题？
	{
		enter(pro, id);
		getsym();
	}
	
	else
	{
		enter(pro, "unknown");//暂时命名为unknown
		error(4);
	}
	
	test(lparen, fsys|ident|rparen|comma|semicolon, 40);	//检查左括号缺失 由嵌套if变为多次同级判断if 
	if(sym == lparen)
		getsym();
		
	test(ident|rparen, fsys|comma|semicolon, 4); //此处涉及 括号内有内容但开头不对； 括号无内容且少括号
		
	if (sym == ident)
	{
		enter(var, id);
		getsym();
	}
	while(sym == comma)
	{
		getsym();
		if(sym == ident)
		{
			// ?如何传参
			enter(var, id);
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
		
	
	if(sym == semicolon)
		getsym();
	else
		error(10);
	
	block(fsys|semicolon);
	
	test(semicolon|beginsym, fsys|procsym|statbegsys|endsym, 10); //只做少一或两部分的假设，别太荒谬
	
	while(sym == semicolon)
	{
		getsym();
		
		if(sym == procsym)
		{
			getsym();
			proc(fsys);//lev++?
		}
		else
			error(43);
	}
	
	//不用getsym()，begin在block中判断 
	lev++;
}


void body(unsigned long long fsys)//begin判断在block和statement中 
{
	if(sym == endsym)//缺少statement部分 
	{
		error(39);
		getsym();
		
		return;
	}
	

	test(ident|statbegsys|endsym, fsys|semicolon, 42); //主要处理分号开头
	if(sym == semicolon)
		getsym();
	
	statement(fsys|semicolon|endsym);
	
	
	if(sym == endsym)
	{
		getsym();
	}
	 

	else //要么有； 要么有end 
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


void expression(unsigned long long fsys)
{
	unsigned long addop;
	
	if(sym == plus || sym == minus)
	{
		addop = sym;
		getsym();
		
		term(fsys|plus|minus);
		
		if (sym == minus)
		{
			gen(opr, 0, 1);
		}
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

		if (addop == plus)
		{
			gen(opr, 0, 2);
		}

		else if (addop == minus)
		{
			gen(opr, 0, 3);
		}
	}
}


void factor(unsigned long long fsys)
{
	long i;
	
	test(facbegsys, fsys, 24);
	
	//while?
	if(sym == ident)
	{
		i = position(cav, id); //??
		
		if (i == -1)
			error(11);
		else
		{
			if (table[i].kind == con)
			{
				gen(lit, 0, table[i].val);
			}

			else if (table[i].kind == var)
			{
				gen(lod, lev - table[i].level, table[i].addr);
			}
		}

		getsym();
	}
	
	else if(sym == number)
	{
		/* 感觉不用，已经处理过了
		if(num > amax)
		{
			error(31);
			num = 0;
		}
		*/
		gen(lit, 0, num);

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


void term(unsigned long long fsys)
{
	unsigned long mulop;
	
	factor(fsys|times|slash);
	
	while(sym == times || sym == slash)
	{
		mulop = sym;
		getsym();
		
		factor(fsys|times|slash);
		if (sym == times)
		{
			gen(opr, 0, 4);
		}
		else if (sym == slash)
		{
			gen(opr, 0, 5);
		}
	}
}


void condition(unsigned long long fsys)
{
	unsigned long relop;
	
	if(sym == oddsym)
	{
		getsym();
		expression(fsys);
		
		gen(opr, 0, 6);
	}
	
	else
	{
		expression(fsys|lopbegsys);
		
		if(!(sym&lopbegsys))//注意！优先级
		{
			error(20);
			relop = eql; //如果缺失默认为==
			
			expression(fsys);
		}

	
		else
		{
			relop = sym;
			getsym();
			
			expression(fsys);			
		}
		switch (relop)
		{
			case eql:
				gen(opr, 0, 7);
				break;
			
			case neq:
				gen(opr, 0, 8);
				break;

			case lss:
				gen(opr, 0, 9);
				break;

			case leq:
				gen(opr, 0, 10);
				break;

			case gtr:
				gen(opr, 0, 11);
				break;

			case geq:
				gen(opr, 0, 12);
				break;
		}
			
	}
}


void statement(unsigned long long fsys)
{
	long i;
	
	if(sym == ident)
	{
		i = position(var, id);
		if (i == -1)
			error(11);
		
		getsym();
		
		if(sym == becomes)
			getsym();
		else
			error(13);
		
		expression(fsys|endsym);
	}
	
	else if(sym == ifsym)
	{
		getsym();
		condition(fsys|thensym|elsesym|endsym);
		
		if(sym == thensym)
			getsym();
		else
			error(16);
			
		statement(fsys|endsym);
		
		if(sym == elsesym)
		{
			getsym();
			statement(fsys|endsym);
		}
		//如果少了else怎么办 
	}
	
	else if(sym == whilesym)
	{
		getsym();
		condition(fsys|dosym|endsym);
		
		if(sym == dosym)
			getsym();
		else
			error(18);
		
		statement(fsys);
	}
	
	else if(sym == callsym)//如何传参？
	{
		getsym();
		
		if(sym != ident)
			error(14);
		else
		{
			i = position(pro, id);
			if (i == -1)
				error(11);
			else
			{
				gen(cal, lev - table[i].level, table[i].addr);
			}
			getsym();
			
			test(lparen, fsys|(facbegsys|plus|minus)|rparen|comma|semicolon|endsym, 40); //检查括号左缺失，结构同上 
			if(sym == lparen)
				getsym();
				
			test((facbegsys|plus|minus)|rparen, fsys|comma|semicolon|endsym, 41); //处理 缺失右符号 错误括号内开头；
				
			if(sym&(facbegsys|plus|minus))
				expression(fsys|rparen|plus|minus|comma|endsym);
			while(sym == comma)
			{
				getsym();
				expression(fsys|rparen|plus|minus|comma|endsym);//
			}
				
			if(sym == rparen)
			{
				getsym();
			}
				
			else
				error(22);
			

			

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
		
		test(lparen, fsys|ident|comma|rparen|semicolon|endsym, 40); //检查左括号缺失，结构同上 
		if(sym == lparen)
			getsym();

		test(ident, fsys|comma|rparen|semicolon|endsym, 4); //防止奇怪的东西开头
			
		if (sym == ident)
		{
			i = position(con, id);
			if (i == -1)
				error(11);
			else
			{
				gen(red, lev - table[i].level, table[i].addr);
			}
			getsym();
		}
		
		while(sym == comma)
		{
			getsym();
			if(sym == ident)
			{
				i = position(con, id);
				if (i == -1)
					error(11);
				else
				{
					gen(red, lev - table[i].level, table[i].addr);
				}
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
	
	else if(sym == writesym) //简单恢复 
	{
		getsym();
		
		test(lparen, fsys|rparen|(facbegsys|plus|minus)|comma|endsym, 40); //检查左括号缺失，结构同上 
		if(sym == lparen)
			getsym();
		
		expression(fsys|rparen|plus|minus|comma|endsym);
		while(sym == comma)
		{
			getsym();
			expression(fsys|rparen|plus|minus|comma|endsym);
		}
			
		if(sym == rparen)
		{
			getsym();
		}
		else
			error(22);

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



void enter(enum symtype k, char* sym_name)
{
	long id = position(k, sym_name);
	if (id == -1)
	{
		tx++;
		id = tx;
	}

	table[id].kind = k;
	strcpy(table[id].name, sym_name);
	switch (k)
	{
		case con:
			//if() getsym已做超出检查
			table[id].val = num;
			table[id].level = lev;
		break;

		case var:
			table[id].level = lev;
			table[id].addr = sp;
			sp++;
		break;

		case pro:
			table[id].level = lev;
			table[id].addr = sp;
		break;
	}
}


long position(enum symtype k, char* sym_name)//有则返回对应位置，无则返回-1 保证栈符号表仅出现一次
{
	bool flag = 1; // 1为没找到 0找到
	long id = display[dx];
	while (dx && flag) //dx从1开始
	{
		id = display[dx];
		if (k == cav)
		{
			while (strcmp(sym_name, table[id].name) != 0 || table[id].kind == pro)
			{
				id = table[id].previous;
			}
		}
		else
		{
			while (strcmp(sym_name, table[id].name) != 0 || k != table[id].kind)
			{
				id = table[id].previous;
			}
		}
		dx--;
	}
	if (flag)
	{
		return -1;
	}
	return id;
}


void gen(enum com x, long y, long z)
{
	if (cx > cxmax)
	{
		cout << "program is too long.\n";
		exit(1);
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;

	cx++;
}


/*
* 活动记录设置：
* 临时单元
* 内情向量
* 简单变量
* 形参单元
* 静态链
* 返回地址
* 动态链
*/

/* 通过静态链求出数据区基地址的函数 base */
/* 参数说明：l:要求的数据区所在层与当前层的层差 */
/* 返回值：要求的数据区基址 */
long findbase(long bp, long l)
{
	long bp1;
	bp1 = bp;
	while (l > 0)
	{
		bp1 = datastk[bp1+2];
		l--;
	}
	return bp1;
}


void interpret()
{
	//TODO
	cout << "PL/0 start!\n";
	pc = 0; bp = 1; sp = 0;
	do
	{
		index = code[pc];
		pc++;
		switch (index.f)
		{
			case lit: // 将常量放入栈顶
				sp++;
				datastk[sp] = index.a;
				break;

			case opr:
				switch (index.a)
				{
					case 0: // return 
						pc = bp - 1; 
						pc = datastk[pc + 3];
						bp = datastk[pc + 2];
						break;

					case 1:  // 取反  
						datastk[sp] = - datastk[sp];
						break;

					case 2:  // 加法  
						sp = sp - 1; 
						datastk[sp] = datastk[sp] + datastk[sp + 1];
						break;

					case 3:  // 减法（次栈顶减去栈顶）
						sp = sp - 1; 
						datastk[sp] = datastk[sp] - datastk[sp + 1];
						break;

					case 4:  // 乘法  
						sp = sp - 1; 
						datastk[sp] = datastk[sp] * datastk[sp + 1];
						break;

					case 5:  // 除法（次栈顶除以栈顶）
						sp = sp - 1; 
						datastk[sp] = datastk[sp] / datastk[sp + 1];
						break;

					case 6:  // odd  
						datastk[sp] = datastk[sp] % 2;
						break;

					/*
					case 7: // mod
						sp = sp - 1;
						datastk[sp] = (datastk[sp] % datastk[sp + 1]);
						break;
					*/

					case 7:  // == 
						sp = sp - 1; 
						datastk[sp] = (datastk[sp] == datastk[sp + 1]);
						break;

					case 8:  // != 
						sp = sp - 1; 
						datastk[sp] = (datastk[sp] != datastk[sp + 1]);
						break;

					case 9: // <  
						sp = sp - 1; 
						datastk[sp] = (datastk[sp] < datastk[sp + 1]);
						break;

					case 10: // <=  
						sp = sp - 1; 
						datastk[sp] = (datastk[sp] <= datastk[sp + 1]);
						break;

					case 11: // > 
						sp = sp - 1; 
						datastk[sp] = (datastk[sp] > datastk[sp + 1]);
						break;

					case 12: // >= 
						sp = sp - 1; 
						datastk[sp] = (datastk[sp] >= datastk[sp + 1]);
						break;

					case 13: //输出
						sp = sp - 1;
						cout << datastk[sp + 1];
						break;

					case 14: // \n
						cout << "\n";
						break;

					case 15: //输入
						sp = sp + 1;
						cin >> datastk[sp - 1];
						break;
						
				}
				break;

			case lod: //将变量的值放到栈顶
				sp = sp + 1; 
				datastk[sp] = datastk[findbase(bp, index.l) + index.a];
				break;

			case sto: //将栈顶的值弹出送到变量中 
				sp = sp - 1;
				datastk[findbase(bp, index.l) + index.a] = datastk[sp + 1]; 
				printf("%10d\n", datastk[sp + 1]);
				break;

			case cal:  //调用过程 ?
				datastk[sp + 1] = findbase(bp, index.l); 
				datastk[sp + 2] = bp;
				datastk[sp + 3] = pc;
				bp = sp + 1;
				pc = index.a;
				break;

			case Int:  //开配内存空间  
				sp = sp + index.a;
				break;

			case jmp:  //无条件跳转
				pc = index.a;
				break;

			case jpc:  //条件跳转（为假跳转）并弹出栈顶
				sp = sp - 1;
				if (datastk[sp + 1] == 0)
				{
					pc = index.a;
				}
				break;

			case red:
				break;

			case wrt:
				break;
		}
	}
	while (pc <= cx); // ?

	cout << "PL/0 complete!\n";
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

