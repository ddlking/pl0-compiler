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
		/*
		if(feof(infile))
		{
			cout<<"Program incomplete!\n";
			
			if(err == 0)
				cout<<"There is no error in program!\n";
			else
				cout<<err<<" errors in PL/0 program!\n";
			exit(1);
		}
		*/
		
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
	if (feof(infile))
	{
		cout << "Program incomplete!\n";

		if (err == 0)
			cout << "There is no error in program!\n";
		else
			cout << err << " errors in PL/0 program!\n";
		return;
	}


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
long position(enum symtype k, char* sym_name, bool is_same_pro = false); //查询符号表
void exitable(long dx0);


/*中间代码产生和翻译*/
long findbase(long bp, long l); //静态链查询基地址
void gen(enum com x, long y, long z);
void interpret();
void listpcode(long pc0);

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

/* 打印函数 */
void printable();
void printcode();
void printblog();


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
	long pc0;

	long ddx0;

	ddx0 = ddx;

	//long sp0; return就包括恢复了

	long lastpro = -1;//block调用前的一个过程id，用于参数传递，-1表示没有则不用传参

	if(dx > 1)
	{
		lastpro = display[dx];
		while (table[lastpro].previous != 0)
			lastpro = table[lastpro].previous;
		ddx = 3 + table[lastpro].size; //形参个数应该是block前面的proc
	}
	else
	{
		ddx = 3;//连接数据 最底下是临时运算单元，第一个表是主block的表，往上走是形参
	}


	if(lev > levmax)
		error(32);

	pc0 = pc;
	gen(jmp, 0, 0);//暂填
	
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
		lev--;
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


	if (lastpro != -1)
	{
		for (long i = 1; i <= table[lastpro].size; i++)
		{
			gen(sto, 0, 3 + table[lastpro].size - i);//从最后即上往下； bp = sp + 1
		}
		gen(Int, 0, table[lastpro].size);//新加入
		table[lastpro].val = pc - table[lastpro].size - 1; //pc指向未填入的下一行代码 多了上面一句
	}


	code[pc0].a = pc;//回填 , 跳过定义部分，只有当调用相应值，栈顶放相应表达式值或实参时再跳转回来
	gen(Int, 0, ddx);//恢复sp sp在bp下面1个


	if(sym == beginsym)
	{
		getsym();
	
	}
	else
		error(33);
	
	body(fsys);

	gen(opr, 0, 0);//return

	ddx = ddx0;

	/* 不放在这，因为body里不能访问子过程变量
	if (dx != dx0)
	{
		dx = dx0;
		tx = sx; 
	}
	*/
	//listpcode(pc0);
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
	int cnt = 0;//变量个数
	long tx1;
	long dx0;//记录对应层次表栈顶，用于删除对应符号表
	long ddx0;//因为在block中，统计ddx不能因为proc过程变动

	ddx0 = ddx;

	dx0 = dx;

	if(sym == ident) //进程名不允许重复（不包括访问不到的区域）可能出问题！退表时TODO
	{
		long i;
		i = position(pro, id);
		if (i != -1)
		{
			error(46);
			enter(pro, "unknown");
		}
		else
		{
			enter(pro, id);
		}
		getsym();
	}
	
	else
	{
		enter(pro, "unknown");//暂时命名为unknown
		error(4);
	}

	sx = tx;//保证过程不重名，所以一定是栈顶！

	tx1 = tx;
	lev++;
	
	test(lparen, fsys|ident|rparen|comma|semicolon, 40);	//检查左括号缺失 由嵌套if变为多次同级判断if 
	if(sym == lparen)
		getsym();
		
	test(ident|rparen, fsys|comma|semicolon, 4); //此处涉及 括号内有内容但开头不对； 括号无内容且少括号
		
	if (sym == ident) //不允许同名！！！！！否则不报错且符号表少1个TODO
	{
		cnt++;
		ddx = cnt + 3 - 1;
		enter(var, id);
		table[tx1].size = cnt;
		getsym();
	}
	while(sym == comma)
	{
		getsym();
		if(sym == ident)
		{
			// ?如何传参
			cnt++;
			ddx = cnt + 3 - 1;
			enter(var, id);
			table[tx1].size = cnt;
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
	
	block(fsys|semicolon);//最后执行gen(opr, 0, 0) 已处理好符号表

	
	if (dx != dx0)
	{
		exitable(dx0);
		//dx = dx0;//删除子符号表
		//tx = sx;//恢复栈顶
	}
	
	
	test(semicolon|beginsym, fsys|procsym|statbegsys|endsym, 10); //只做少一或两部分的假设，别太荒谬
	
	while(sym&(semicolon|procsym))
	{
		if (sym == procsym)
			;
			//error(10);
		else
			getsym();
		
		if(sym == procsym)
		{
			getsym();
			proc(fsys);//此时同层不用lev++
		}
		else
			error(43);
	}

	ddx = ddx0;
	
	//不用getsym()，begin在block中判断 

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
		
		if (addop == minus)
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
		if (mulop == times)
		{
			gen(opr, 0, 4);
		}
		else if (mulop == slash)
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

		if (i != -1)
		{
			gen(sto, lev - table[i].level, table[i].addr);
		}
	}
	
	else if(sym == ifsym) //可能跳转会相差1
	{
		long pc1;
		long pc2;

		getsym();
		condition(fsys|thensym|elsesym|endsym);
		
		if(sym == thensym)
			getsym();
		else
			error(16);

		pc1 = pc;
		gen(jpc, 0, 0);//暂填 为假跳转
			
		statement(fsys|endsym);

		pc2 = pc; 
		gen(jmp, 0, 0);//暂填

		code[pc1].a = pc; //回填，为假则直接跳出then后statement
		
		if(sym == elsesym)
		{
			getsym();
			statement(fsys|endsym);
		}
		code[pc2].a = pc; //回填，无else等于没有
		//此处不做else缺失的检查，如果缺失else，会合理转向body缺失分号的恢复，报错17
	}
	
	else if(sym == whilesym)
	{
		long pc1;
		long pc2;

		getsym();

		pc1 = pc;
		condition(fsys|dosym|endsym);
		pc2 = pc;
		gen(jpc, 0, 0); //暂填
		
		if(sym == dosym)
			getsym();
		else
			error(18);
		
		statement(fsys);

		gen(jmp, 0, pc1);
		code[pc2].a = pc;//回填
		
	}
	
	else if(sym == callsym)//block中传参（确保栈顶都是对应值） TODO 如果里面是表达式
	{
		getsym();
		int cnt = 0;
		
		if (sym != ident)
		{
			test(ident, fsys, 14); //若无标识符，直接跳过，因为不知道怎么恢复到正确过程 
		}
		else
		{
			i = position(pro, id);

			getsym();
			
			test(lparen, fsys|(facbegsys|plus|minus)|rparen|comma|semicolon|endsym, 40); //检查括号左缺失，结构同上 
			if (sym == lparen)
			{
				getsym();
			}
				
			test((facbegsys|plus|minus)|rparen, fsys|comma|semicolon|endsym, 41); //处理 缺失右符号 错误括号内开头；
				
			if(sym&(facbegsys|plus|minus))
			{
				expression(fsys|rparen|plus|minus|comma|endsym);
				cnt++;
			}
			while(sym == comma)
			{
				getsym();
				expression(fsys|rparen|plus|minus|comma|endsym);
				cnt++;
			}
			
			if(sym == rparen)
			{
				getsym();
			}
				
			else
				error(22);

			if (i == -1)
				error(11);
			else
			{
				if (cnt != table[i].size)
				{
					error(48);
					if (cnt > table[i].size)//多则取前
						pc = pc - (cnt - table[i].size);
					else if (cnt < table[i].size)//少则填0
					{
						long l = table[i].size - cnt;
						while (l--)
							gen(lit, 0, 0);
					}
				}
				gen(cal, lev - table[i].level, table[i].val); //还不确定该位置是否正确 val表明转向代码地址
			}

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
			i = position(var, id);
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
				i = position(var, id);
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
	
	else if(sym == writesym) //简单恢复 如果读到没有赋值的怎么办？
	{
		getsym();
		
		test(lparen, fsys|rparen|(facbegsys|plus|minus)|comma|endsym, 40); //检查左括号缺失，结构同上 
		if(sym == lparen)
			getsym();
		
		expression(fsys|rparen|plus|minus|comma|endsym);
		gen(wrt, 0, 0);//直接输出栈顶
		while(sym == comma)
		{
			getsym();
			expression(fsys|rparen|plus|minus|comma|endsym);
			gen(wrt, 0, 0);
		}
		gen(opr, 0, 13);
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
	bool is_exist = 0;

	if (tx >= txmax)
	{
		error(47);
		return;
	}
	long id = position(k, sym_name, true);//同区域不允许重复定义
	if (id == -1)
	{
		tx++;
		id = tx;
	}//tx从1开始
	else
	{
		is_exist = 1;
	}

	table[id].kind = k;
	strcpy(table[id].name, sym_name);

	switch (k)
	{
		case con:
			//if() getsym已做超出检查
			table[id].val = num;
			table[id].level = lev;
			table[id].previous = lastx;
			if(lastx == id && is_exist)
				table[id].previous = llastx; //第一个则llast = 0 恢复后正常 且不会llastx还指向自己
			display[dx] = id;
		break;

		case var:
			table[id].level = lev;
			table[id].addr = ddx;
			ddx++;//ddx指向已有的最高
			table[id].previous = lastx;
			if (lastx == id && is_exist)
				table[id].previous = llastx;
			display[dx] = id;
		break;

		case pro://外层已做同名进程检查
			table[id].level = lev;
			table[id].previous = 0;
			table[id].size = 0;
			dx++;
			display[dx] = id;
			//sx = id; 放在外面，要保证多个过程同层定义要恢复到当层最初
		break;
	}

	llastx = lastx;//用于当死循环时恢复
	lastx = id;
	
}


long position(enum symtype k, char* sym_name, bool is_same_pro)//有则返回对应位置，无则返回-1 保证栈符号表仅出现一次
{
	bool flag = 1; // 1为没找到 0找到
	long id = display[dx];
	long dxtemp = dx;
	if (is_same_pro)
	{
		if (k == cav)
		{
			while ((strcmp(sym_name, table[id].name) != 0 || table[id].kind == pro) && id != 0)
			{
				id = table[id].previous;
			}
		}
		else
		{
			while ((strcmp(sym_name, table[id].name) != 0 || k != table[id].kind) && id != 0)
			{
				id = table[id].previous;
			}
		}
		if (id != 0)
			flag = 0;
	}
	else
	{
		while (dxtemp && flag) //dx从1开始
		{
			id = display[dxtemp];
			if (k == cav)
			{
				while ((strcmp(sym_name, table[id].name) != 0 || table[id].kind == pro) && id != 0)
				{
					id = table[id].previous;
				}
			}
			else
			{
				while ((strcmp(sym_name, table[id].name) != 0 || k != table[id].kind) && id != 0)
				{
					id = table[id].previous;
				} 
			}
			if (id != 0)
			{
				flag = 0;
				break;
			}
			dxtemp--;
		}
	}
	if (flag)
	{
		return -1;
	}
	return id;
}


void exitable(long dx0)//过程退出后子符号表要删除，但子过程做为变量存入上一层符号表，其中previous不为0,应该只差一个
{
	if (dx0 > dx)
		return;
	long idtemp;
	while (dx > dx0)
	{
		idtemp = display[dx];
		while (table[idtemp].previous)
			idtemp = table[idtemp].previous;
		dx--;
		table[idtemp].previous = display[dx0];
		display[dx0] = idtemp;

	}

	tx = sx;//恢复栈顶也要改
}


void gen(enum com x, long y, long z)
{
	if (pc > pcmax)
	{
		cout << "program is too long.\n";
		exit(1);
	}
	code[pc].f = x;
	code[pc].l = y;
	code[pc].a = z;

	pc++;
}


void listpcode(long pc0)
{
	for (long i = pc0; i <= pc; i++)
	{
		cout << codename[code[i].f] << " " << code[i].l << " " << code[i].a<<"\n";
	}
}


void printcode()
{
	cout << "start print code!" << "\n";
	for (long i = 0; i < pc; i++)
	{
		cout << i <<" "<<codename[code[i].f] << " " << code[i].l << " " << code[i].a << "\n";
	}
	cout << "print code end!" << "\n";
}


void printable()
{
	cout << "start print table!" << "\n";
	for (long i = 1; i <= tx; i++)
	{
		cout << table[i].kind << " " << table[i].name << " " << table[i].val << " " << table[i].level << " ";
		cout << table[i].addr << " " << table[i].size << " " << table[i].previous;
		cout << "\n";
	}
	cout << "print table end!" << "\n";
}

void printblog()
{
	cout << "start print blog!" << "\n";
	for (long i = bp; i <= sp; i++)
	{
		cout << datastk[i] << "\n";
	}
	cout << "print blog end!" << "\n";
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
	long temp;
	cout << "PL/0 start!\n";

	pc = 0;//开始执行指令
	bp = 0;
	sp = -1;//初值不要2的原因是 第一次block中int， 0， dx包括了变量 TODO但你要实现从3开始用栈
	datastk[0] = 0;
	datastk[1] = 0;
	datastk[2] = 0;//主过程连接数据 等于没有

	do
	{
		index = code[pc];
		//cout << codename[index.f] << " " << index.l << " " << index.a<<"\n";
		pc++;
		switch (index.f) //sp指向已填的栈顶，而非下一个
		{
			case lit: // 将常量放入栈顶
				sp++;
				datastk[sp] = index.a;
				break;

			case opr:
				switch (index.a)
				{
					case 0: // return 
						sp = bp - 1;//指向已有
						pc = datastk[bp + 1];
						bp = datastk[bp];//动态链
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

					case 13: //输出"\n"
						cout << "\n";
						break;
				}
				break;

			case lod: //将变量的值放到栈顶
				sp = sp + 1; 
				datastk[sp] = datastk[findbase(bp, index.l) + index.a];
				break;

			case sto: //将栈顶的值弹出送到变量中 sp-- 得减，但sto后要恢复
				sp = sp - 1;
				datastk[findbase(bp, index.l) + index.a] = datastk[sp + 1]; 
				//datastk[findbase(bp, index.l) + index.a] = datastk[sp]; 
				//cout<<datastk[sp + 1]<<"\n";
				break;

			case cal:  //调用过程 sp不变 用于sto找对应实参或表达式值
				datastk[sp + 1] = bp; //动态链
				datastk[sp + 2] = pc;//返回地址
				datastk[sp + 3] = findbase(bp, index.l); //静态链 因为此处l是调用的过程的层差
				bp = sp + 1;
				pc = index.a;
				break;

			case Int:  //开配内存空间  (实际是将sp恢复至本该在的栈顶)
				sp = sp + index.a;
				//printblog();
				break;

			case jmp:  //无条件跳转
				pc = index.a;
				break;

			case jpc:  //条件跳转（为假跳转）并弹出栈顶 sp--
				sp = sp - 1;
				if (datastk[sp + 1] == 0)
				{
					pc = index.a;
				}
				break;

			case red: //输入
				cout << "请输入变量的值："; //如何显示变量名？
				cin >> temp;
				datastk[findbase(bp, index.l) + index.a] = temp;
				break;

			case wrt: //输出
				cout << datastk[sp] << " ";
				break;
		}
	}
	while (pc != 0); // ?

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
	
	tx = 0;
	lev = 0;
	dx = 1;//dx从1开始
	display[dx] = 1;
	err = 0;
	cc = 0;
	pc = 0;
	ll = 0;
	ch = ' ';
	row = 0;
	column = 0;
	rowlast = collast = rowtemp = coltemp = 0;
	
	
	program(declbegsys|statbegsys);

	cout << "是否输出Pcode:1 or 0" << endl;
	int flag;
	cin >> flag;
	if (flag)
	{
		printcode();
	}
	cout << "是否输出符号表:1 or 0" << endl;
	cin >> flag;
	if (flag)
	{
		printable();
		cout << endl;
	}

	if (err == 0)
	{
		interpret();
	}

	
	if(!feof(infile))
	{
		cout<<"Program incomplete!\n";
		error(34);
		
		cout<<err<<" errors in PL/0 program!\n";
	}
	
	fclose(infile);

	return 0;
}

