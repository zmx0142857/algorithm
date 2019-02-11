#ifndef STACK_H
#define STACK_H

#include <algorithm>

template <class T> class ArrStack;
template <class T> std::ostream &operator<<(std::ostream &, const ArrStack<T> &);

/* stack by array
 * [0, 1, 2, 3, *]
 *              ^
 *             top
 */
template <class T>
class ArrStack {

	friend std::ostream &operator<< <> (std::ostream &, const ArrStack<T> &);

public:
	ArrStack(size_t n=0);				// allocate space for n elements
	ArrStack(const T *arr, size_t n);	// allocate space for 2*n elements
	~ArrStack();

	void clear();
	void push(const T &val);
	T pop();
	const T &get() const;
	T &get();
	size_t size() const;
	bool full() const;
	bool empty() const;
	
protected:
	const size_t capacity;
	size_t top;
	T *stack;

// -DDEBUG
#ifdef DEBUG
public:
	static void test()
	{
		using namespace std;
		int arr[] = {1, 2, 3, 4, 5};
		ArrStack<int> as(arr, 5);
		cout << as << endl;
		for (size_t i = 0; i < 6; i++) {
			cout << as.get() << endl;
			as.pop();
			cout << as << endl;
		}
		for (size_t i = 0; i < 11; i++) {
			as.push(i*i);
			cout << as << endl;
			cout << as.get() << endl;
			cout << "size: " << as.size() << endl;
		}
	}
#endif
};

template <class T>
std::ostream &operator<<(std::ostream& os, const ArrStack<T> &rhs)
{
	os << "[";
	if (!rhs.empty())
		os << rhs.stack[0];
	for (size_t i = 1; i < rhs.size(); ++i)
		os << ", " << rhs.stack[i];
	return os << "] <- top";
}

template <class T>
ArrStack<T>::ArrStack(size_t n):
	capacity(n), top(0), stack(new T[n]) {}

template <class T>
ArrStack<T>::ArrStack(const T *arr, size_t n):
	capacity(n<<1), top(n), stack(new T[n<<1])
{
	// cannot use memcpy() because class T may be pointer-based
	std::copy(arr, arr + n, stack);
}

template <class T>
ArrStack<T>::~ArrStack()
{
	delete[] stack;
}

template <class T>
inline void ArrStack<T>::clear()
{
	top = 0;
}

template <class T>
void ArrStack<T>::push(const T &val)
{
	if (full()) {
		std::cerr << "push: stack overflow\n";
		return;
	}
	stack[top++] = val;
}

template <class T>
T ArrStack<T>::pop()
{
	if (empty()) {
		std::cerr << "pop: stack underflow\n";
		return T();
	}
	return stack[--top];
}

template <class T>
const T &ArrStack<T>::get() const
{
	if (empty()) {
		std::cout << "get: stack is empty\n";
		T *p = new T;
		return *p;		// but the memory leaks...
	}
	return stack[top-1];
}

template <class T>
T &ArrStack<T>::get()
{
	const ArrStack<T> &const_this = *this;
	return const_cast<T&> ( const_this.get() );
}

template <class T>
inline size_t ArrStack<T>::size() const
{
	return top;
}

template <class T>
inline bool ArrStack<T>::full() const
{
	return top == capacity;
}

template <class T>
inline bool ArrStack<T>::empty() const
{
	return top == 0;
}

#endif // STACK_H
