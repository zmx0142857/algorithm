#ifndef GRAPH_H
#define GRAPH_H

#include <cstring>

class Graph {
public:
	typedef size_t Vertex;
	struct Edge {
		Vertex from;
		Vertex to;
		int weight;

		Edge(size_t f=-1, size_t t=-1, int w=0):
			from(f), to(t), weight(w) {}
		valid() const
		{
			return weight > 0 && from != size_t(-1) && to != size_t(-1);
		}
	};

	Graph(size_t size): n_vertex(size), n_edge(0),
		visited(new bool[size]), indegree(new size_t[size])
	{
		memset(visited, 0, size*sizeof(bool));
		memset(indegree, 0, size*sizeof(size_t));
	}
protected:
	Vertex n_vertex;
	Vertex n_edge;
	bool *visited;
	size_t *indegree;
};

class MatGraph: public Graph {
public:
	MatGraph(size_t size): Graph(size), mat(new int*[size])
	{
		for (size_t i = 0; i != size; ++i) {
			mat[i] = new int[size];
			memset(mat[i], 0, size*sizeof(int));
		}
	}

	Edge first_edge(Vertex v)
	{
		Edge e(v);
		return next_edge(e);
	}

	// return next edge from 'from'.
	// return a edge to n_vertex if e is the last edge
	Edge next_edge(Edge e)
	{
		Edge ret(e.from, e.to + 1);
		while (ret.to < n_vertex) {
			ret.weight = mat[ret.from][ret.to];
			if (ret.weight > 0)
				break;
			else
				++ret.to;
		}
		return ret;
	}

	void set_edge(Vertex from, Vertex to, int weight=1)
	{
		if (mat[from][to] <= 0) {
			++n_edge;
			++indegree[to];
		}
		mat[from][to] = weight;
	}

	void remove_edge(Vertex from, Vertex to)
	{
		if (mat[from][to] > 0) {
			--n_edge;
			--indegree[to];
		}
		mat[from][to] = 0;
	}

	void dump()
	{
		for (Vertex i = 0; i != n_vertex; ++i) {
			for (Vertex j = 0; j != n_vertex; ++j)
				std::cout << mat[i][j] << ' ';
			std::cout << '\n';
		}
	}

	static void test()
	{
		using namespace std;
		MatGraph g(3);
		g.set_edge(0, 1);
		g.set_edge(0, 2);
		g.set_edge(2, 3);
		g.set_edge(3, 0);
		g.dump();
	}

	int **mat;
};

#endif // GRAPH_H
