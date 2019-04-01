#include <iostream>
#include "array.h"
using namespace std;

int main()
{
	int arr[] = {1, 2, 3, 4, 5};
	size_t size = sizeof(arr)/sizeof(int);
	Array<int> a(arr, size);
	cout << a << endl;
	return 0;
}
