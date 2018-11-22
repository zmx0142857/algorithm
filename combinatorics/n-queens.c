#include <stdio.h>
#include <stdlib.h>

char verbose = 1;

void swap(int *p, int *q)
{
	int tmp = *p;
	*p = *q;
	*q = tmp;
}

char is_valid(int *s, int beg)
{
	for (int i = 0; i < beg; ++i) {
		// on conflict returns false
		if (s[i] == s[beg] || beg+s[i] == s[beg]+i || beg+s[beg] == i+s[i])
			return 0;
	}
	return 1;
}

void output(int *s, int n)
{
	if (verbose) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				printf(j == s[i] ? "Q " : ". ");
			}
			puts("");
		}	
	} else {
		for (int i = 0; i < n; ++i)
			printf("%d ", s[i]);
	}
	puts("");
}

void permutate(int *s, int beg, int end)
{
	if (beg < end) {
		for (int i = beg; i < end; ++i) {
			swap(s+i, s+beg);
			if (is_valid(s, beg))
				permutate(s, beg+1, end);
			swap(s+i, s+beg);
		}
	} else {
		output(s, end);
	}
}

void queens(int n)
{
	if (n < 1 || n > 9) {
		puts("n must in range 1...9");
		return;
	}

	// init solution
	// (i, solution[i]), i = 0 ... n-1
	// are the positions of the queens
	int *solution = (int*)malloc(n*sizeof(int));
	for (int i = 0; i < n; ++i)
		solution[i] = i;
	permutate(solution, 0, n);
}

int main()
{
	int n;
	printf("n = ");
	while (scanf("%d", &n) == 1) {
		queens(n);
		printf("n = ");
	}
	puts("");
	return 0;
}
