#include <iostream>
using namespace std;

// suppose p != ''
// suppose pi has enough space m
void compute_prefix_function(const string &p, int *pi)
{
	int m = p.length();
	pi[0] = -1;
	int k = -1;
	for (int q = 1; q != m; ++q) {
		while (k != -1 && p[k+1] != p[q])
			k = pi[k];
		if (p[k+1] == p[q])
			++k;
		pi[q] = k;
	}
}

// kmp matcher O(m + n)
void kmp_matcher(const string &text, const string &pattern)
{
	cout << "pattern occurs in";
	int n = text.length();
	int m = pattern.length();
	if (m > 0) {
		int *pi = new int[m];
		compute_prefix_function(pattern, pi);
		int q = -1;
		for (int i = 0; i != n; ++i) {
			while (q != -1 && pattern[q+1] != text[i])
				q = pi[q];
			if (pattern[q+1] == text[i])
				++q;
			if (q == m-1) {
				cout << ' ' << i-m+1;
				q = pi[q];
			}
		}
		delete[] pi;
	}
	cout << '\n';
}
int main()
{
	string t, p;
	while (cin >> t >> p) {
		kmp_matcher(t, p);
	}
	return 0;
}
