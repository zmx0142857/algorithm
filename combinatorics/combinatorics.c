#include <stdio.h>
#include <stdlib.h>

void swap(char *p, char *q)
{
	char tmp = *p;
	*p = *q;
	*q = tmp;
}

void do_permutation(char *r, char *beg, char *end)
{
	if (beg < end) {
		for (char *i = beg; i < end; ++i) {
			swap(i, beg);
			do_permutation(r, beg+1, end);
			swap(i, beg);
		}
	} else {
		puts(r);
	}
}

void permutation(char *s, int n)
{
	do_permutation(s, s, s+n);
}

void test_permutation()
{
	char s[] = "abcd";
	permutation(s, sizeof(s)-1);
	printf("s did not changed: %s\n", s);
}

void do_combination(char *r, char *beg, char *end, int m, char *j)
{
	if (m > 0) {
		for (char *i = beg; i <= end-m; ++i) {
			*j = *i;
			do_combination(r, i+1, end, m-1, j+1);
		}
	} else {
		puts(r);
	}
}

void combination(char *s, int n, int m)
{
	char *r = (char*)malloc(m+1);
	r[m] = '\0';
	do_combination(r, s, s+n, m, r);
	free(r);
}

void test_combination()
{
	char s2[] = "abcde";
	for (int i = 0; i <= 5; ++i) {
		combination(s2, sizeof(s2)-1, i);
		puts("");
	}
}

int main()
{
	return 0;
}
