#include <iostream>
#include <limits>
#include <cctype>
#include <cstdlib>

using namespace std;

void shuffle(string &s)
{
	string scopy(s.length(), '#');
	size_t i = 0, j = 0;
	while (i < s.length()) {
		scopy[j] = s[i];
		scopy[s.length()-1-j] = s[i+1];
		i += 2;
		j += 1;
	}
	s = scopy;
}

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

bool are_cyclic_permutations(const string &s, const string &t)
{
	int n = s.length();
	if (int(t.length()) != n)
		return false;
	else if (n == 0)
		return true;
	int *pi = new int[n];
	compute_prefix_function(t, pi);
	int q = -1;
	for (int i = 0; i < (2*n - 1); i++) {
		while (q != -1 && t[q+1] != s[i%n])
			q = pi[q];
		if (t[q+1] == s[i%n])
			++q;
		if (q == n-1) {
			//cout << "pattern occurs in " << i-n+1 << endl;
			//q = pi[q];
			delete[] pi;
			return true;
		}
	}
	delete[] pi;
	return false;
}

// argument to int
// convert string to int (range INT_MIN - INT_MAX)
// usually, INT_MIN = -2147483648, INT_MAX = 2147483647
int my_atoi(string s)
{
	int ret = 0;
	size_t i = 0;
	bool negative = false;
	// empty string returns 0
	if (s.empty())
		return 0;
	// eat spaces
	for (; i < s.size() && isspace(s[i]); i++) {}
	// check the sign
	if (s[i] == '+') {
		++i;
	} else if (s[i] == '-') {
		++i;
		negative = true;
	}
	// read the digits
	for (; i < s.size() && isdigit(s[i]); i++) {
		int diff = s[i] - '0';
		// I don't know why this wrok!!!
		//if (ret > (INT_MAX - diff)/10)
			//return negative ? INT_MIN : INT_MAX;
		ret = 10 * ret + diff;
	}
	return negative ? -ret : ret;
}

size_t find_digit(size_t i, const string &s)
{
	while (i < s.length() && !isdigit(s[i]))
		++i;
	cout << s[i];
	return i;
}

size_t find_non_digit(size_t i, const string &s)
{
	while (i < s.length() && isdigit(s[i]))
		cout << s[i++];
	cout << ' ';
	return i;
}

int count_int(string &s)
{
	int count = 0;
	size_t i = find_digit(0, s);
	while (i < s.length()) {
		i = find_non_digit(i+1, s);
		++count;
		if (i < s.length())
			i = find_digit(i+1, s);
		else
			break;
	}
	cout << '\n';
	return count;
}

// O(n^2)
size_t longest_repeat_substr(const string &s, int &len)
{
	size_t n = s.length();
	if (n == 0) {
		len = 0;
		return -1;
	}
	size_t index = 0;
	len = 1;
	int *pi = new int[n];
	for (size_t i = 0; i < n; i++) {
		compute_prefix_function(s.substr(i,n-i), pi);
		int temp = pi[0];
		for (size_t j = 1; j < n; j++) {
			if (pi[j] > temp)
				temp = pi[j];
		}
		temp += 1;
		if (temp > len) {
			index = i;
			len = temp;
		}
	}
	delete[] pi;
	return index;
}

// 以下为测试函数
void test_shuffle()
{
	string s("abcdefghijkl");
	shuffle(s);
	cout << s << endl;
}

void test_my_atoi()
{
	string str;
	while (cin >> str) {
		cout << "my_atoi: " << my_atoi(str) << endl;
		cout << "atoi:    " << atoi(str.c_str()) << endl;
	}
}

void test_count_int()
{
	string str;
	while (cin >> str)
		cout << count_int(str) << endl;
}

void test_are_cyclic_permutations()
{
	string s, t;
	while (cin >> s >> t) {
		cout << "are cyclic permutations: " 
			 << (are_cyclic_permutations(s, t)?"true":"false") << endl;
	}
}

void test_longest_repeat_substr()
{
	string str;
	int len;
	size_t index;
	while (cin >> str) {
		index = longest_repeat_substr(str, len);
		cout << "substr: " << str.substr(index, len) << endl
			 << "index: " << index << endl;
	}
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		cout << "usage: ch4 [option]\n"
		"-s\ttest_shuffle\n"
		"-a\ttest_atoi\n"
		"-i\ttest_count_int\n"
		"-c\ttest_are_cyclic_permutations\n"
		"-r\ttest_longest_repeat_substr\n";
	} else if (argc == 2) {
		switch (argv[1][1]) {
			case 's': test_shuffle(); break;
			case 'a': test_my_atoi(); break;
			case 'i': test_count_int(); break;
			case 'c': test_are_cyclic_permutations(); break;
			case 'r': test_longest_repeat_substr(); break;
			default: break;
		}
	} else {
		cout << "too many arguments\n";
	}
	return 0;
}

