#include <stdio.h>

typedef char bool;
const bool true = 1;
const bool false = 0;

int n;
bool is_prime[100+10];

// init boolean array
void init_is_prime() {
	is_prime[0] = false;
	is_prime[1] = false;
	is_prime[2] = true;
	// consider odd numbers as primes
	for (int p = 3; p <= n; p += 2)
		is_prime[p] = true;
}

// Eratoschenes sieve method
// generates primes <= n
// return 0 if generation exhausted
int next_prime()
{
	static int p = 2;
	if (p > n) return 0;
	if (p == 2) return p++;

	int c;
	while ((c = p*p) <= n) {
		if (is_prime[p]) {
			// sieve all composite numbers on factor p
			for (; c <= n; c += p) {
				if (p == 0)
					return 0;
				is_prime[c] = false;
			}
			return p++;
		}
		++p;
	}

	// return the rest
	while (p <= n) {
		if (is_prime[p])
			return p++;
		++p;
	}
	return 0;
}

// print primes pairs (p, q) such that q - p = 2
void prime_pairs()
{
	int p = 0, last_p = -1;
	scanf("%d", &n);
	init_is_prime();
	while ((p = next_prime())) {
		if (p - last_p == 2)
			printf("%d %d\n", last_p, p);
		last_p = p;
	}
	puts("");
}

void factor(int n) {
	
}

int main()
{
	return 0;
}
