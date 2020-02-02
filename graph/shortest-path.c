#include <stdio.h>
#include <limits.h>

/*     10       1
 *  .----->(1)------>(2)
 *  |       |^    T| |^
 *  |      2||   /  4||
 *  |       ||3 /9   ||6
 *  |       v| /     v|
 * (0)----->(4)----->(3)
 *  ^   5        2   |
 *  `----------------'
 *          7
 */

#define N 5
#define E 10

int first[N] = {
	0, 2, 4, 5, 7
};
int next[E] = {
	1, -1,
	3, -1,
	-1,
	6, -1,
	8, 9, -1
};
int arcs[E] = {
	1, 4,
	2, 4,
	3,
	0, 2,
	1, 2, 3
};
int weight[E] = {
	10, 5,
	1, 2,
	4,
	7, 6,
	3, 9, 2
};

int size = 0;
int queue[N]; // 手动实现的优先级队列
int idx[N];   // 顶点 i 在队列中的下标
int from[N];  // 顶点 i 在最短路径上的前驱
int cost[N];  // 起点到顶点 i 的最短距离

#define left(i)		(((i)<<1) + 1)
#define right(i)	(((i)<<1) + 2)
#define parent(i)	(((i)-1) >> 1)

void siftup(int i) {
	int tmp = queue[i];
	while (i > 0 && cost[queue[parent(i)]] > cost[tmp]) {
		queue[i] = queue[parent(i)];
		idx[queue[i]] = i;
		i = parent(i);
	}
	queue[i] = tmp;
	idx[queue[i]] = i;
}

void siftdown(int i) {
	int tmp = queue[i];
	int j = left(i);
	while (j < size) {
		if (j < size-1 && cost[queue[j]] > cost[queue[j+1]])
			++j;
		if (tmp > queue[j]) {
			queue[i] = queue[j];
			idx[queue[i]] = i;
			i = j;
			j = left(j);
		} else
			break;
		queue[i] = tmp;
		idx[queue[i]] = i;
	}
}

void queue_update(int i) {
	if (cost[queue[parent(i)]] > cost[queue[i]])
		siftup(i);
	else
		siftdown(i);
}

void enqueue(int v) {
	queue[size] = v;
	idx[queue[size]] = size; // sync
	siftup(size);
	++size;
}

int dequeue() {
	--size;
	int tmp = queue[0];		// swap head top and bottom
	queue[0] = queue[size];
	queue[size] = tmp;
	idx[queue[0]] = 0;	// sync
	idx[queue[size]] = size;
	siftdown(0);
	return tmp;
}

void print_heap() {
	putchar('[');
	for (int i = 0; i < size; ++i)
		printf("%d ", queue[i]);
	puts("]");
}

void dijkstra(int v)
{
	for (int j = 0; j < N; ++j) {
		from[j] = v;
		cost[j] = (j == v ? 0 : INT_MAX);
		enqueue(j);
	}
	for (int n = 0; n < N; ++n) {
		int i = dequeue();
		for (int p = first[i]; p != -1; p = next[p]) {
			int newcost = cost[i] + weight[p];
			int j = arcs[p];
			if (newcost < cost[j]) {
				from[j] = i;
				cost[j] = newcost;
				//printf("newarc:(%d,%d), cost: %d\n", i, j, newcost);
				queue_update(idx[j]);
			}
		}
	}
	// print solution
	for (int i = 0; i < N; ++i) {
		putchar('[');
		for (int j = i; j != v; j = from[j])
			printf("%d ", j);
		printf("%d ", v);
		printf("]: cost %d\n", cost[i]);
	}
}

int main()
{
	dijkstra(0);
	return 0;
}

/* output:
newarc:(0,1), cost: 10
newarc:(0,4), cost: 5
newarc:(4,1), cost: 8
newarc:(4,2), cost: 14
newarc:(4,3), cost: 7
newarc:(3,2), cost: 13
newarc:(1,2), cost: 9
[0 ]: cost 0
[1 4 0 ]: cost 8
[2 1 4 0 ]: cost 9
[3 4 0 ]: cost 7
[4 0 ]: cost 5
*/
