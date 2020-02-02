// 无向图的最小生成树 (minimum spanning tree)
#include <iostream>
#include <climits>	// UINT_MAX
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

typedef unsigned Id;
const Id N = 6;	// 顶点个数

typedef unsigned Cost;
const Cost INF = UINT_MAX;

typedef Cost Graph[N][N];

// 向最小生成树中添加边
void add_edge(Id from, Id to, Cost cost)
{
	cout << char('A' + from) << char('A' + to) << " = " << cost << '\n';
}

// Prim 算法的边结构
struct PrimEdge {
	Id from;	// 到该顶点最近的顶点
	Cost cost;	// 该顶点与最近顶点的距离
};

// prim 算法, 适合稠密图
void naive_prim(const Graph &g, Id start)
{
	cout << "naive prim:\n";

	// 初始化边数组
	PrimEdge to[N];
	for (Id i = 0; i < N; ++i) {
		to[i].from = start;
		to[i].cost = g[start][i];
	}

	// N-1 次循环产生 N-1 条边
	for (Id cnt = 1; cnt < N; cnt++) {

		// 选择最小边
		Cost cost = INF;
		Id id = INF;
		for (Id i = 0; i < N; ++i) {
			if (to[i].cost < cost && to[i].cost != 0) {
				id = i;
				cost = to[i].cost;
			}
		}

		if (id == INF) {
			cout << "error: the graph is not connected" << endl;
			return;
		}

		// 添加选中的最小边
		add_edge(to[id].from, id, to[id].cost);

		// 将顶点 id 的 cost 置零, 表示已经访问
		to[id].cost = 0;

		// 更新最小边信息
		for (Id i = 0; i < N; ++i) {
			if (g[id][i] < to[i].cost) {
				to[i].from = id;
				to[i].cost = g[id][i];
			}
		}
	}
	cout << endl;
}

// 用优先队列优化的 prim 算法
void prim(const Graph &g, Id start)
{
	cout << "prim:\n";

	PrimEdge to[N];
	for (Id i = 0; i < N; ++i) {
		to[i].from = start;
		to[i].cost = INF;
	}
	to[start].from = start;
	to[start].cost = 0;

	typedef pair<Cost, Id> P;
	priority_queue< P, vector<P>, greater<P> > q;
	q.push(P(to[start].cost, start));
	while (!q.empty()) {
		P p = q.top(); q.pop();
		Id i = p.second;
		if (to[i].cost < p.first)
			continue;
		if (to[i].from != i)
			add_edge(to[i].from, i, to[i].cost);
		to[i].cost = 0;
		for (Id j = 0; j < N; ++j) {
			if (g[i][j] < to[j].cost) {
				to[j].from = i;
				to[j].cost = g[i][j];
				q.push(P(to[j].cost, j));
			}
		}
	}
	cout << endl;
}

// Kruskal 算法的边结构
struct KruskalEdge {
	Id from, to;
	Cost cost;
	KruskalEdge(Id f, Id t, Cost c): from(f), to(t), cost(c) {}
	bool operator<(const KruskalEdge &rhs) {
		return this->cost < rhs.cost;
	}
};

// naive 的 Kruskal 算法, 使用 array 确定结点所在的连通分量
void naive_kruskal(const Graph &g)
{
	cout << "naive kruskal\n";
	Id cnt = 0;

	// 初始化 N 棵树 (连通分量), 每棵树只有一个顶点
	// 这里令第 i 个顶点属于第 i 棵树
	Id tree[N];
	for (Id i = 0; i < N; ++i)
		tree[i] = i;

	// 初始化边数组
	vector<KruskalEdge> edges;
	for (Id i = 0; i < N; ++i) {
		for (Id j = i+1; j < N; ++j) {
			if (g[i][j] != INF) {
				edges.push_back(KruskalEdge(i, j, g[i][j]));
			}
		}
	}
	sort(edges.begin(), edges.end());

	// 每次循环取出最小边
	for (Id i = 0; i < edges.size(); ++i) {
		Id from = edges[i].from;
		Id to = edges[i].to;
		Cost cost = edges[i].cost;

		// from, to 不在一颗树上时, 合并两颗树
		if (tree[from] != tree[to]) {
			++cnt;
			Id tree_to = tree[to];
			for (Id j = 0; j < N; ++j) {
				if (tree[j] == tree_to) {
					tree[j] = tree[from];
				}
			}
			add_edge(from, to, cost);
		}
	}
	if (cnt < N-1)
		cout << "error: the graph is not connected";
	cout << endl;
}

// Kruskal 算法的并查集结点
struct Set {
	Id id;
	Id size;
} set[N];

// 求并查集的根
Id root(Id id)
{
	Set &parent = set[id];
	if (id == parent.id)
		return id;
	parent.id = root(parent.id);
	return parent.id;
}

// 合并两个并查集
void merge(Id a, Id b)
{
	if (set[a].size < set[b].size) {
		set[a].id = b;
		set[a].size += set[b].size;
	} else {
		set[b].id = a;
		set[b].size += set[a].size;
	}
}

// Kruskal 算法, 适合稀疏图
void kruskal(const Graph &g)
{
	cout << "kruskal\n";
	Id cnt = 0;

	// 初始化 N 棵树 (连通分量), 每棵树只有一个顶点
	// 使用并查集表示这些树
	for (Id i = 0; i < N; ++i) {
		set[i].id = i;
		set[i].size = 1;
	}

	// 初始化边数组
	vector<KruskalEdge> edges;
	for (Id i = 0; i < N; ++i) {
		for (Id j = i+1; j < N; ++j) {
			if (g[i][j] != INF) {
				edges.push_back(KruskalEdge(i, j, g[i][j]));
			}
		}
	}
	sort(edges.begin(), edges.end());

	// 每次循环取出最小边
	for (Id i = 0; i < edges.size(); ++i) {
		// edges[i].from, edges[i].to 不在一颗树上时, 合并两颗树
		Id root_from = root(edges[i].from);
		Id root_to = root(edges[i].to);
		if (root_from != root_to) {
			++cnt;
			merge(root_from, root_to);
			add_edge(edges[i].from, edges[i].to, edges[i].cost);
		}
	}
	if (cnt < N-1)
		cout << "error: the graph is not connected";
	cout << endl;
}

int main()
{
	cout << "# 1\n";

	// 邻接矩阵
	Graph g = {
		{0, 6, 1, 5, INF, INF},
		{6, 0, 5, INF, 3, INF},
		{1, 5, 0, 5, 6, 4},
		{5, INF, 5, 0, INF, 2},
		{INF, 3, 6, INF, 0, 6},
		{INF, INF, 4, 2, 6, 0}
	};

	naive_prim(g, 0); // 从顶点 0 开始
	prim(g, 0);
	naive_kruskal(g);
	kruskal(g);

	cout << "# 2\n";

	// 邻接矩阵
	Graph g1 = {
		{0, 5, 3, 7, 4, 2},
		{5, 0, 4, 6, 2, 6},
		{3, 4, 0, 6, 1, 1},
		{7, 6, 6, 0, 10, 8},
		{4, 2, 1, 10, 0, 3},
		{2, 6, 1, 8, 3, 0}
	};

	naive_prim(g1, 0); // 从顶点 0 开始
	prim(g1, 0);
	naive_kruskal(g1);
	kruskal(g1);

	return 0;
}

/*
# 1
naive prim:
AC = 1
CF = 4
FD = 2
CB = 5
BE = 3

prim:
AC = 1
CF = 4
FD = 2
CB = 5
BE = 3

naive kruskal
AC = 1
DF = 2
BE = 3
CF = 4
BC = 5

kruskal
AC = 1
DF = 2
BE = 3
CF = 4
BC = 5

# 2
naive prim:
AF = 2
FC = 1
CE = 1
EB = 2
CD = 6

prim:
AF = 2
FC = 1
CE = 1
EB = 2
CD = 6

naive kruskal
CE = 1
CF = 1
AF = 2
BE = 2
BD = 6

kruskal
CE = 1
CF = 1
AF = 2
BE = 2
BD = 6
*/
