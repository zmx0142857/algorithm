#include <stdio.h>
#include <string.h> // memcpy(void *dest, void *src, unsigned cnt)
#include <stdlib.h> // atoi(char*) malloc() free()

void help();						// infomation for user
void swap(int*, int*);				// swap two int
void print(int[], int size);		// print the array

void bubble_sort(int[], int size);
void insert_sort(int[], int size);
void select_sort(int[], int size);
void shell_sort(int[], int size);

void merge_sort(int[], int size);
void do_merge_sort(int[], int[], int beg, int end);		
void merge(int[], int [], int beg, int mid, int end); 

void quick_sort(int[], int size);
void do_quick_sort(int [], int beg, int end);

void heap_sort(int[], int size);
void heapify(int[], int i, int size);

int a[100000];

int main(int argc, char **argv)
{
	if ((argc != 2 && argc != 3) || strlen(argv[1]) != 2)
	{
		help();
		return 1;
	}

	int size = 0;
	if (argc == 2) // read stdin
	{
		while (scanf("%d", a+size) == 1)
			++size;
	}
	if (argc == 3) // read file
	{
		FILE *fin = fopen(argv[2], "r");
		if (!fin)
		{
			printf("file does not exist\n");
			return 2;
		}
		while ( fscanf(fin, "%d", a+size) == 1)
			++size;
	}

	switch (argv[1][1])
	{
		case 'b':	bubble_sort(a, size); break;
		case 'h':	heap_sort(a, size); break;
		case 'i':	insert_sort(a, size); break;
		case 'm':	merge_sort(a, size); break;
		case 'q':	quick_sort(a, size); break;
		case 's':	select_sort(a, size); break;
		case 'S':	shell_sort(a, size); break;
		default:	help();	return 0;
	}
	
	print(a, size);
	return 0;
}

void help()
{
	printf(	"usage: sort <option> <numbers>\n"
			"-b   bubble sort\n"
			"-h   heap sort\n"
			"-i   insertion sort\n"
			"-m   merge sort\n"
			"-q   quick sort\n"
			"-s   select sort\n"
			"-S   Shell sort\n"
	);
}

void swap(int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

void print(int a[], int size)
{
	int i;
	for (i = 0; i < size; ++i)
		printf("%d ", a[i]);
	printf("\n");
}

void bubble_sort(int a[], int size)
{
    int i, j;
    for (i = 1; i < size; ++i)
        for (j = 0; j < size-i; ++j)
            if (a[j] > a[j+1])
				swap(a+j, a+j+1);
}

void insert_sort(int a[], int size)
{
	int i, j, key;
	for (i = 1; i < size; ++i)
	{
		key = a[i];
		// insert a[i] to the sorted sequence a[0:i)
		for (j = i; j > 0 && a[j-1] > key; --j)
			a[j] = a[j-1];
		a[j] = key;
	}
}

void select_sort(int a[], int size)
{
    int i, j, min;
	for (i = 0; i < size; ++i)
    {
        min = i; 
		for (j = i + 1; j < size; ++j)
		{
            if (a[j] < a[min])
                min = j;
		}
		// min stores the index of minmum of a[i:size)
		swap(a + min, a + i);
	}
}

// this algorithm is a little like insertion sort;
// in fact insertion sort is when gap=1
void shell_sort(int a[], int size)
{
    int i, j, gap, key;
    for (gap = size/2; gap > 0; gap /= 2)
        for (i = gap; i < size; ++i)
		{
			key = a[i];
			for (j = i; j >= gap && a[j - gap] > key; j -= gap)
				a[j] = a[j - gap];
			a[j] = key;
		}
}

void merge_sort(int a[], int size)
{
	int *b = (int*)malloc(size * sizeof(int));
	memcpy(b, a, size * sizeof(int));
	do_merge_sort(b, a, 0, size);
	free(b);
}

// time cost is O(n lg n)
void do_merge_sort(int a[], int b[], int beg, int end)
{
	if (beg + 1 < end) // [beg:end) has at least two elems
	{
		int mid = (beg + end) / 2;
		do_merge_sort(b, a, beg, mid); // sort [n/2] elems
		do_merge_sort(b, a, mid, end); // sort [n/2]+1 elems
		merge(a, b, beg, mid, end);
	}
}

// merge a[beg:mid) and a[mid:end) into b[beg:end)
// time cost is O(n)
void merge(int a[], int b[], int beg, int mid, int end)
{
	int i = beg, j = mid, k = beg;
	while (i < mid && j < end)
	{
		if (a[i] <= a[j])
			b[k++] = a[i++];
		else
			b[k++] = a[j++];
	}
	while (i < mid)
		b[k++] = a[i++];
	while (j < end)
		b[k++] = a[j++];
}

void quick_sort(int a[], int size)
{
	do_quick_sort(a, 0, size);
}

void do_quick_sort(int a[], int beg, int end)
{
	if (beg + 1 >= end) return;
	int pivot = a[beg]; 
	int i = beg, j = end;
	while (1)
	{
		// in each iteration,
		// elements in range[beg+1,i) <= pivot
		// elements in range (j, end) >= pivot
		while (i+1 < end && a[++i] < pivot) {}
		while (j > beg && a[--j] > pivot) {}
		if (i < j)
			swap(a+i, a+j);
		else
			break;
	}
	swap(a+j, a+beg);
	// invariant: a[j] <= a[i] <= a[k] for beg <= j < i < k < end
	do_quick_sort(a, beg, j);
	do_quick_sort(a, j+1, end);
}

void heap_sort(int a[], int size)
{
	for (int i = size/2 - 1; i >= 0; --i)
	{
		heapify(a, i, size);
	}
	for (int i = size-1; i > 0; --i)
	{
		swap(a, a+i);
		heapify(a, 0, i);
	}
}

// make the subtree rooted at i a max-heap
void heapify(int a[], int i, int size)
{
	int top = a[i];
	while (i < size/2)		// i has child
	{
		int j = 2*i + 1;	// left child
		// if right child exists and greater than left child, choose it
		if (j+1 < size && a[j] < a[j+1]) ++j;
		if (a[j] <= top) break;
		a[i] = a[j];
		i = j;
	}
	a[i] = top;
}
