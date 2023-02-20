#include"pl0.h"
#include<bits/stdc++.h>

using namespace std;


void error(long n)
{
	cout<<"["<<row<<","<<column<<"] ";
	cout<<"Error("<<n<<")-> "<<err_msg[n]<<"\n";//eg:[2,9] Error(1)-> Found ':=' when expecting '='.
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
			//exit(1);
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
	}
	
	
	else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' 
	|| ch == '(' || ch == ')' || ch == ',' || ch == ';') 
	{
		a[l] = ch;
		sym = ssym[(unsigned char)ch];
		getch();
	}
	
	else
	{
		error(25);
		a[l] = ' ';
		getch();
		sym = nul;//错误字符 
	}
}
/* 词法分析过程 getsym 总结：从源文件中读出若干有效字符，组成一个 token 串，识别它的类型
为保留字/标识符/数字或是其它符号。如果是保留字，把 sym 置成相应的保留字类型，如果是
标识符，把 sym 置成 ident 表示是标识符，于此同时，id 变量中存放的即为保留字字符串或标识
符名字。如果是数字，把 sym 置为 number,同时 num 变量中存放该数字的值。如果是其它的操作符，
则直接把 sym 置成相应类型。经过本过程后 ch 变量中存放的是下一个即将被识别的字符 */


int main()
{
	ofstream outfile;
	
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
	statbegsys = beginsym|callsym|ifsym|whilesym;
	facbegsys = ident|number|lparen;
	
	cout<<"Please input source program file name:";
	cin>>infilename;
	cout<<'\n';
	
	if((infile=fopen(infilename, "r")) == NULL)
	{
		cout<<"File "<<infilename<<" can’t be opened. \n";
		exit(1); 
	}
	
	outfile.open("output.txt", ios::out);
	if(!outfile)
	{
		cout<<"File output.txt can’t be opened. \n";
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
	int temp;
	
	while(!feof(infile))
	{
		temp = err;
		getsym();
		
		if( /*temp != err ||*/ feof(infile));
		else
		{
			if(sym == number)
			{
				cout<<"<"<<num<<" ,";
				outfile<<"<"<<num<<" ,";
			}
			else
			{
				cout<<"<";
				if(sym != nul)
					outfile<<"<";
				for(long i = 0; i <= l; i++)
				{
					cout<<a[i];
					if(sym != nul)
						outfile<<a[i];
				}
				cout<<" ,";
				if(sym != nul)
					outfile<<" ,";
			}
			
			if(sym == number)
			{
				cout<<"number";
				outfile<<"number";
			}
			else if(sym == ident)
			{
				cout<<"ident";
				outfile<<"ident";
			}
			else
			{
				cout<<"-";
				if(sym != nul)
					outfile<<"-";
			}
			cout<<">\n";
			if(sym != nul)
				outfile<<">\n";		
		}

	}
	
	fclose(infile); 
	outfile.close();
	
	return 0;
}

