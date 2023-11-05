#include <iostream>
#include <cmath>
#include <string>
#include <stack>
#include <queue>
#include "RexpToNFA.h"
#include"NFATDFA.h"
using namespace std;
int main() {
	
	RTN rt;
	
	cout << "有效的正规式的字母表∑={a-z,A-Z}，辅助字母表∑={'*','|','(',')','.'}" << endl;
	cout << "'*'   表示闭包" << endl << "'|'   表示联合" << endl << "'.'   表示连接" << endl;
	

	cout << "\n请输入一个正规式:";
	string s = "";
	cin >> s;
	if (rt.islegal(s)) {
		cout << "输入的正规式合法！" << endl;
		string ss = rt.turnToConnect(s);
		cout << "补缺省略的连接符号'.'之后为：" << ss << endl;
		cout << "对应的后缀表达式（逆波兰式）为：" << rt.ReversePolishType(ss);
			
		string rs = rt.ReversePolishType(ss);
		rt.PolishTypeToNFA(rs);
	}
	else {
		cout << "输入的正规式不合法！" << endl << "请重新输入：";
	}



	//NTd

	NTD ntd;
	memset(ntd.eps_clos, -1, sizeof(ntd.eps_clos));
	ifstream in("NTD.txt");
	ofstream out("dfa.txt");
	in >> ntd.zf;
	for (i = 0; i < ntd.zf; i++) {
		in >> ntd.zfj[i];
	}
	in >> ntd.zt;
	
	in >> ntd.Begin >> ntd.End;
	//NFA边数
	int n;
	in >> n;
	
	int x, y;
	char c;
	for (i = 0; i < n; i++) {
		in >> x >> c >> y;
		ntd.G[x][y] = c;
	}
	in.close();
	ntd.ZJGZ();

	out << ntd.zf << endl;
	for (i = 0; i < ntd.zf; i++) {
		out << ntd.zfj[i] << ' ';
	}
	out << endl;
	//DFA状态集映射到从0开始的状态标号
	map<int, int> mp;
	out << ntd.V.size() << endl;
	for (i = 0; i < ntd.V.size(); i++) {
		out << i << ' ' << ntd.V[i].flag << endl;
		mp[ntd.V[i].s] = i;
	}
	out << ntd.edge.size() << endl;
	for (i = 0; i < ntd.V.size(); i++)
		ntd.out_put(i, ntd.V[i].s, ntd.V[i].flag);
	for (i = 0; i < ntd.edge.size(); i++) {
		out << mp[ntd.edge[i].from] << ' ' << mp[ntd.edge[i].to] << ' ' << ntd.edge[i].c << endl;
		printf("状态q%d -> 状态q%d，吃入字符%c\n", mp[ntd.edge[i].from], mp[ntd.edge[i].to], ntd.edge[i].c);
	}
	
	out.close();
}
