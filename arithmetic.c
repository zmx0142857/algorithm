#include <stdio.h>

// sqrare root for integers
// returns floor(sqrt(x)) if x >= 0
// returns -1 if x < 0
int isqrt(int x)
{
	if (x < 0) return -1;
	if (x == 0) return 0;
	int ret = 1, last;
	do {
		last = ret;
		ret = (ret + x/ret) >> 1;
	} while (ret != last);
	return ret;
}

void test_isqrt()
{
	// ceil(sqrt(2**31-1)) == 46341
	for (int x = 0; x < 46341; ++x) {
		int ret = isqrt(x*x);
		if (x != ret) {
			printf("error: isqrt(%d^2) returns %d\n", x, ret);
			return;
		}
	}
	puts("ok");
}

int main()
{
	test_isqrt();
	return 0;
}
