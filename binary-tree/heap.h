#ifndef HEAP_H
#define HEAP_H

#include <algorithm>

template <class T> class Heap;
template <class T> std::ostream &operator<<(std::ostream &os, const Heap<T> &rhs);

template <class T>
class Heap {

	friend std::ostream &operator<< <>(std::ostream &os, const Heap<T> &rhs);

public:
	static bool less_than(const T &lhs, const T &rhs) { return lhs < rhs; }
	static bool more_than(const T &lhs, const T &rhs) { return lhs > rhs; }

	Heap(size_t n, bool comp(const T &, const T &)=less_than);
	Heap(T *arr, size_t n, bool comp(const T &, const T &)=less_than);
	~Heap();

	bool isleaf(size_t i) const;
	static size_t left(size_t i);
	static size_t right(size_t i);
	static size_t parent(size_t i); // parent of root is root itself

	void push(const T &val);		// append to the end then siftup
	T pop();
	T get() const;
	void erase(size_t i);
	void siftup(size_t i);
	void siftdown(size_t i);

protected:
	size_t capacity;
	size_t size;
	T *data;
	bool (*compare)(const T &, const T &);

private:
	void construct();

#ifdef DEBUG
public:
	static void test()
	{
		using namespace std;

		int arr[] = {6, 5, 4, 3, 2, 1, 0};
		Heap<int> h1(arr, sizeof(arr)/sizeof(int));
		cout << h1 << endl;	// [0, 2, 1, 3, 5, 6, 4]

		Heap<int> h2(10);
		cout << h2 << endl; // []
		for (int i = 10; i > -1; --i) {
			h2.push(i);
			cout << h2 << endl;
		}
		for (int i = 10; i > -1; --i) {
			h2.pop();
			cout << h2 << endl;
		}
		Heap<int> h3(10, Heap<T>::more_than);
		cout << h3 << endl; // []
		for (int i = 0; i < 11; ++i) {
			h3.push(i);
			cout << h3 << endl;
		}
		for (int i = 0; i < 11; ++i) {
			h3.pop();
			cout << h3 << endl;
		}
	}

	static void test_string()
	{
		using namespace std;
		string arr[] = {"hello", "I", "am", "zmx"};
		Heap<string> h3(arr, 4, Heap<T>::more_than);
		cout << h3 << endl;
		for (size_t i = 0; i != 4; ++i)
			cout << arr[i] << ' ';
		cout << endl;
	}
#endif
};

template <class T>
std::ostream &operator<<(std::ostream &os, const Heap<T> &rhs)
{
	os << '[';
	if (rhs.size > 0) {
		os << rhs.data[0];
		for (size_t i = 1; i != rhs.size; ++i)
			os << ", " << rhs.data[i];
	}
	return os << ']';
}

template <class T>
Heap<T>::Heap(size_t n, bool comp(const T &, const T &)):
	capacity(n), size(0), data(new T[capacity]), compare(comp) {}

template <class T>
Heap<T>::Heap(T *arr, size_t n, bool comp(const T &, const T &)):
	capacity(n << 1), size(n), data(new T[capacity]), compare(comp)
{
	std::copy(arr, arr + n, data);
	construct();
}

template <class T>
Heap<T>::~Heap()
{
	delete[] data;
}

template <class T>
bool Heap<T>::isleaf(size_t i) const
{
	return i >= (size >> 1) && i < size;
}

template <class T>
size_t Heap<T>::left(size_t i)
{
	return (i << 1) + 1;
}

template <class T>
size_t Heap<T>::right(size_t i)
{
	return (i + 1) << 1;
}

template <class T>
size_t Heap<T>::parent(size_t i)
{
	return (i - 1) >> 1;
}

template <class T>
void Heap<T>::push(const T &val)
{
	if (size == capacity) {
		std::cout << "push: heap overflow\n";
		return;
	}
	data[size] = val;
	siftup(size++);
}

template <class T>
T Heap<T>::pop()
{
	if (size == 0) {
		std::cout << "pop: heap underflow\n";
		return T();
	}
	T ret = data[0];
	data[0] = data[--size];
	siftdown(0);
	return ret;
}

template <class T>
T Heap<T>::get() const
{
	if (size == 0) {
		std::cout << "get: heap is empty\n";
		return T();
	}
	return data[0];
}

template <class T>
void Heap<T>::erase(size_t i)
{
	if (i >= size) {
		std::cout << "earse: index out of range\n";
		return;
	}
	data[i] = data[--size];
	if (compare(data[i], data[parent(i)]))
		siftup(i);
	else
		siftdown(i);
}

template <class T>
void Heap<T>::siftup(size_t i)
{
	T tmp = data[i];
	while (i > 0 && compare(tmp, data[parent(i)])) {
		data[i] = data[parent(i)];
		i = parent(i);
	}
	data[i] = tmp;
}

template <class T>
void Heap<T>::siftdown(size_t i)
{
	T tmp = data[i];
	size_t j = left(i);
	while (j < size) {
		if (j+1 < size && compare(data[j+1], data[j]))
			++j;
		if (compare(data[j], tmp)) {
			data[i] = data[j];
			i = j;
			j = left(j);
		} else {
			break;
		}
	}
	data[i] = tmp;
}

template <class T>
void Heap<T>::construct()
{
	// size_t(-1) probably equals to 4294967295 (2^32-1)
	for (size_t i = (size >> 1) - 1; i != size_t(-1); --i)
		siftdown(i);
}

#endif // HEAP_H
