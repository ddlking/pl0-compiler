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

/* ���Ե�ǰ�����Ƿ�Ϸ����� */
/* ������s1:���﷨����������˳�ĳһ�﷨��Ԫʱ��ǰ���ʷ���Ӧ���ڵļ��� */
/* s2:��ĳһ����״̬�£��ɻָ��﷨�������������Ĳ��䵥�ʼ��� */
/* n:������Ϣ��ţ�����ǰ���Ų����ںϷ��� s1 ����ʱ�����ĳ�����Ϣ */
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


/*��¼��ѯ���ű�*/
void enter(enum symtype k, char* sym_name); //��½���ű�
long position(enum symtype k, char* sym_name, bool is_same_pro = false); //��ѯ���ű�
void exitable(long dx0);


/*�м��������ͷ���*/
long findbase(long bp, long l); //��̬����ѯ����ַ
void gen(enum com x, long y, long z);
void interpret();
void listpcode(long pc0);

/* �ݹ��½��﷨�������� */
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

/* ��ӡ���� */
void printable();
void printcode();
void printblog();


void program(unsigned long long fsys) //�򵥱�����ν�����block��,program��δ����
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

	//long sp0; return�Ͱ����ָ���

	long lastpro = -1;//block����ǰ��һ������id�����ڲ������ݣ�-1��ʾû�����ô���

	if(dx > 1)
	{
		lastpro = display[dx];
		while (table[lastpro].previous != 0)
			lastpro = table[lastpro].previous;
		ddx = 3 + table[lastpro].size; //�βθ���Ӧ����blockǰ���proc
	}
	else
	{
		ddx = 3;//�������� ���������ʱ���㵥Ԫ����һ��������block�ı����������β�
	}


	if(lev > levmax)
		error(32);

	pc0 = pc;
	gen(jmp, 0, 0);//����
	
	if(sym == constsym)
	{
		getsym();
		constdeclaration();
	}
	
	while(sym == constsym)//������
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
	
	while(sym&(constsym|varsym))//������ 
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
	
	while(sym&(constsym|varsym))//������ 
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
			gen(sto, 0, 3 + table[lastpro].size - i);//����������£� bp = sp + 1
		}
		gen(Int, 0, table[lastpro].size);//�¼���
		table[lastpro].val = pc - table[lastpro].size - 1; //pcָ��δ�������һ�д��� ��������һ��
	}


	code[pc0].a = pc;//���� , �������岿�֣�ֻ�е�������Ӧֵ��ջ������Ӧ���ʽֵ��ʵ��ʱ����ת����
	gen(Int, 0, ddx);//�ָ�sp sp��bp����1��


	if(sym == beginsym)
	{
		getsym();
	
	}
	else
		error(33);
	
	body(fsys);

	gen(opr, 0, 0);//return

	ddx = ddx0;

	/* �������⣬��Ϊbody�ﲻ�ܷ����ӹ��̱���
	if (dx != dx0)
	{
		dx = dx0;
		tx = sx; 
	}
	*/
	//listpcode(pc0);
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


void vardeclaration()//�򵥻ָ���varʶ����block 
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


void proc(unsigned long long fsys)//procedureʶ����block 
{
	int cnt = 0;//��������
	long tx1;
	long dx0;//��¼��Ӧ��α�ջ��������ɾ����Ӧ���ű�
	long ddx0;//��Ϊ��block�У�ͳ��ddx������Ϊproc���̱䶯

	ddx0 = ddx;

	dx0 = dx;

	if(sym == ident) //�������������ظ������������ʲ��������򣩿��ܳ����⣡�˱�ʱTODO
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
		enter(pro, "unknown");//��ʱ����Ϊunknown
		error(4);
	}

	sx = tx;//��֤���̲�����������һ����ջ����

	tx1 = tx;
	lev++;
	
	test(lparen, fsys|ident|rparen|comma|semicolon, 40);	//���������ȱʧ ��Ƕ��if��Ϊ���ͬ���ж�if 
	if(sym == lparen)
		getsym();
		
	test(ident|rparen, fsys|comma|semicolon, 4); //�˴��漰 �����������ݵ���ͷ���ԣ� ������������������
		
	if (sym == ident) //������ͬ���������������򲻱����ҷ��ű���1��TODO
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
			// ?��δ���
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
	
	block(fsys|semicolon);//���ִ��gen(opr, 0, 0) �Ѵ���÷��ű�

	
	if (dx != dx0)
	{
		exitable(dx0);
		//dx = dx0;//ɾ���ӷ��ű�
		//tx = sx;//�ָ�ջ��
	}
	
	
	test(semicolon|beginsym, fsys|procsym|statbegsys|endsym, 10); //ֻ����һ�������ֵļ��裬��̫����
	
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
			proc(fsys);//��ʱͬ�㲻��lev++
		}
		else
			error(43);
	}

	ddx = ddx0;
	
	//����getsym()��begin��block���ж� 

}


void body(unsigned long long fsys)//begin�ж���block��statement�� 
{
	if(sym == endsym)//ȱ��statement���� 
	{
		error(39);
		getsym();
		
		return;
	}
	

	test(ident|statbegsys|endsym, fsys|semicolon, 42); //��Ҫ����ֺſ�ͷ
	if(sym == semicolon)
		getsym();
	
	statement(fsys|semicolon|endsym);
	
	
	if(sym == endsym)
	{
		getsym();
	}
	 

	else //Ҫô�У� Ҫô��end 
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
		/* �о����ã��Ѿ��������
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
		
		if(!(sym&lopbegsys))//ע�⣡���ȼ�
		{
			error(20);
			relop = eql; //���ȱʧĬ��Ϊ==
			
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
	
	else if(sym == ifsym) //������ת�����1
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
		gen(jpc, 0, 0);//���� Ϊ����ת
			
		statement(fsys|endsym);

		pc2 = pc; 
		gen(jmp, 0, 0);//����

		code[pc1].a = pc; //���Ϊ����ֱ������then��statement
		
		if(sym == elsesym)
		{
			getsym();
			statement(fsys|endsym);
		}
		code[pc2].a = pc; //�����else����û��
		//�˴�����elseȱʧ�ļ�飬���ȱʧelse�������ת��bodyȱʧ�ֺŵĻָ�������17
	}
	
	else if(sym == whilesym)
	{
		long pc1;
		long pc2;

		getsym();

		pc1 = pc;
		condition(fsys|dosym|endsym);
		pc2 = pc;
		gen(jpc, 0, 0); //����
		
		if(sym == dosym)
			getsym();
		else
			error(18);
		
		statement(fsys);

		gen(jmp, 0, pc1);
		code[pc2].a = pc;//����
		
	}
	
	else if(sym == callsym)//block�д��Σ�ȷ��ջ�����Ƕ�Ӧֵ�� TODO ��������Ǳ��ʽ
	{
		getsym();
		int cnt = 0;
		
		if (sym != ident)
		{
			test(ident, fsys, 14); //���ޱ�ʶ����ֱ����������Ϊ��֪����ô�ָ�����ȷ���� 
		}
		else
		{
			i = position(pro, id);

			getsym();
			
			test(lparen, fsys|(facbegsys|plus|minus)|rparen|comma|semicolon|endsym, 40); //���������ȱʧ���ṹͬ�� 
			if (sym == lparen)
			{
				getsym();
			}
				
			test((facbegsys|plus|minus)|rparen, fsys|comma|semicolon|endsym, 41); //���� ȱʧ�ҷ��� ���������ڿ�ͷ��
				
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
					if (cnt > table[i].size)//����ȡǰ
						pc = pc - (cnt - table[i].size);
					else if (cnt < table[i].size)//������0
					{
						long l = table[i].size - cnt;
						while (l--)
							gen(lit, 0, 0);
					}
				}
				gen(cal, lev - table[i].level, table[i].val); //����ȷ����λ���Ƿ���ȷ val����ת������ַ
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
		
		test(lparen, fsys|ident|comma|rparen|semicolon|endsym, 40); //���������ȱʧ���ṹͬ�� 
		if(sym == lparen)
			getsym();

		test(ident, fsys|comma|rparen|semicolon|endsym, 4); //��ֹ��ֵĶ�����ͷ
			
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
	
	else if(sym == writesym) //�򵥻ָ� �������û�и�ֵ����ô�죿
	{
		getsym();
		
		test(lparen, fsys|rparen|(facbegsys|plus|minus)|comma|endsym, 40); //���������ȱʧ���ṹͬ�� 
		if(sym == lparen)
			getsym();
		
		expression(fsys|rparen|plus|minus|comma|endsym);
		gen(wrt, 0, 0);//ֱ�����ջ��
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
		column -= 3;//����Ӧ��ǰ��endǰ
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
	long id = position(k, sym_name, true);//ͬ���������ظ�����
	if (id == -1)
	{
		tx++;
		id = tx;
	}//tx��1��ʼ
	else
	{
		is_exist = 1;
	}

	table[id].kind = k;
	strcpy(table[id].name, sym_name);

	switch (k)
	{
		case con:
			//if() getsym�����������
			table[id].val = num;
			table[id].level = lev;
			table[id].previous = lastx;
			if(lastx == id && is_exist)
				table[id].previous = llastx; //��һ����llast = 0 �ָ������� �Ҳ���llastx��ָ���Լ�
			display[dx] = id;
		break;

		case var:
			table[id].level = lev;
			table[id].addr = ddx;
			ddx++;//ddxָ�����е����
			table[id].previous = lastx;
			if (lastx == id && is_exist)
				table[id].previous = llastx;
			display[dx] = id;
		break;

		case pro://�������ͬ�����̼��
			table[id].level = lev;
			table[id].previous = 0;
			table[id].size = 0;
			dx++;
			display[dx] = id;
			//sx = id; �������棬Ҫ��֤�������ͬ�㶨��Ҫ�ָ����������
		break;
	}

	llastx = lastx;//���ڵ���ѭ��ʱ�ָ�
	lastx = id;
	
}


long position(enum symtype k, char* sym_name, bool is_same_pro)//���򷵻ض�Ӧλ�ã����򷵻�-1 ��֤ջ���ű������һ��
{
	bool flag = 1; // 1Ϊû�ҵ� 0�ҵ�
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
		while (dxtemp && flag) //dx��1��ʼ
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


void exitable(long dx0)//�����˳����ӷ��ű�Ҫɾ�������ӹ�����Ϊ����������һ����ű�����previous��Ϊ0,Ӧ��ֻ��һ��
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

	tx = sx;//�ָ�ջ��ҲҪ��
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
* ���¼���ã�
* ��ʱ��Ԫ
* ��������
* �򵥱���
* �βε�Ԫ
* ��̬��
* ���ص�ַ
* ��̬��
*/

/* ͨ����̬���������������ַ�ĺ��� base */
/* ����˵����l:Ҫ������������ڲ��뵱ǰ��Ĳ�� */
/* ����ֵ��Ҫ�����������ַ */
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

	pc = 0;//��ʼִ��ָ��
	bp = 0;
	sp = -1;//��ֵ��Ҫ2��ԭ���� ��һ��block��int�� 0�� dx�����˱��� TODO����Ҫʵ�ִ�3��ʼ��ջ
	datastk[0] = 0;
	datastk[1] = 0;
	datastk[2] = 0;//�������������� ����û��

	do
	{
		index = code[pc];
		//cout << codename[index.f] << " " << index.l << " " << index.a<<"\n";
		pc++;
		switch (index.f) //spָ�������ջ����������һ��
		{
			case lit: // ����������ջ��
				sp++;
				datastk[sp] = index.a;
				break;

			case opr:
				switch (index.a)
				{
					case 0: // return 
						sp = bp - 1;//ָ������
						pc = datastk[bp + 1];
						bp = datastk[bp];//��̬��
						break;

					case 1:  // ȡ��  
						datastk[sp] = - datastk[sp];
						break;

					case 2:  // �ӷ�  
						sp = sp - 1; 
						datastk[sp] = datastk[sp] + datastk[sp + 1];
						break;

					case 3:  // ��������ջ����ȥջ����
						sp = sp - 1; 
						datastk[sp] = datastk[sp] - datastk[sp + 1];
						break;

					case 4:  // �˷�  
						sp = sp - 1; 
						datastk[sp] = datastk[sp] * datastk[sp + 1];
						break;

					case 5:  // ��������ջ������ջ����
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

					case 13: //���"\n"
						cout << "\n";
						break;
				}
				break;

			case lod: //��������ֵ�ŵ�ջ��
				sp = sp + 1; 
				datastk[sp] = datastk[findbase(bp, index.l) + index.a];
				break;

			case sto: //��ջ����ֵ�����͵������� sp-- �ü�����sto��Ҫ�ָ�
				sp = sp - 1;
				datastk[findbase(bp, index.l) + index.a] = datastk[sp + 1]; 
				//datastk[findbase(bp, index.l) + index.a] = datastk[sp]; 
				//cout<<datastk[sp + 1]<<"\n";
				break;

			case cal:  //���ù��� sp���� ����sto�Ҷ�Ӧʵ�λ���ʽֵ
				datastk[sp + 1] = bp; //��̬��
				datastk[sp + 2] = pc;//���ص�ַ
				datastk[sp + 3] = findbase(bp, index.l); //��̬�� ��Ϊ�˴�l�ǵ��õĹ��̵Ĳ��
				bp = sp + 1;
				pc = index.a;
				break;

			case Int:  //�����ڴ�ռ�  (ʵ���ǽ�sp�ָ��������ڵ�ջ��)
				sp = sp + index.a;
				//printblog();
				break;

			case jmp:  //��������ת
				pc = index.a;
				break;

			case jpc:  //������ת��Ϊ����ת��������ջ�� sp--
				sp = sp - 1;
				if (datastk[sp + 1] == 0)
				{
					pc = index.a;
				}
				break;

			case red: //����
				cout << "�����������ֵ��"; //�����ʾ��������
				cin >> temp;
				datastk[findbase(bp, index.l) + index.a] = temp;
				break;

			case wrt: //���
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
		cout<<"File "<<infilename<<" can��t be opened. \n";
		exit(1); 
	}
	
	tx = 0;
	lev = 0;
	dx = 1;//dx��1��ʼ
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

	cout << "�Ƿ����Pcode:1 or 0" << endl;
	int flag;
	cin >> flag;
	if (flag)
	{
		printcode();
	}
	cout << "�Ƿ�������ű�:1 or 0" << endl;
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

