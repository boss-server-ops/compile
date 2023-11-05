#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <cstring>
#include <map>
#include"RexpToNFA.h"

using namespace std;

int i = 0;
//����ַ���
const int MAX = 101;

class NTD {
public:
	struct Node {
		//DFA�ڵ�
		int s;//��s����λ�������Թ����������ЩNFA�ڵ�
		bool flag;
		//���һ��DFA�ڵ㼯���Ƿ����NFA����̬,����ʾDFA���Ƿ���һ������̬
		Node(int ss, bool f) {//���캯����ʼ��
			s = ss;
			flag = f;
		}
	};

	struct Edge2 {
		//DFA��
		int from, to;//ǰһ���ڵ����һ���ڵ�
		char c;//ת�Ʊ��ϵ��ַ�
		Edge2(int x, int y, char z) {//���캯��
			from = x;
			to = y;
			c = z;
		}
	};

	
	//�ַ���
	int zf;
	//�ַ���
	char zfj[MAX];
	//״̬����0��ʼ
	int zt;
	//��ʼ�ͽ���̬
	int Begin, End;
	//���
	char G[MAX][MAX];
	//���NFA״̬�Ƿ񱻷��ʣ���հ��õ�
	int vis[MAX];
	//DFA�ڵ㼯
	vector<Node> V;
	//DFA�߼�
	vector<Edge2> edge;
	//����ıհ������Ժ���
	int eps_clos[MAX];
	queue<int> eq;

	//��eps�հ�
	int E_closure(int x) {
		if (eps_clos[x] != -1) {
			return eps_clos[x];//����Ѿ�������հ��ģ�ֱ�ӷ��ؼ���
		}
		queue<int> q;
		memset(vis, 0, sizeof(vis));//����vis������������հ�
		int S = 0;//��ʼ��Ϊ0��������Ҫ����λ����
		S = S | (1 << x);//NFA�е�״̬���Ǽ����ƶ���λ������ֻҪ�ж����λ���ǲ���1�ͺ���
		q.push(x);//ΪBFS��׼��
		while (!q.empty()) {
			//BFS��հ�
			int v = q.front();
			q.pop();//������������
			for (int w = 0; w < zt; w++) {//ѭ������NFA
				if (G[v][w] == '#' && !vis[w]) {//��������ڵ�֮���ǿ��ַ��Ҹýڵ�δ������
					vis[w] = 1;//��Ǹýڵ㱻������
					S |= (1 << w);//����ͨ����λ���
					q.push(w);//��������м���BFS
				}
			}
		}
		eps_clos[x] = S;//BFS������
		return S;
	}

	int set_edge(int s, char c) {//����һ��������������һ��״̬�������ַ� c ת�Ƶ���״̬����
		//��һ��״̬�����ַ������״̬��
		
		int i, j;
		int S = 0;
		for (i = 0; i < zt; i++) {//��������NFA�Ľڵ�
			if ((s >> i) & 1) {//���ƥ�䵽������״̬����NFA�ڵ�
				for (j = 0; j < zt; j++) {//�ٱ�������NFA
					
					if (G[i][j] == c)//����ýڵ㵽��һ���ڵ���ת�Ʊ������ַ�c��
						S |= E_closure(j);//���ýڵ�ıհ������ַ���
				}
			}
		}
		return S;//�����ַ���
	}

	bool check(int s) {
		//���DFA�ڵ㼯�Ƿ���ֹ�
		for (int i = 0; i < V.size(); i++) {
			if (V[i].s == s) return true;
		}
		return false;
	}

	bool is_end(int s) {
		//״̬���Ƿ�����ս��
		return (s >> End) & 1;
	}

	void ZJGZ() {
		//�Ӽ������㷨
		int i;
		queue<int> work;
		
		work.push(E_closure(Begin));//����һ������ work�����ڴ洢�������״̬����
		//����DFA�ڵ㼯
		V.push_back(Node(E_closure(Begin), is_end(E_closure(Begin))));//����ʼ״̬�ıհ�����DFA�ڵ㼯��������Ƿ�����ս�㡣
		while (!work.empty()) {//ѭ������δ�����״̬��
			
			int v = work.front();//ȡ����ͷ��״̬��
			
			work.pop();
			for (i = 0; i < zf; i++) {
				//�����ַ���
				//����NFA������ַ����ܵ��������״̬
				int s = set_edge(v, zfj[i]);
				if (s != 0) {
					edge.push_back(Edge2(v, s, zfj[i]));////����ܵ����״̬����Ϊ�գ�˵���бߣ��������߼���߼�
					if (!check(s)) {//����ýڵ�û��DFA�ڵ㼯������ֹ�
						V.push_back(Node(s, is_end(s)));//���ýڵ����DFA�ڵ㼯��
						work.push(s);//���Ҽ���δ�����״̬������
					}
				}
			}
		}
	}

	void out_put(int i, int s, bool f) {
		printf("DFA״̬��q%d ������NFA�е�״̬Ϊ��", i);
		for (int j = 0; j < zt; j++)
			if ((s >> j) & 1) printf("s%d ", j);
		if (f) printf("��������̬\n");
		else printf("����������̬\n");
	}
};




