#include <iostream>

using namespace std;


int main()
{
	return 0;
}

/*
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
*/