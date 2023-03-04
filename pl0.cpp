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


/* ��ȡ��һ���ַ����� getch( );*/ 
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
		
		row++;//ָʾ����
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
			ll--;//��ΪEOF�����ȥ�����һ�� 
		
		ll++;
		line[ll] = ' ';//ÿһ�н�β�ո�
	}
	//����ÿ�δ��л�������ȡһ���ַ�
	
	column++;
	cc++;
	ch = line[cc];
}


/* �ʷ��������� */
void getsym()
{
	collast = coltemp;
	rowlast = rowtemp;
	
	while(ch == ' ' || ch == '\t')
		getch();
	
	l = 0;
	if(isalpha(ch)) //tokenΪ��ĸ,���жϱ�ʶ��������
	{
		l = 0; //lΪ����
		memset(a, 0, sizeof a);
		while(isalpha(ch) || isdigit(ch)) //��������ĸ������
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
		//β���ַ����� ?
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
		strcpy(id, a);//id��ŵ�ǰ����
	
		long i, j, k; 
		i = 0;
		j = norw - 1;//norwΪ�ؼ����������� 
	
		//��ؼ��ֱ� 
		while(i <= j)
		{
			k = (i+j)/2;
			
			if(strcmp(id, word[k]) <= 0)
				j = k - 1;
			
			if(strcmp(id, word[k]) >= 0)
				i = k + 1;
				
		}
		
		if(i - 1 > j)
			sym = wsym[k];//�ؼ���
		else
			sym = ident;//��ʶ��
			
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
		
		if(isalpha(ch))//���ֿ�ͷ�ı�ʶ����ʡ������ 
		{
			error(28);
			getsym();
		} 
		
		if(ch == '.')//PL/0��֧������ 
		{
			error(27);
			getch();
			while(isdigit(ch))
				getch(); //ȥ��С������ 
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
			error(29);//�ʷ�������ֵ��������� 
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
		//sym = nul;//�����ַ�  �����ַ�����¼λ�� 
	}
	 
}
/* �ʷ��������� getsym �ܽ᣺��Դ�ļ��ж���������Ч�ַ������һ�� token ����ʶ����������
Ϊ������/��ʶ��/���ֻ����������š�����Ǳ����֣��� sym �ó���Ӧ�ı��������ͣ������
��ʶ������ sym �ó� ident ��ʾ�Ǳ�ʶ�����ڴ�ͬʱ��id �����д�ŵļ�Ϊ�������ַ������ʶ
�����֡���������֣��� sym ��Ϊ number,ͬʱ num �����д�Ÿ����ֵ�ֵ������������Ĳ�������
��ֱ�Ӱ� sym �ó���Ӧ���͡����������̺� ch �����д�ŵ�����һ��������ʶ����ַ� */

/* ���Ե�ǰ�����Ƿ�Ϸ����� */
/* ������s1:���﷨����������˳�ĳһ�﷨��Ԫʱ��ǰ���ʷ���Ӧ���ڵļ��� */
/* s2:��ĳһ����״̬�£��ɻָ��﷨�������������Ĳ��䵥�ʼ��� */
/* n:������Ϣ��ţ�����ǰ���Ų����ںϷ��� s1 ����ʱ�����ĳ�����Ϣ */
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


//void enter(enum object k) //��½���ű�


//long position(char* id) //��ѯ���ű�


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



void program(unsigned long fsys) //�򵥱�����ν�����block�� 
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
	//û���Ƕ���ʹ������� �ѵ�����
	if(sym == beginsym)
	{
		getsym();
	
	}
	else
		error(33);
	
	body(fsys);
}
 

void constdeclaration()//�򵥻ָ���constʶ����block 
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


void vardeclaration()//�򵥻ָ���varʶ����block 
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


void proc(unsigned long fsys)//procedureʶ����block 
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
		test(ident|rparen, fsys|comma|semicolon, 4); //�˴��漰 �����������ݵ���ͷ���ԣ� ������������������
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
		
	//else ������ 
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
	//���˷ֺ���ô�� 
	
	lev++;
}

void body(unsigned long fsys)//begin�ж���block��statement�� 
{
	if(sym == endsym)//ȱ��statement���� 
	{
		error(39);
		getsym();
		
		return;
	}
	
	//test(statbegsys|endsym, fsys|semicolon, 42); //��Ҫ����ֺſ�ͷ
	
	
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
		
		if(!(sym&lopbegsys))//ע�⣡���ȼ�
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
		//�������else��ô�� 
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
				test((facbegsys|plus|minus)|rparen, fsys|comma|semicolon, 41); //���� ȱʧ�ҷ��� ���������ڿ�ͷ��
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

			
			//else ������ 
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
				test(ident, fsys|comma|rparen|semicolon, 4); //��ֹ��ֵĶ�����ͷ
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
		//else ������ 
		
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
		//else ������ 
	}
	
	else if(sym == endsym)
	{
		column -= 3;//����Ӧ��ǰ��endǰ
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
		cout<<"File "<<infilename<<" can��t be opened. \n";
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

