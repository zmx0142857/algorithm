#include <iostream>
#include <cmath>
using namespace std;

// calculate finonacci number f_n
// f_0 = 0
// f_1 = 1
// f_n = f_(n-1) + f_(n-2)
// both functions overflow at n = 47
int fibo(int n)
{
	int a = 0;
	int b = 1;
	for (int i = 0; i < n-1; i += 2) {
		a += b;
		b += a;
	}
	return (n & 1) ? b : a;
}

int fibonacci(int n)
{
	if (n == 0)
		return 0;
	static const double sqrt5 = sqrt(5);
	static const double phi = 0.5*(1 + sqrt5);
	return round(pow(phi, n) / sqrt5);
}

int main()
{
	for (int n = 0; n < 100; ++n) {
		int f1 = fibo(n);
		int f2 = fibonacci(n);
		if (f1 != f2)
			cout << "n = " << n
				 << " f1 = " << f1
				 << " f2 = " << f2 << endl;
	}
	return 0;
}
