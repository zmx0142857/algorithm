#include <stdio.h>

const unsigned bits = 31;
const unsigned N = 1 << bits;

// 1684996667383472989748638208589148922131258297907555764671493439489
unsigned arr[] = {8, 7, 6, 5, 4, 3, 2, 1};
const unsigned size = sizeof(arr) / sizeof(unsigned);

// 0N 到 9N 除以 10 的结果
unsigned qN[10] = {0, 214748364, 429496729, 644245094, 858993459, 1073741824, 1288490188, 1503238553, 1717986918, 1932735283};
unsigned rN[10] = {0, 8, 6, 4, 2, 0, 8, 6, 4, 2};

unsigned top = 0;
unsigned stack[1000];

void print()
{
	while (top)
		printf("%u", stack[--top]);
	putchar('\n');
}

int main()
{
	unsigned begin = 0;
	while (begin < size) {
		unsigned r = 0;
		for (unsigned i = begin; i < size; ++i) {
			unsigned tmp = rN[r] + arr[i];
			arr[i] = qN[r] + tmp / 10; // 商保存在原地
			r = tmp % 10; // 余数
		}
		stack[top++] = r;
		while (arr[begin] == 0)
			++begin;
	}
	print();

	return 0;
}
