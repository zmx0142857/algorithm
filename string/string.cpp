#include <iostream>
using namespace std;

int n, m;

// O(m * n)
// suppose p != ''
void naive_match(const string &text, const string &pattern)
{
	int i = 0, j = 0;
	while (i < n) {
		if (text[i] != pattern[j]) {
			i -= j-1;
			j = 0;
		} else if (j == m-1) {
			cout << ' ' << i-j;
			i -= j-1;
			j = 0;
		} else {
			++i, ++j;
		}
	}
}

/*
// Intro to Algotirhms' version
// suppose p != ''
// suppose pi has enough space m
void compute_prefix_function(const string &p, int *pi)
{
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

// O(m + n)
// suppose p != ''
void kmp_match(const string &text, const string &pattern)
{
	int *pi = new int[m];
	compute_prefix_function(pattern, pi);
	int q = -1;
	for (int i = 0; i != n; ++i) {
		while (q != -1 && pattern[q+1] != text[i])
			q = pi[q];
		if (pattern[q+1] == text[i])
			++q;
		if (q == m-1) {
			cout << ' ' << i-q;
			q = pi[q];
		}
	}
	delete[] pi;
}
*/

// yan wei ming's version
// suppose p != ''
// suppose pi has enough space m
void compute_prefix_function(const string &p, int *pi)
{
	int j = 0, k = -1;
	pi[j] = k;
	while (j < m-1) {
		while (k != -1 && p[j] != p[k]) {
			k = pi[k];
		}
		pi[++j] = ++k;
		/*
		if (p[++j] == p[++k]) {
			pi[j] = pi[k];
		} else {
			pi[j] = k;
		}
		*/
	}
}
// O(m + n)
// suppose p != ''
void kmp_match(const string &text, const string &pattern)
{
	int *pi = new int[m];
	compute_prefix_function(pattern, pi);
	int i = 0, j = 0;
	while (i < n) {
		while (j != -1 and text[i] != pattern[j]) {
			j = pi[j];
		}
		if (j == m-1) {
			cout << ' ' << i-j;
			j = pi[j];
		} else {
			++i, ++j;
		}
	}
}

int main()
{
	string t, p;
	while (cin >> t >> p) {
		n = t.length();
		m = p.length();
		if (m > 0) {
			cout << "pattern occurs in";
			//naive_match(t, p);
			kmp_match(t, p);
			cout << '\n';
		}
	}
	return 0;
}
