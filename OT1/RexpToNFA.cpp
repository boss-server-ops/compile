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
	
	cout << "��Ч������ʽ����ĸ���={a-z,A-Z}��������ĸ���={'*','|','(',')','.'}" << endl;
	cout << "'*'   ��ʾ�հ�" << endl << "'|'   ��ʾ����" << endl << "'.'   ��ʾ����" << endl;
	

	cout << "\n������һ������ʽ:";
	string s = "";
	cin >> s;
	if (rt.islegal(s)) {
		cout << "���������ʽ�Ϸ���" << endl;
		string ss = rt.turnToConnect(s);
		cout << "��ȱʡ�Ե����ӷ���'.'֮��Ϊ��" << ss << endl;
		cout << "��Ӧ�ĺ�׺���ʽ���沨��ʽ��Ϊ��" << rt.ReversePolishType(ss);
			
		string rs = rt.ReversePolishType(ss);
		rt.PolishTypeToNFA(rs);
	}
	else {
		cout << "���������ʽ���Ϸ���" << endl << "���������룺";
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
	//NFA����
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
	//DFA״̬��ӳ�䵽��0��ʼ��״̬���
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
		printf("״̬q%d -> ״̬q%d�������ַ�%c\n", mp[ntd.edge[i].from], mp[ntd.edge[i].to], ntd.edge[i].c);
	}
	
	out.close();
}
