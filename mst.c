// 13
#include <stdio.h>
#include <stdlib.h>

const int INF = 2147483647;

typedef char bool;
const bool true = 1;
const bool false = 0;

typedef struct Vertex {
	bool visited;
	int len;
	int prev;
} Vertex;

typedef struct Edge {
	int from;
	int to;
	int cost;
} Edge;

int n, m; // n_vertex, n_edge
Vertex *G;
Edge *edges;

int min_vertex()
{
	int i, v;
	for (i = 1; i <= n; ++i) {
		if (G[i].visited == false) {
			v = i;
			break;
		}
	}
	for (i = 1; i <= n; ++i) {
		if (G[i].visited == false && G[i].len < G[v].len)
			v = i;
	}
	return v;
}

int cost(int from, int to)
{
	for (int j = 0; j < m; ++j) {
		if (edges[j].from == from && edges[j].to == to)
			return edges[j].cost;
	}
	return INF;
}

int main()
{
	int len = 0;

	scanf("%d %d", &n, &m);

	// read edges
	edges = (Edge*) malloc(m*sizeof(Edge));
	for (int i = 0; i < m; ++i) {
		scanf("%d %d %d", &edges[i].from, &edges[i].to, &edges[i].cost);
	}
	// init vertces
	G = (Vertex*) malloc((n+1)*sizeof(Vertex));
	for (int i = 1; i <= n; ++i) {
		G[i].visited = false;
		G[i].len = INF;
		G[i].prev = 1;
	}
	G[1].visited = true;
	G[1].len = 0;
	int v = 1;
	for (int i = 1; i < n; ++i) {
		if (G[v].len == INF) {
			puts("-1");
			return 0;
		}
		for (int j = 0; j < m; ++j) {
			if (edges[j].from == v) {
				int to = edges[j].to;
				if (!G[to].visited && G[to].len > edges[j].cost) {
					G[to].len = edges[j].cost;
					G[to].prev = v;
				}
			}
		}

		for (int j = 1; j <= n; ++j) {
			printf("%d\t", G[j].len);
		}
		puts("");

		G[v].visited = true;
		v = min_vertex();
		len += cost(G[v].prev, v);
	}
	printf("%d\n", len);
	return 0;
}

/*
input:
6 15
1 2 5
1 3 3
1 4 7
1 5 4
1 6 2
2 3 4
2 4 6
2 5 2
2 6 6
3 4 6
3 5 1
3 6 1
4 5 10
4 6 8
5 6 3

output:
12
*/
