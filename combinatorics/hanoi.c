#include <stdio.h>

void move(char a, char b)
{
	char str[7];
	sprintf(str, "%c -> %c", a, b);
}

void hanoi(int n, int a, int b, int c)
{
	if (n == 1) {
		move(a, c);
	} else {
		hanoi(n-1, a, c, b);
		move(a, c);
		hanoi(n-1, b, a, c);
	}
}

int main()
{
	// 2.240s
	hanoi(24, 'a', 'b', 'c');
	return 0;
}
