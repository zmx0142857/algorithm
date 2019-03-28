#include <stdio.h>
typedef char bool;
const bool true = 1;
const bool false = 0;

void print(int *arr, int size)
{
	printf("[%d", arr[0]);
	for (int i = 1; i < size; ++i)
		printf(" %d", arr[i]);
	printf("]\n");
}

bool bsearch(int *arr, int size, int val)
{
	if (size <= 0) {
		printf("arr is empty");
		return false;
	}
	int lo = 0, hi = size-1;
	while (lo < hi) {
		int mid = lo + ((hi-lo) >> 1);
		if (val == arr[mid]) {
			printf("%d = arr[%d]", val, mid);
			return true;
		} else if (val < arr[mid])
			hi = mid-1;
		else
			lo = mid+1;
	}
	if (val == arr[lo]) {
		printf("%d = arr[%d]", val, lo);
		return true;
	} else if (val < arr[lo]) {
		if (lo == 0)
			printf("%d < arr[%d]", val, lo);
		else
			printf("arr[%d] < %d < arr[%d]", lo-1, val, lo);
		return false;
	} else {
		if (lo == size-1)
			printf("arr[%d] < %d", lo, val);
		else
			printf("arr[%d] < %d < arr[%d]", lo, val, lo+1);
		return false;
	}
}

int main()
{
	int a[] = {1, 2, 4, 7, 8, 13, 18};
	const int size = sizeof(a)/sizeof(int);

	print(a, size);
	for (int i = 0; i < 20; ++i) {
		bsearch(a, size, i);
		puts("");
	}
	return 0;
}

