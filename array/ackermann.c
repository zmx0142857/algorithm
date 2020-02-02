// Ackermann 函数
// A(m, n) = { n+1,              if m = 0
//           { A(m-1, 1),        if m > 0 and n = 0
//           { A(m-1, A(m, n-1)) if else
#include <stdio.h>
#define N 1000
// expr 放后缀表达式, 用 -1 表示 A 这个符号
int expr[N];
int exprlen = 0;
// op 放数字, 保存计算过程
int op[N];
int oplen = 0;

int A(int m, int n)
{
	if (m == 0)
		return n+1;
	else if (n == 0)
		return A(m-1, 1);
	else
		return A(m-1, A(m, n-1));
}

void print_stack(int *st, int len)
{
	while (len) {
		--len;
		printf("%d ", st[len]);
	}
	putchar('\n');
}

// 后缀表达式
int A_nonrec(int m, int n)
{
	int token; // 符号
	expr[exprlen++] = -1;
	expr[exprlen++] = n;
	expr[exprlen++] = m;
	while (exprlen) {
		//printf("expr: "); print_stack(expr, exprlen);
		//printf("op: "); print_stack(op, oplen);
		token = expr[--exprlen];
		if (token != -1) {
			op[oplen++] = token;
		} else {
			n = op[--oplen];
			m = op[--oplen];
			if (m == 0)
				op[oplen++] = n+1;
			else if (n == 0) {
				expr[exprlen++] = -1;
				expr[exprlen++] = 1;
				expr[exprlen++] = m-1;
			} else {
				expr[exprlen++] = -1;
				expr[exprlen++] = -1;
				expr[exprlen++] = n-1;
				expr[exprlen++] = m;
				expr[exprlen++] = m-1;
			}
		}
	}
	return op[oplen-1];
}

int main()
{
	printf("%d\n", A_nonrec(2, 1));
	return 0;
}

