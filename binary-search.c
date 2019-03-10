#include <stdio.h>
typedef char bool;
const bool true = 1;
const bool false = 0;

bool binary_search(int *arr, int size, int val)
{
	if (size <= 0) {
		printf("arr is empty, ");
		return false;
	}
	int lo = 0, hi = size-1;
	while (lo < hi) {
		int mid = (lo+hi) >> 1;
		if (val > arr[mid])
			lo = mid+1;
		else if (val < arr[mid])
			hi = mid-1;
		else {
			printf("%d == arr[%d], ", val, mid);
			return true;
		}
	}
	
	if (val == arr[lo]) {
		printf("%d == arr[%d], ", val, lo);
		return true;
	} else if (val < arr[lo]) {
		while (val < arr[lo]) {
			if (lo == 0) {
				printf("%d < arr[0], ", val);
				return false;
			}
			--lo;
		}
	} else {
		while (val > arr[lo]) {
			if (lo == size-1) {
				printf("%d > arr[%d], ", val, size-1);
				return false;
			}
			++lo;
		}
		--lo;
	}
	printf("arr[%d] < %d < arr[%d], ", lo, val, lo+1);
	return false;
}

int main()
{
	int a[] = {1, 2, 4, 7, 8, 13, 18};
	const int size = sizeof(a)/sizeof(int);
	for (int i = 0; i < 20; ++i)
		printf("%d\n", binary_search(a, size, i));
	return 0;
}

