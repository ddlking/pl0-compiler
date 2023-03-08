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
/* �ʷ��������� getsym �ܽ᣺��Դ�ļ��ж���������Ч�ַ������һ�� token ����ʶ����������
Ϊ������/��ʶ��/���ֻ����������š�����Ǳ����֣��� sym �ó���Ӧ�ı��������ͣ������
��ʶ������ sym �ó� ident ��ʾ�Ǳ�ʶ�����ڴ�ͬʱ��id �����д�ŵļ�Ϊ�������ַ������ʶ
�����֡���������֣��� sym ��Ϊ number,ͬʱ num �����д�Ÿ����ֵ�ֵ������������Ĳ�������
��ֱ�Ӱ� sym �ó���Ӧ���͡����������̺� ch �����д�ŵ�����һ��������ʶ����ַ� */

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
long position(enum symtype k, char* sym_name); //��ѯ���ű�


/*�м��������ͷ���*/
long findbase(long bp, long l); //��̬����ѯ����ַ
void gen(enum com x, long y, long z);
void interpret();


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
	if(lev > levmax)
		error(32);
	
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
	if(sym == ident) //���Ƕ��ض������⣿
	{
		enter(pro, id);
		getsym();
	}
	
	else
	{
		enter(pro, "unknown");//��ʱ����Ϊunknown
		error(4);
	}
	
	test(lparen, fsys|ident|rparen|comma|semicolon, 40);	//���������ȱʧ ��Ƕ��if��Ϊ���ͬ���ж�if 
	if(sym == lparen)
		getsym();
		
	test(ident|rparen, fsys|comma|semicolon, 4); //�˴��漰 �����������ݵ���ͷ���ԣ� ������������������
		
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
			// ?��δ���
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
	
	test(semicolon|beginsym, fsys|procsym|statbegsys|endsym, 10); //ֻ����һ�������ֵļ��裬��̫����
	
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
	
	//����getsym()��begin��block���ж� 
	lev++;
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
		//�������else��ô�� 
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
	
	else if(sym == callsym)//��δ��Σ�
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
			
			test(lparen, fsys|(facbegsys|plus|minus)|rparen|comma|semicolon|endsym, 40); //���������ȱʧ���ṹͬ�� 
			if(sym == lparen)
				getsym();
				
			test((facbegsys|plus|minus)|rparen, fsys|comma|semicolon|endsym, 41); //���� ȱʧ�ҷ��� ���������ڿ�ͷ��
				
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
		
		test(lparen, fsys|ident|comma|rparen|semicolon|endsym, 40); //���������ȱʧ���ṹͬ�� 
		if(sym == lparen)
			getsym();

		test(ident, fsys|comma|rparen|semicolon|endsym, 4); //��ֹ��ֵĶ�����ͷ
			
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
	
	else if(sym == writesym) //�򵥻ָ� 
	{
		getsym();
		
		test(lparen, fsys|rparen|(facbegsys|plus|minus)|comma|endsym, 40); //���������ȱʧ���ṹͬ�� 
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
		column -= 3;//����Ӧ��ǰ��endǰ
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
			//if() getsym�����������
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


long position(enum symtype k, char* sym_name)//���򷵻ض�Ӧλ�ã����򷵻�-1 ��֤ջ���ű������һ��
{
	bool flag = 1; // 1Ϊû�ҵ� 0�ҵ�
	long id = display[dx];
	while (dx && flag) //dx��1��ʼ
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
	cout << "PL/0 start!\n";
	pc = 0; bp = 1; sp = 0;
	do
	{
		index = code[pc];
		pc++;
		switch (index.f)
		{
			case lit: // ����������ջ��
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

					case 13: //���
						sp = sp - 1;
						cout << datastk[sp + 1];
						break;

					case 14: // \n
						cout << "\n";
						break;

					case 15: //����
						sp = sp + 1;
						cin >> datastk[sp - 1];
						break;
						
				}
				break;

			case lod: //��������ֵ�ŵ�ջ��
				sp = sp + 1; 
				datastk[sp] = datastk[findbase(bp, index.l) + index.a];
				break;

			case sto: //��ջ����ֵ�����͵������� 
				sp = sp - 1;
				datastk[findbase(bp, index.l) + index.a] = datastk[sp + 1]; 
				printf("%10d\n", datastk[sp + 1]);
				break;

			case cal:  //���ù��� ?
				datastk[sp + 1] = findbase(bp, index.l); 
				datastk[sp + 2] = bp;
				datastk[sp + 3] = pc;
				bp = sp + 1;
				pc = index.a;
				break;

			case Int:  //�����ڴ�ռ�  
				sp = sp + index.a;
				break;

			case jmp:  //��������ת
				pc = index.a;
				break;

			case jpc:  //������ת��Ϊ����ת��������ջ��
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

