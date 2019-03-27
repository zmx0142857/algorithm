#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pascal_triangle(int n)
{
	if (n < 1)
		return;
	// int arr[n] = {1};
	int *arr = malloc(n*sizeof(int));
	memset(arr, 0, n*sizeof(int));
	arr[0] = 1;
	for (int i = 0; i < n; ++i) {
		int j = 0;
		for (; j <= i; ++j)
			printf("%d ", arr[j]);
		puts("");
		for (; j > 0; --j)
			arr[j] += arr[j-1];
	}
	free(arr);
}

int main()
{
	int n;
	while (scanf("%d", &n) == 1)
		pascal_triangle(n);
}
