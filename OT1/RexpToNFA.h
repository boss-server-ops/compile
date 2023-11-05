#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <stack>
#include <queue>
#include<fstream>
using namespace std;

int Aflag = 0;//a���ڱ��NFA�е�״̬�����磺A0��A1
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
/*RexpToNFA��������ʽת��ΪNFA����*/
class RTN {
	
public:
	int zf = 0;
	int edgenum = 0;
	string Begin;
	char zfj[101];
	RTN() {};//���캯�������κδ���

	bool isletter(char ch) {//�ж��Ƿ�Ϊ��ĸ
		if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
			return true;
		}
		else {
			return false;
		}
	}

	bool islegal(string s) {//�ж��ַ����Ƿ��������ʽ���﷨
		int Lbracket = 0;//��¼�����ŵĸ���
		int Rbracket = 0;//��¼�����ŵĸ���
		RTN r;
		for (int i = 0; i < s.length(); i++) {
			char ch = s[i];
			if (ch == '*' || ch == '|' || ch == '.') {
				continue;
			}
			//���ֲ������ж�
			else if (ch >= 0 && ch <= 9) {
				continue;
			}//��ĸ�������ж�
			else if (r.isletter(ch)) {
				continue;
			}//������������ţ������Ÿ���+1
			else if (ch == '(') {
				Lbracket++;
			}//������������ţ������Ÿ���+1
			else if (ch == ')') {
				Rbracket++;
			}//�������ķ��Ų��������������ֱ�ӷ��ز��Ϸ�
			else {
				return false;
			}
		}
		//��������Ÿ��������������Ÿ��������ز��Ϸ�����֮���غϷ�
		if (Lbracket != Rbracket) {
			return false;
		}
		else {
			return true;
		}
	}

	//������ı��ʽ�е����ӷ�����'.'��ȫ
	string turnToConnect(string s) {
		string ns = s.substr(0, 1);
		for (int i = 1; i < s.length(); i++) {
			char prech = s[i - 1];
			char ch = s[i];
			//��������ĸ֮����Ҫ��������ӷ���'.'
			if (isletter(prech) && isletter(ch)) {
				ns = ns + '.' + ch;
				//cout << endl << ns;
				continue;
			}
			//�жϵ�ǰ����ĸ������ǰ����������')'�����
			else if (isletter(ch) && prech == ')') {
				ns = ns + '.' + ch;
				continue;
			}
			//�жϵ�ǰ��������'('������ǰ����������')'����ǰ������ĸ�����
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
	//תΪ��׺���ʽ
	string ReversePolishType(string s) {
		stack<char> st;//ջ��������׺���ʽתΪ��׺���ʽ
		string ns = "";//����ַ���

		for (int i = 0; i < s.length(); i++) {//ѭ�������ַ����е�ÿ���ַ�
			char ch = s[i];
			if (isletter(ch)) {//�������ĸ��ֱ�Ӽӵ�����ַ�����
				ns = ns + ch;
			}
			else if (ch == '(') {//�����������ֱ����ջ
				st.push(ch);
			}
			else if (ch == ')') {//����������Ž�ջ�����ݵ������뵽����ַ����У�ֱ������������
				while (st.top() != '(') {
					ns = ns + st.top();
					st.pop();
				}
				st.pop();//��ջ��ʣ��������ŵ���
			}
			else if (ch == '*') {
				//ջ�ջ������ȼ�����ջ��������ʱֱ����ջ
				if (st.empty() || st.top() == '.' || st.top() == '|' || st.top() == '(') {
					st.push(ch);
				}
				else {//�����������ֻ������ջ����*������ѭ����ջ����*������ֱ��ջ�ջ������ȼ�����ջ��������ʱ��ջ
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
			else if (ch == '.') {//�������ȼ�����������ͬ��
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
			else if (ch == '|') {//�������ȼ�����������ͬ��
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
		while (!st.empty()) {//�����ȡ�������ַ���ջ�л��в�������ȫ������
			ns = ns + st.top();
			st.pop();
		}

		return ns;
	}

	//���� |
	Edge Unite(Edge Left, Edge Right) {
		Edge ed1(to_string(Aflag), '#', Left.getBegin());
		Edge ed2(to_string(Aflag), '#', Right.getBegin());
		Edge ed3(Left.getEnd(), '#', to_string(Aflag + 1));
		Edge ed4(Right.getEnd(), '#', to_string(Aflag + 1));
		q.push(ed1);
		q.push(ed2);
		q.push(ed3);
		q.push(ed4);
		//thro���ǿո�˵����ԭ�ӱߣ���Ҫ���������
		if (Left.getThro() != ' ') {
			this->q.push(Left);
		}
		if (Right.getThro() != ' ') {
			this->q.push(Right);
		}
		//�õ����µı߽�thro��Ϊ�ո��Ժ�ԭ�ӱ߽�������
		Edge ed(to_string(Aflag), ' ', to_string(Aflag + 1));
		Aflag = Aflag + 2;
		return ed;
	}

	//���� ����
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

	//���� �հ�
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
	//����Ĳ���Ϊ��׺���ʽ���˴�ΪThompson�㷨�ĺ��Ĳ���
	void PolishTypeToNFA(string s) {
		stack<Edge> st;
		Aflag = 0;
		for (int i = 0; i < s.length(); i++) {
			char ch = s[i];

			if (this->isletter(ch)) {//���ַ�Ϊ��ĸʱ������һ��Edge���󣬸���AFlag������ͬ״̬�Ľڵ㣬����thro����Ϊ���ַ�
				Edge ed(to_string(Aflag), ch, to_string(Aflag + 1));
				Aflag = Aflag + 2;
				st.push(ed);//����ջ�У���Ϊ�˴��൱�ڽ���׺���ʽ�ļ��㣬��Ҫ��ջ������
			}
			else if (ch == '*') {
				zf++;
				Edge ed = st.top();//��Ŀ���������ջ��һ��Ԫ��
				st.pop();
				st.push(this->Self(ed));//ѹ���¹����ıհ�����ʽ
				Begin = to_string(Aflag - 2);
			}
			else if (ch == '.') {
				zf++;
				Edge ed1 = st.top();
				st.pop();
				Edge ed2 = st.top();
				st.pop();//������˫Ŀ�����������Ҫ��������Ԫ��
				Edge ed = Join(ed2, ed1);
				st.push(ed);//�����������������Ԫ��
				Begin = ed2.getBegin();
			}
			else if (ch == '|') {
				zf++;
				Edge ed1 = st.top();
				st.pop();
				Edge ed2 = st.top();
				st.pop();
				st.push(this->Unite(ed1, ed2));//������ͬ��
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
		outfile << 53 << endl; //�ֱ�Ϊ�ַ������ڵ�������ʼ�ڵ�״̬��������ֹ�ڵ�״̬,����
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
		
			cout << endl << "��ʼ���Ϊ��" << e.getBegin() << "     ת������Ϊ��" << e.getThro() << "     �������Ϊ��" << e.getEnd();
			p.pop();//��������Ԫ�ز����
		}
		
		outfile.close();
	}

private:
	queue<Edge> q;
	queue<Edge> p;
};



