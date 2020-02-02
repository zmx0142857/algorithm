#include <stdio.h>
#include <stdbool.h>

/*    0---1
 *   / \ /|
 *  2   X |
 *   \ / \|
 *    3---4
 */

/* paths from 0 to 3
0 1 3 
0 1 4 3 
0 2 3 
0 4 1 3 
0 4 3 
*/

#define N 5
#define E 14
int first[N] = {0, 3, 6, 8, 11};
int next[E] = {
	1, 2, -1,
	4, 5, -1,
	7, -1,
	9, 10, -1,
	12, 13, -1
};
int edges[E] = {
	1, 2, 4,
	0, 3, 4,
	0, 3,
	1, 2, 4,
	0, 1, 3
};
bool visited[N];
int path[N];

void print(int *path, int depth)
{
	for (int i = 0; i < depth; ++i)
		printf("%d ", path[i]);
	putchar('\n');
}

#define target 3

void find_path(int v, int depth)
{
	path[depth++] = v;
	visited[v] = true;
	if (v == target)
		print(path, depth);
	else {
		for (int w = first[v]; w != -1; w = next[w]) {
			if (!visited[edges[w]])
				find_path(edges[w], depth);
		}
	}
	visited[v] = false;
}

int main()
{
	find_path(0, 0);
	return 0;
}

