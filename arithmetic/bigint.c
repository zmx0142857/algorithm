#include <stdio.h>
#include <string.h> // memcpy
#include <stdint.h> // uint32_t

typedef uint32_t digit_t;
typedef uint64_t two_digits_t;
const unsigned bitsN = 31;
const digit_t N = 1 << bitsN; // 保证两个小于 N 的数相加不溢出
const digit_t maskN = N - 1;
typedef struct {
    digit_t *data;
    int size;
} bigint;

// 0N 到 9N 除以 10 的结果
const digit_t qN[10] = {0, 214748364, 429496729, 644245094, 858993459, 1073741824, 1288490188, 1503238553, 1717986918, 1932735283};
const digit_t rN[10] = {0, 8, 6, 4, 2, 0, 8, 6, 4, 2};

int stack[10000];

void bigint_print(bigint *num)
{
    digit_t cpy[num->size];
    memcpy(cpy, num->data, num->size * sizeof(digit_t));
    int top = 0, begin = num->size-1;
	while (begin >= 0) {
		digit_t r = 0;
		for (int i = begin; i >= 0; --i) {
			digit_t tmp = rN[r] + cpy[i];
			cpy[i] = qN[r] + tmp / 10; // 商保存在原地
			r = tmp % 10; // 余数
		}
		stack[top++] = r;
		while (begin >= 0 && cpy[begin] == 0)
			--begin;
	}
	while (top)
		printf("%u", stack[--top]);
}

// lhs += rhs, 假设 lhs 有足够空间
void bigint_add(bigint *lhs, bigint *rhs)
{
    digit_t carry = 0; // 进位数等于 0 或 1
    for (int i = 0; i < lhs->size; ++i) {
        carry += lhs->data[i] + rhs->data[i]; // (N-1)+(N-1)+1 不会溢出
        lhs->data[i] = carry & maskN; // 相当于 % N
        carry >>= bitsN; // 相当于 / N
    }
}

void test_bigint_print()
{
    // 低位在先
    // x = 0
    // for i in range(8, 0, -1):
    // ...   x = x * N + i
    // x ==
    // 1684996667383472989748638208589148922131258297907555764671493439489
    digit_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size = sizeof(arr) / sizeof(digit_t);
    bigint num = {arr, size};
	bigint_print(&num);
	putchar('\n');
}

void test_bigint_add()
{
    // a = 0
    // b = 1
    // for i in range(100):
    // ...   a += b
    // ...   b += a
    // b == 453973694165307953197296969697410619233826
    digit_t arr[8] = {0}, brr[8] = {1};
    bigint a = {arr, 8};
    bigint b = {brr, 8};
    int n = 100;
    while (n--) {
        bigint_add(&a, &b);
        bigint_add(&b, &a);
    }
    bigint_print(&b);
    putchar('\n');
}

int main()
{
    //test_bigint_print();
    test_bigint_add();
	return 0;
}
