#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <cstring>
#include <map>
#include"RexpToNFA.h"

using namespace std;

int i = 0;
//最大字符数
const int MAX = 101;

class NTD {
public:
	struct Node {
		//DFA节点
		int s;//对s进行位操作可以管理包含了哪些NFA节点
		bool flag;
		//标记一个DFA节点集合是否包含NFA结束态,即表示DFA中是否是一个结束态
		Node(int ss, bool f) {//构造函数初始化
			s = ss;
			flag = f;
		}
	};

	struct Edge2 {
		//DFA边
		int from, to;//前一个节点和下一个节点
		char c;//转移边上的字符
		Edge2(int x, int y, char z) {//构造函数
			from = x;
			to = y;
			c = z;
		}
	};

	
	//字符数
	int zf;
	//字符集
	char zfj[MAX];
	//状态数，0开始
	int zt;
	//起始和结束态
	int Begin, End;
	//存边
	char G[MAX][MAX];
	//标记NFA状态是否被访问，求闭包用到
	int vis[MAX];
	//DFA节点集
	vector<Node> V;
	//DFA边集
	vector<Edge2> edge;
	//求过的闭包保存以后用
	int eps_clos[MAX];
	queue<int> eq;

	//求eps闭包
	int E_closure(int x) {
		if (eps_clos[x] != -1) {
			return eps_clos[x];//如果已经计算过闭包的，直接返回即可
		}
		queue<int> q;
		memset(vis, 0, sizeof(vis));//重置vis数组用来计算闭包
		int S = 0;//初始化为0，后续需要进行位操作
		S = S | (1 << x);//NFA中的状态数是几就移动几位，后续只要判断这个位上是不是1就好了
		q.push(x);//为BFS做准备
		while (!q.empty()) {
			//BFS求闭包
			int v = q.front();
			q.pop();//弹出队列列首
			for (int w = 0; w < zt; w++) {//循环遍历NFA
				if (G[v][w] == '#' && !vis[w]) {//如果两个节点之间是空字符且该节点未被访问
					vis[w] = 1;//标记该节点被访问了
					S |= (1 << w);//并且通过移位标记
					q.push(w);//放入队列中继续BFS
				}
			}
		}
		eps_clos[x] = S;//BFS结束后
		return S;
	}

	int set_edge(int s, char c) {//定义一个函数，用于求一个状态集经过字符 c 转移到的状态集。
		//求一个状态集吃字符到达的状态集
		
		int i, j;
		int S = 0;
		for (i = 0; i < zt; i++) {//遍历整个NFA的节点
			if ((s >> i) & 1) {//如果匹配到了输入状态集的NFA节点
				for (j = 0; j < zt; j++) {//再遍历整个NFA
					
					if (G[i][j] == c)//如果该节点到另一个节点有转移边上是字符c的
						S |= E_closure(j);//将该节点的闭包加入字符集
				}
			}
		}
		return S;//返回字符集
	}

	bool check(int s) {
		//检查DFA节点集是否出现过
		for (int i = 0; i < V.size(); i++) {
			if (V[i].s == s) return true;
		}
		return false;
	}

	bool is_end(int s) {
		//状态集是否包含终结点
		return (s >> End) & 1;
	}

	void ZJGZ() {
		//子集构造算法
		int i;
		queue<int> work;
		
		work.push(E_closure(Begin));//定义一个队列 work，用于存储待处理的状态集。
		//加入DFA节点集
		V.push_back(Node(E_closure(Begin), is_end(E_closure(Begin))));//将初始状态的闭包加入DFA节点集，并标记是否包含终结点。
		while (!work.empty()) {//循环处理未处理的状态集
			
			int v = work.front();//取出队头的状态集
			
			work.pop();
			for (i = 0; i < zf; i++) {
				//遍历字符集
				//生成NFA吃完该字符所能到达的所有状态
				int s = set_edge(v, zfj[i]);
				if (s != 0) {
					edge.push_back(Edge2(v, s, zfj[i]));////如果能到达的状态集不为空，说明有边，将这条边加入边集
					if (!check(s)) {//如果该节点没在DFA节点集合里出现过
						V.push_back(Node(s, is_end(s)));//将该节点加入DFA节点集合
						work.push(s);//并且加入未处理的状态集合中
					}
				}
			}
		}
	}

	void out_put(int i, int s, bool f) {
		printf("DFA状态：q%d 包含的NFA中的状态为：", i);
		for (int j = 0; j < zt; j++)
			if ((s >> j) & 1) printf("s%d ", j);
		if (f) printf("包含结束态\n");
		else printf("不包含结束态\n");
	}
};




