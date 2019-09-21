// 链式基数排序
#include <stdio.h>

#define LEN 10
#define RADIX 10
#define KEYS 3

// 278 109 063 930 589 184 505 269 008 083

char key[LEN+1][KEYS] = {
	{0, 0, 0},
	{8, 7, 2}, {9, 0, 1}, {3, 6, 0}, {0, 3, 9}, {9, 8, 5},
	{4, 8, 1}, {5, 0, 5}, {9, 6, 2}, {8, 0, 0}, {3, 8, 0}
};
int next[LEN+1];
int first[RADIX];
int last[RADIX];

void distribute(int i)
{
	for (int j = 0; j < RADIX; ++j)
		first[j] = 0;
	for (int p = next[0]; p; p = next[p]) {
		int d = key[p][i];
		if (first[d])
			next[last[d]] = p;
		else
			first[d] = p;
		last[d] = p;
	}
}

void collect(int i)
{
	int tail = 0;
	for (int j = 0; j < RADIX; ++j) {
		if (first[j]) {
			next[tail] = first[j];
			tail = last[j];
		}
	}
	next[tail] = 0;
}

void print()
{
	for (int p = next[0]; p; p = next[p]) {
		for (int j = KEYS-1; j >= 0; --j) {
			putchar(key[p][j] + '0');
		}
		putchar(' ');
	}
	putchar('\n');
}

int main()
{
	for (int i = 0; i < LEN; ++i)
		next[i] = i+1;
	next[LEN] = 0;
	print();

	for (int i = 0; i < KEYS; ++i) {
		distribute(i);
		collect(i);
	}

	print();
	return 0;
}
