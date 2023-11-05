#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <stack>
#include <queue>
#include<fstream>
using namespace std;

int Aflag = 0;//a用于标记NFA中的状态，例如：A0、A1
ofstream outfile("NTD.txt");

class Edge {
public:
	Edge() {
		this->begin = "";
		this->thro = ' ';
		this->end = "";
	};

	Edge(string begin, char thro, string end) {
		this->begin = begin;
		this->thro = thro;
		this->end = end;
	}

	void setThro(char ch) {
		this->thro = ch;
	}

	char getThro() {
		return this->thro;
	}

	void setBegin(string ch) {
		this->begin = ch;
	}

	string getBegin() {
		return this->begin;
	}

	void setEnd(string ch) {
		this->end = ch;
	}

	string getEnd() {
		return this->end;
	}


private:
	char thro;
	string begin;
	string end;

};
/*RexpToNFA将正则表达式转换为NFA的类*/
class RTN {
	
public:
	int zf = 0;
	int edgenum = 0;
	string Begin;
	char zfj[101];
	RTN() {};//构造函数不做任何处理

	bool isletter(char ch) {//判断是否为字母
		if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
			return true;
		}
		else {
			return false;
		}
	}

	bool islegal(string s) {//判断字符串是否符合正规式的语法
		int Lbracket = 0;//记录左括号的个数
		int Rbracket = 0;//记录右括号的个数
		RTN r;
		for (int i = 0; i < s.length(); i++) {
			char ch = s[i];
			if (ch == '*' || ch == '|' || ch == '.') {
				continue;
			}
			//数字不考虑判断
			else if (ch >= 0 && ch <= 9) {
				continue;
			}//字母不考虑判断
			else if (r.isletter(ch)) {
				continue;
			}//如果输入左括号，左括号个数+1
			else if (ch == '(') {
				Lbracket++;
			}//如果输入右括号，右括号个数+1
			else if (ch == ')') {
				Rbracket++;
			}//如果输入的符号不符合以上情况，直接返回不合法
			else {
				return false;
			}
		}
		//如果左括号个数不等于右括号个数，返回不合法，反之返回合法
		if (Lbracket != Rbracket) {
			return false;
		}
		else {
			return true;
		}
	}

	//将输入的表达式中的连接符号用'.'补全
	string turnToConnect(string s) {
		string ns = s.substr(0, 1);
		for (int i = 1; i < s.length(); i++) {
			char prech = s[i - 1];
			char ch = s[i];
			//连续的字母之间需要添加上连接符号'.'
			if (isletter(prech) && isletter(ch)) {
				ns = ns + '.' + ch;
				//cout << endl << ns;
				continue;
			}
			//判断当前是字母，并且前面是右括号')'的情况
			else if (isletter(ch) && prech == ')') {
				ns = ns + '.' + ch;
				continue;
			}
			//判断当前是左括号'('，并且前面是右括号')'或者前面是字母的情况
			else if (ch == '(' && prech == ')' || ch == '(' && isletter(prech)) {
				ns = ns + '.' + ch;
				continue;
			}
			else {
				ns += ch;
				//cout << endl << ns;
				continue;
			}
		}
		return ns;
	}
	//转为后缀表达式
	string ReversePolishType(string s) {
		stack<char> st;//栈用来将中缀表达式转为后缀表达式
		string ns = "";//结果字符串

		for (int i = 0; i < s.length(); i++) {//循环遍历字符串中的每个字符
			char ch = s[i];
			if (isletter(ch)) {//如果是字母，直接加到结果字符串中
				ns = ns + ch;
			}
			else if (ch == '(') {//如果是左括号直接入栈
				st.push(ch);
			}
			else if (ch == ')') {//如果是右括号将栈中内容弹出加入到结果字符串中，直到碰到左括号
				while (st.top() != '(') {
					ns = ns + st.top();
					st.pop();
				}
				st.pop();//将栈顶剩余的左括号弹出
			}
			else if (ch == '*') {
				//栈空或者优先级高于栈顶操作符时直接入栈
				if (st.empty() || st.top() == '.' || st.top() == '|' || st.top() == '(') {
					st.push(ch);
				}
				else {//出现其他情况只可能是栈顶是*，所以循环将栈顶的*弹出，直到栈空或者优先级高于栈顶操作符时入栈
					while (st.top() == '*') {
						ns = ns + st.top();
						st.pop();
						if (st.empty()) break;
					}
					if (st.empty() || st.top() == '.' || st.top() == '|' || st.top() == '(') {
						st.push(ch);
					}
				}
			}
			else if (ch == '.') {//除了优先级有区别其他同理
				if (st.empty() || st.top() == '|' || st.top() == '(') {
					st.push(ch);
				}
				else {
					while (st.top() == '*' || st.top() == '.') {
						ns = ns + st.top();
						st.pop();
						if (st.empty()) break;
					}
					if (st.empty() || st.top() == '|' || st.top() == '(') {
						st.push(ch);
					}
				}
			}
			else if (ch == '|') {//除了优先级有区别其他同理
				if (st.empty() || st.top() == '(') {
					st.push(ch);
				}
				else {
					while (st.top() == '*' || st.top() == '.' || st.top() == '|') {
						ns = ns + st.top();
						st.pop();
						if (st.empty()) break;
					}
					if (st.empty() || st.top() == '(') {
						st.push(ch);
					}
				}
			}
		}
		while (!st.empty()) {//如果读取完所有字符后栈中还有操作符，全部弹出
			ns = ns + st.top();
			st.pop();
		}

		return ns;
	}

	//处理 |
	Edge Unite(Edge Left, Edge Right) {
		Edge ed1(to_string(Aflag), '#', Left.getBegin());
		Edge ed2(to_string(Aflag), '#', Right.getBegin());
		Edge ed3(Left.getEnd(), '#', to_string(Aflag + 1));
		Edge ed4(Right.getEnd(), '#', to_string(Aflag + 1));
		q.push(ed1);
		q.push(ed2);
		q.push(ed3);
		q.push(ed4);
		//thro不是空格说明是原子边，需要将边入队列
		if (Left.getThro() != ' ') {
			this->q.push(Left);
		}
		if (Right.getThro() != ' ') {
			this->q.push(Right);
		}
		//得到的新的边将thro置为空格以和原子边进行区分
		Edge ed(to_string(Aflag), ' ', to_string(Aflag + 1));
		Aflag = Aflag + 2;
		return ed;
	}

	//处理 连接
	Edge Join(Edge Left, Edge Right) {
		
		Edge ed1(Left.getEnd(), '#', Right.getBegin());
		q.push(ed1);
		if (Left.getThro() != ' ') {
			q.push(Left);
		}
		if (Right.getThro() != ' ') {
			q.push(Right);
		}


		Edge ed(Left.getBegin(), ' ', Right.getEnd());
		return ed;
	}

	//处理 闭包
	Edge Self(Edge edge) {
		
		Edge ed1(to_string(Aflag), '#', edge.getBegin());
		Edge ed2(edge.getEnd(), '#', to_string(Aflag + 1));
		Edge ed3(edge.getEnd(), '#', edge.getBegin());
		Edge ed4(to_string(Aflag), '#', to_string(Aflag+1));
		this->q.push(ed1);
		this->q.push(ed2);
		this->q.push(ed3);
		this->q.push(ed4);

		if (edge.getThro() != ' ') {
			this->q.push(edge);
		}

		Aflag = Aflag + 2;
		Edge ed(to_string(Aflag), ' ', to_string(Aflag + 1));
		return ed;
	}
	//传入的参数为后缀表达式，此处为Thompson算法的核心部分
	void PolishTypeToNFA(string s) {
		stack<Edge> st;
		Aflag = 0;
		for (int i = 0; i < s.length(); i++) {
			char ch = s[i];

			if (this->isletter(ch)) {//当字符为字母时，创建一个Edge对象，根据AFlag创建不同状态的节点，并将thro设置为该字符
				Edge ed(to_string(Aflag), ch, to_string(Aflag + 1));
				Aflag = Aflag + 2;
				st.push(ed);//加入栈中，因为此处相当于将后缀表达式的计算，需要用栈来进行
			}
			else if (ch == '*') {
				zf++;
				Edge ed = st.top();//单目运算符弹出栈顶一个元素
				st.pop();
				st.push(this->Self(ed));//压入新构建的闭包正规式
				Begin = to_string(Aflag - 2);
			}
			else if (ch == '.') {
				zf++;
				Edge ed1 = st.top();
				st.pop();
				Edge ed2 = st.top();
				st.pop();//连接是双目运算符，所以要弹出两个元素
				Edge ed = Join(ed2, ed1);
				st.push(ed);//将进行完连接运算的元素
				Begin = ed2.getBegin();
			}
			else if (ch == '|') {
				zf++;
				Edge ed1 = st.top();
				st.pop();
				Edge ed2 = st.top();
				st.pop();
				st.push(this->Unite(ed1, ed2));//与连接同理
				Begin = to_string(Aflag - 2);
			}
		}
		
		while (!this->q.empty())
		{
			Edge e = q.front();
			p.push(e);
			zf++;
			edgenum++;
			q.pop();
		}
		outfile << 53 << endl; //分别为字符数，节点数，开始节点状态，结束终止节点状态,边数
		for (char letter = 'a'; letter <= 'z'; letter++) {
			outfile << letter << ' '; 
		}
		for (char letter = 'A'; letter <= 'Z'; letter++) {
			outfile << letter << ' ';
		}
		outfile << '#' << ' ';
		outfile << Aflag << ' ' << Begin << ' ' << Aflag - 1 << ' ' << edgenum << endl;
		while (!this->p.empty())
		{
			Edge e = p.front();
			outfile << e.getBegin()  << ' ' << e.getThro() << ' ' << e.getEnd()  << endl;
		
			cout << endl << "开始结点为：" << e.getBegin() << "     转换过程为：" << e.getThro() << "     结束结点为：" << e.getEnd();
			p.pop();//弹出队列元素并输出
		}
		
		outfile.close();
	}

private:
	queue<Edge> q;
	queue<Edge> p;
};



