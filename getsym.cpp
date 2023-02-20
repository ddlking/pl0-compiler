#include"pl0.h"
#include<bits/stdc++.h>

using namespace std;


void error(long n)
{
	cout<<"["<<row<<","<<column<<"] ";
	cout<<"Error("<<n<<")-> "<<err_msg[n]<<"\n";//eg:[2,9] Error(1)-> Found ':=' when expecting '='.
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
			//exit(1);
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
		sym = nul;//�����ַ� 
	}
}
/* �ʷ��������� getsym �ܽ᣺��Դ�ļ��ж���������Ч�ַ������һ�� token ����ʶ����������
Ϊ������/��ʶ��/���ֻ����������š�����Ǳ����֣��� sym �ó���Ӧ�ı��������ͣ������
��ʶ������ sym �ó� ident ��ʾ�Ǳ�ʶ�����ڴ�ͬʱ��id �����д�ŵļ�Ϊ�������ַ������ʶ
�����֡���������֣��� sym ��Ϊ number,ͬʱ num �����д�Ÿ����ֵ�ֵ������������Ĳ�������
��ֱ�Ӱ� sym �ó���Ӧ���͡����������̺� ch �����д�ŵ�����һ��������ʶ����ַ� */


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
		cout<<"File "<<infilename<<" can��t be opened. \n";
		exit(1); 
	}
	
	outfile.open("output.txt", ios::out);
	if(!outfile)
	{
		cout<<"File output.txt can��t be opened. \n";
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

