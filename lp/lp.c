// linear programming
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define mat_i (mat + i*(m+n+1))
#define vec_i mat_i[n+m]
#define EPS 1e-8
#define DIGITS "6.2" // 6.2

void print_table(double *mat, int *base, int m, int n)
{
	int i = m;
	printf("z\t| ");
	for (int j = 0; j < m+n; ++j)
		printf("%" DIGITS "lf ", mat_i[j]);
	printf("| ");
	printf("%" DIGITS "lf \n", vec_i);
	for (int i = 0; i < m; ++i) {
		printf("x%d\t| ", base[i]);
		for (int j = 0; j < m+n; ++j)
			printf("%" DIGITS "lf ", mat_i[j]);
		printf("| ");
		printf("%" DIGITS "lf \n", vec_i);
	}
	putchar('\n');
}

// 换入变量为 swapin, 换出变量为 base[swapout]
void rotate(double *mat, int *base, int m, int n, int swapin, int swapout, int addone)
{
	base[swapout] = swapin + addone;
	int i = swapout;
	double pivot = mat_i[swapin];
	for (int j = 0; j < m+n; ++j)
		mat_i[j] /= pivot;
	vec_i /= pivot;
	for (int i = 0; i <= m; ++i) {
		if (i != swapout) {
			double r = -mat_i[swapin];
			for (int j = 0; j <= m+n; ++j)
				mat_i[j] += (mat + swapout*(m+n+1))[j] * r;
		}
	}
}

// 求解一个具有初始可行基解的松弛形
int solve_slack(double *mat, int *base, int m, int n, int instruction)
{
	int addone = (instruction == -1);

	putchar('\n');

	print_table(mat, base, m, n);
	// 换入换出指示
	if (instruction > -1)
		rotate(mat, base, m, n, 0, instruction, addone);

	#define RUNNING 0
	#define FOUND 1
	#define UNBOUNDED 2
	#define NO_SOLUTION 3
	#define TODO 4
	int status = RUNNING;
	// (n+m choose m)
	int cnt = 1;
	for (int i = 1; i <= m; ++ i) {
		cnt *= n+i;
		cnt /= i;
	}
	while (status == RUNNING && cnt--) {

		// 打印计算过程
		print_table(mat, base, m, n);

		// 确定换入变量
		int swapin = 0;
		int i = m;
		while (swapin < m+n && mat_i[swapin] > -EPS)
			++swapin;
		if (swapin == m+n) {
			puts("solution found");
			status = FOUND;
			break;
		}

		// 计算最紧约束, 确定换出变量
		int swapout = -1;
		const double inf = 1.0 / 0.0;
		double min = inf;
		for (int i = 0; i < m; ++i) {
			double tmp = mat_i[swapin] > EPS ? vec_i / mat_i[swapin] : inf;
			if (tmp < min) {
				min = tmp;
				swapout = i;
			}
		}
		if (swapout == -1) {
			puts("unbounded solution");
			status = UNBOUNDED;
			break;
		}

		// 旋转
		rotate(mat, base, m, n, swapin, swapout, addone);
	}

	// 超过最大迭代次数限制
	if (status == RUNNING) {
		puts("max iterations exceeded");
	}
	// 辅助线性规划是否取得最优解 0
	else if (status == FOUND && instruction > -1) {
		int i = m;
		if (-EPS < mat_i[m+n] && mat_i[m+n] < EPS) {
			status = TODO;
			// 若 x0 是基变量, 执行一次退化旋转使它成为非基变量
			for (int i = 0; i < m; ++i) {
				if (base[i] == 0) {
					for (int j = 0; j < m+n; ++j) {
						int isbase = 0;
						for (int k = 0; k < m; ++k) {
							if (base[k] == j) {
								isbase = 1;
								break;
							}
						}
						if (!isbase && (mat_i[j] < -EPS || mat_i[j] > EPS)) {
							rotate(mat, base, m, n, j, i, 0);
							putchar('\n');
							print_table(mat, base, m, n);
							break;
						}
					}
					break;
				}
			}
			puts("the helper LP has got optimal value");
		} else {
			puts("the helper LP failed. original problem has no solution");
			status = NO_SOLUTION;
		}
	}

	return status;
}

// 以单纯形法求解标准形线性规划
//   max z = c^T x
//   A x <= b
//   x >= 0
// A 是 m x n 矩阵, b 是 m 维向量, c 是 n 维向量
void simplex(double *A, double *b, double *c, int m, int n)
{
	// mat 是 (m+1) * (m+n+1) 矩阵
	double *mat = malloc(sizeof(double) * (m+1) * (m+n+1));
	for (int i = 0; i < m; ++i) {
		memcpy(mat_i, A + i*n, sizeof(double)*n);
		for (int j = 0; j < m; ++j)
			mat_i[n+j] = (i == j);
		vec_i = b[i];
	}
	int i = m;
	for (int j = 0; j < n; ++j)
		mat_i[j] = -c[j];
	for (int j = 0; j <= m; ++j)
		mat_i[n+j] = 0;

	// 判断初始基解是否可行
	double minb = 0;
	int swapout = -1;
	for (int i = 0; i < m; ++i) {
		if (b[i] < minb) {
			minb = b[i];
			swapout = i;
		}
	}

	int *base = malloc(sizeof(int) * m);
	for (int i = 0; i < m; ++i)
		base[i] = n + i + 1;
	if (swapout > -1) {
		// 构造辅助线性规划, 引入人工变量 x0, 要求最大化 -x0
		double *help = malloc(sizeof(double) * (m+1) * (m+n+2));
		for (int i = 0; i < m; ++i) {
			help[i*(m+n+2)] = -1;
			memcpy(help + i*(m+n+2) + 1, mat_i, sizeof(double)*(m+n+1));
		}
		help[m*(m+n+2)] = 1;
		for (int j = 1; j <= m+n+1; ++j)
			help[m*(m+n+2) + j] = 0;
		
		// 换入 x0, 换出 swapout
		puts("\nconstructing helper LP...");
		if (solve_slack(help, base, m, n+1, swapout) == TODO) {
			// 去掉 x0, 重置目标函数
			for (int i = 0; i < m; ++i) {
				memcpy(mat_i, help + i*(m+n+2) + 1, sizeof(double)*(m+n+1));
				for (int j = 0; j <= m+n; ++j)
					if (mat_i[j] < -1e8) {
						printf("%d %d %lf\n", i, j, mat_i[j]);
						puts("lp internal error");
						exit(0);
					}
				double r = -(mat+m*(m+n+1))[base[i]-1];
				if (r < -EPS || r > EPS) {
					for (int j = 0; j <= m+n; ++j)
						(mat+m*(m+n+1))[j] += mat_i[j] * r;
				}
			}
			solve_slack(mat, base, m, n, -1); // 不作换入换出的指示
		}
		free(help);
	} else {
		solve_slack(mat, base, m, n, -1); // 不作换入换出的指示
	}
	free(base);
	free(mat);
}

int main()
{
	int m, n;
	printf("input m n: ");
	scanf("%d %d", &m, &n);

	double *A = malloc(sizeof(double) * m * n);
	printf("input A[m][n]: ");
	for (int i = 0; i < m*n; ++i)
		scanf("%lf", A+i);

	double *b = malloc(sizeof(double) * m);
	printf("input b[m]: ");
	for (int i = 0; i < m; ++i)
		scanf("%lf", b+i);

	double *c = malloc(sizeof(double) * n);
	printf("input c[n]: ");
	for (int i = 0; i < n; ++i)
		scanf("%lf", c+i);
	/*
	int m = 4, n = 3;
	double A[] = {1, -2, 1, 4, -1, -2, -2, 0, 1, 2, 0, -1};
	double b[] = {11, -3, 1, -1};
	double c[] = {3, -1, -1};
	*/

	simplex(A, b, c, m, n);
	return 0;
}
