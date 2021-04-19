#include <stdio.h>
#include <stdlib.h>

// return 2^k such that 2^k <= n < 2^(k+1)
unsigned highest_bit_mask(unsigned n)
{
	if (n == 0)
		return 0;
	const int shift = sizeof(unsigned)*8-1;
	unsigned mask = 1 << shift;
	while (!(mask & n))
		mask >>= 1;
	return mask;
}

size_t printfb(unsigned n)
{
	printf("0b");
	size_t cnt = 2;
	unsigned mask = highest_bit_mask(n);
	if (!mask) {
		printf("0");
		return 3;
	} else do {
		printf(mask & n ? "1" : "0");
		mask >>= 1;
		++cnt;
	} while (mask);
	return cnt;
}

size_t scanfb(unsigned *p)
{
	char ch = getchar();
	if (ch != '0')
		return 0;
	ch = getchar();
	if (ch != 'b')
		return 0;
	size_t cnt = 2;
	*p = 0;
	while ((ch = getchar())) {
		if (ch == '0') {
			*p <<= 1;
		} else if (ch == '1') {
			*p <<= 1;
			*p |= 1;
		} else {
			break;
		}
		++cnt;
	}
	return cnt;
}

// input 0xb 0x5
// return 0x27
//
//   0101
// x 1011
// ------
//    101
//   101
//  000
// 101
// ------
// 100111
unsigned mod2mul(unsigned x, unsigned y)
{
	unsigned ret = 0;
	while (x) {
		if (x & 1)
			ret ^= y;
		x >>= 1;
		y <<= 1;
	}
	return ret;
}

// return remainder
// input 0b101001000 0b1101
// return 0b1
unsigned mod2div(unsigned x, unsigned y)
{
	if (y == 0)
		exit(1);
	/* suppose the dividend is 32-bit by adding zeros to the left
	 * align the highest bit of divisor with that of unsigned type
	 * then the highest bit of divisor and dividend are aligned.
	 * for example, x = 101001000, y = 1101 are alighed like:
	 *
	 * 00000000000000000000000101001000
	 * 1101
	 */
	const int shift = sizeof(unsigned)*8-1;
	unsigned mask = 1 << shift;
	size_t cnt = 1;
	while (!(y & mask)) {
		y <<= 1;
		++cnt;
	}
	while (cnt--) {
		if (x & mask)
			x ^= y;
		mask >>= 1;
		y >>= 1;
	}
	return x;
}

void test_io()
{
	unsigned x;
	while (scanfb(&x)) {
		printfb(x);
		puts("");
	}
}

void test_mul()
{
	unsigned x, y;
	while (scanfb(&x) && scanfb(&y)) {
		printfb(mod2mul(x, y));
		puts("");
	}
}

void test_div()
{
	unsigned x, y;
	while (scanfb(&x) && scanfb(&y)) {
		printfb(mod2div(x, y));
		puts("");
	}
}

void help()
{
	printf(	"usage: mod2 [option]\n"
			"-i   test_io\n"
			"-m   test_mul\n"
			"-d   test_div\n"
	);
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		help();

	switch (argv[1][1]) {
		case 'i':	test_io();	break;
		case 'm':	test_mul();	break;
		case 'd':	test_div();	break;
		default:	help();
	}
	return 0;
}
