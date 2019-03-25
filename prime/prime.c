#include <stdio.h>
#include <stdlib.h>

typedef char bool;
const bool true = 1;
const bool false = 0;

bool flag[10000001];
//int primes[664579]; // pi(10^7) = 664579
int primes[5761455]; // pi(10^8) = 5761455

// i is a prime iff flag[i] == true
// consider odd numbers as primes
void init(int n)
{
	flag[0] = false;
	flag[1] = false;
	flag[2] = true;
	for (int p = 3; p <= n; p += 2)
		flag[p] = true;
}

// Eratoschenes sieve method
// generates primes <= n
// return 0 if generation exhausted
int next_prime(int n)
{
	static int p = 2;
	if (p > n)
		return 0;
	if (p == 2) {
		init(n);
		return p++;
	}

	while (p <= n/p) {
		if (flag[p]) {
			// sieve all composite numbers on factor p
			for (int c = p*p; c <= n; c += p) {
				if (p == 0)
					return 0;
				flag[c] = false;
			}
			return p++;
		}
		++p;
	}

	// return the rest
	while (p <= n) {
		if (flag[p])
			return p++;
		++p;
	}
	return 0;
}

// print primes pairs (p, q) such that q - p = 2
void prime_pairs(int n)
{
	int p = 0, last_p = -1;
	while ((p = next_prime(n))) {
		if (p - last_p == 2)
			printf("%d %d\n", last_p, p);
		last_p = p;
	}
	puts("");
}

void factor(int n) {
	
}

// slower, 3*10^7 ok, 5*10^7 segmentation fault
void print_primes1(int n)
{
	int p;
	while ((p = next_prime(n)))
		printf("%d ", p);
	puts("");
}

// faster, 10^7 ok, 1.1*10^7 segmentation fault
void print_primes2(int n)
{
	int m, cnt = 0;
	for (int i = 2; i <= n; ++i) {
		// i is a prime iff flag[i] == false
		if (!flag[i])
			primes[cnt++] = i;
		for (int j = 0; (m = i * primes[j]) <= n; ++j) {
			flag[m] = true;
			if (i % primes[j] == 0)
				break;
		}
	}
	for (int i = 0; i < cnt; ++i)
		printf("%d ", primes[i]);
	puts("");
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		puts("output primes <= n\n"
			 "usage: prime n 1 (slower but support n=30M)\n"
			 "       prime n 2 (faster but segmentation fault for n=11M)");
		return 0;
	}
	int n = atoi(argv[1]);
	int method = atoi(argv[2]);
	if (method == 1)
		print_primes1(n);
	else
		print_primes2(n);
	return 0;
}
