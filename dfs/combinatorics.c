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

// Helper function for permutation(s), please call permutation(s)
void old_do_permutation(int *next, char *stack)
{
	int i = 0;
	static int n = 0;
	if (next[i] == 0)
		printf("%s ", stack);
	else
		for (i = 0; next[i] != 0; i = next[i])
		{
			char tmp = next[i];
			next[i] = next[next[i]];		// erase(next[i])
			stack[n++] = tmp;				// stack.push(tmp)
			old_do_permutation(next, stack);
			next[i] = tmp;					// insert(next[i])
		}
	--n;									// stack.pop()
}

/* Permutation for string, input 'abc' and you get:
 *     abc acb bac bca cab cba
 * each char must not appear more than once!
 */
void old_permutation(char *str)
{
	int i = 0, cnt = 0;
	char *p = str;
	int next[256] = {0};			// use forward_list: next[0] is first elem
	char stack[256] = {0};			// ... and a stack
	for (i = 0; ; i = next[i])		// copy the chars to the list
	{
		if (next[i] != 0)			// check if the chars repeat
		{
			printf("Error In function permutation(): '%c' "
				   "occurs more than once\n", i);
			printf("\t%s\n", str);
			printf("\t");
			for (i = 0; i < cnt-1; ++i)
				printf(" ");
			printf("^\n");
			return;
		}
		if (*p == '\0')			// check if the str ends
			break;
		next[i] = *p++;
		++cnt;
	}
	old_do_permutation(next, stack);
	printf("\n");
}

int main()
{
	test_combination();
	return 0;
}
