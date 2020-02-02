#include <stdio.h>

int left[8] = {0, 2, 0, 0, 0, 0, 0, 0};
int right[8] = {0, 3, 4, 5, 6, 7, 0, 0};
char str[8];

void huffman_code(int root, int lv)
{
	if (root) {
		str[lv] = '0';
		huffman_code(left[root], lv+1);
		str[lv] = '1';
		huffman_code(right[root], lv+1);
	} else {
		str[lv] = '\0';
		puts(str);
	}
}

/* result
00
010
0110
0111
10
110
1110
1111
*/
int main()
{
	huffman_code(1, 0);
	return 0;
}

