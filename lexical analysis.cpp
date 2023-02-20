#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
char ch;
int state, row = 1, col = 0;
bool isfinal;
string temp;
vector<string> num;
vector<string> var;
vector<string> res = {"program", "const", "var", "procedure", "begin", "end", "if", "then", "else", "while", "do", "call", "read", "write", "odd"};
bool ischar(char x)
{
    return (x <= 'z' && x >= 'a') || (x <= 'Z' && x >= 'A');
}
bool isdigit(char x)
{
    return x <= '9' && x >= '0';
}
bool isempty(char x)
{
    return x == ' ' || x == '\n' || x == '\t';
}
bool ID(string x, string *&loc)
{
    for (int i = 0; i < res.size(); i++)
    {
        if (x == res[i])
            return false;
    }
    for (int i = 0; i < var.size(); i++)
    {
        if (x == var[i])
        {
            loc = &var[i];
            return true;
        }
    }
    var.push_back(x);
    loc = &var[var.size() - 1];
    return true;
}
int main()
{
    fstream in, out;
    in.open("test.txt", ios::in);
    out.open("result.txt", ios::out);
    while (1)
    {
        if (!isfinal)
        {
            in.get(ch);
            if (ch == '\n')
            {
                row++;
                col = 0;
            }
            else
                col++;
            if (in.fail())
            {
                break;
            }
        }
        switch (state)
        {
        case 0:
        {
            if (ischar(ch))
            {
                state = 1;
                temp.push_back(ch);
                isfinal = false;
            }
            else if (isdigit(ch))
            {
                state = 3;
                isfinal = false;
                temp.push_back(ch);
            }
            else if (ch == '*')
            {
                state = 13;
                isfinal = false;
                break;
            }
            else if (ch == '<')
            {
                state = 8;
                isfinal = false;
                break;
            }
            else if (ch == '>')
            {
                state = 9;
                isfinal = false;
                break;
            }
            else if (ch == ':')
            {
                state = 15;
                isfinal = false;
                break;
            }
            else if (isempty(ch))
            {
                state = 0;
                isfinal = false;
                break;
            }
            else
            {
                switch (ch)
                {
                case '=':
                {
                    out << "< = , lop >\n";
                    break;
                }
                case '+':
                case '-':
                {
                    out << "< " << ch << " , aop >\n";
                    break;
                }
                case '(':
                case ')':
                {
                    out << "< " << ch << " , bracket >\n";
                    break;
                }
                case '/':
                {
                    out << "< / , mop >\n";
                    break;
                }
                case ',':
                case ';':
                {
                    out << "< " << ch << " , punctuation >\n";
                    break;
                }
                default:
                {
                    out << "第 " << row << " 行 ， 第 " << col << " 列出现字符错误！\n";
                    cout << "第 " << row << " 行 ， 第 " << col << " 列出现字符错误！\n";
                }
                }
                isfinal = false;
            }
            break;
        }
        case 1:
        {
            if (isdigit(ch) || ischar(ch))
            {
                isfinal = false;
                temp.push_back(ch);
            }
            else
            {
                state = 0;
                isfinal = true;
                string *loc;
                if (ID(temp, loc))
                {
                    out << "< " << temp << " , var ,地址为 " << loc << " >\n";
                }
                else
                {
                    out << "< " << temp << " , res >\n";
                }
                temp = "";
            }
            break;
        }
        case 3:
        {
            if (isdigit(ch))
            {
                isfinal = false;
                temp.push_back(ch);
            }
            else
            {
                state = 0;
                isfinal = true;
                if (ischar(ch))
                {
                    out << "第 " << row << " 行 ， 第 " << col << " 列出现整数错误！\n";
                    cout << "第 " << row << " 行 ， 第 " << col << " 列出现整数错误！\n";
                    temp.push_back(ch);
                    while (ischar(ch))
                    {
                        in.get(ch);
                        if (in.fail())
                        {
                            out << " < " << temp << " , 错误的integer >\n";
                            cout << " < " << temp << " , 错误的integer >\n";
                            in.close();
                            out.close();
                            return 0;
                        }
                        if (ch == '\n')
                        {
                            row++;
                            col = 0;
                        }
                        else
                            col++;
                        if (ischar(ch))
                            temp.push_back(ch);
                        else
                            break;
                    }
                    out << " < " << temp << " , 错误的integer >\n";
                    cout << " < " << temp << " , 错误的integer >\n";
                }
                else
                {
                    num.push_back(temp);
                    out << "< " << temp << " , integer , 地址为 " << (&num[num.size() - 1]) << " >\n";
                }
                temp = "";
            }
            break;
        }
        case 13:
        {
            if (ch == '*')
            {
                out << "< ** , mop >\n";
                isfinal = false;
            }
            else
            {
                out << "< * , mop >\n";
                isfinal = true;
            }
            state = 0;
            break;
        }
        case 8:
        {
            if (ch == '>' || ch == '=')
            {
                out << "< <" << ch << " , lop >\n";
                isfinal = false;
            }
            else
            {
                out << "< < , lop >\n";
                isfinal = true;
            }
            state = 0;
            break;
        }
        case 9:
        {
            if (ch == '=')
            {
                out << "< >= , lop >\n";
                isfinal = false;
            }
            else
            {
                out << "< > , lop >\n";
                isfinal = true;
            }
            state = 0;
            break;
        }
        case 15:
        {
            if (ch == '=')
            {
                out << "< := , lop >\n";
                isfinal = false;
            }
            else
            {
                out << "第 " << row << " 行 ， 第 " << col << " 列 := 符号出现错误！\n";
                cout << "第 " << row << " 行 ， 第 " << col << " 列 := 符号出现错误！\n";
                isfinal = true;
            }
            state = 0;
            break;
        }
        }
    }

    in.close();
    out.close();
    cout << "词法分析完成！";
    return 0;
}
