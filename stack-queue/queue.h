#ifndef QUEUE_H
#define QUEUE_H

#include <algorithm>

template <class T> class ArrQueue;
template <class T> std::ostream &operator<<(std::ostream &, const ArrQueue<T> &);

template <class T>
class ArrQueue {

	friend std::ostream &operator<< <>(std::ostream &, const ArrQueue<T> &);

public:
	ArrQueue(size_t n);				// allocate space for n+1 elements
	ArrQueue(T *arr, size_t n);		// allocate space for 2*n+1 elements
	~ArrQueue();

	void clear();
	void enqueue(const T &val);
	T dequeue();
	const T &get() const;
	T &get();
	size_t size() const;
	bool full() const;
	bool empty() const;

protected:
	const size_t capacity;
	size_t front;
	size_t rear;
	T *queue;

private:
	size_t next(size_t i) const { return (i + 1) % capacity; }

#ifdef DEBUG
public:
	static void test()
	{
		using namespace std;
		int arr[] = {0, 1, 2, 3, 4, 5};
		ArrQueue<int> aq(arr, sizeof(arr)/sizeof(int));
		cout << aq << endl;
		for (size_t i = 0; i < 7; ++i) {
			aq.dequeue();
			cout << aq
				 << " size: " << aq.size()
				 << " get: " << aq.get() << endl;
		}
		for (size_t i = 0; i < 13; ++i) {
			aq.enqueue(i*i);
			cout << aq
				 << " size: " << aq.size()
				 << " get: " << aq.get() << endl;
		}
	}
#endif
};

template <class T>
std::ostream &operator<<(std::ostream &os, const ArrQueue<T> &rhs)
{
	os << "<- [";
	if (!rhs.empty()) {
		os << rhs.queue[rhs.front];
		for (size_t i = rhs.next(rhs.front);
			i != rhs.rear; i = rhs.next(i))
			os << ", " << rhs.queue[i];
	}
	return os << "] <-";
}

template <class T>
ArrQueue<T>::ArrQueue(size_t n):
	capacity(n+1), front(0), rear(0),
	queue(new T[capacity]) {}

template <class T>
ArrQueue<T>::ArrQueue(T *arr, size_t n):
	capacity((n<<1)+1), front(0),
	rear(n), queue(new T[capacity])
{
	std::copy(arr, arr+n, queue);
}

template <class T>
ArrQueue<T>::~ArrQueue()
{
	delete[] queue;
}

template <class T>
inline void ArrQueue<T>::clear()
{
	rear = front = 0;
}

template <class T>
void ArrQueue<T>::enqueue(const T &val)
{
	if (full()) {
		std::cerr << "enqueue: queue overflow\n";
		return;
	}
	queue[rear] = val;
	rear = next(rear);
}

template <class T>
T ArrQueue<T>::dequeue()
{
	if (empty()) {
		std::cerr << "dequeue: queue underflow\n";
		return T();
	}
	size_t temp = front;
	front = next(front);
	return queue[temp];
}

template <class T>
const T &ArrQueue<T>::get() const
{
	if (empty()) {
		std::cerr << "get: queue is empty\n";
		T *p = new T;
		return *p;		// but the memory leaks...
	}
	return queue[front];
}

template <class T>
T &ArrQueue<T>::get()
{
	const ArrQueue &const_this = *this;
	return const_cast<T&> ( const_this.get() );
}

template <class T>
size_t ArrQueue<T>::size() const
{
	if (front <= rear)
		return rear - front;
	return rear + capacity - front;
}

template <class T>
inline bool ArrQueue<T>::full() const
{
	return next(rear) == front;
}

template <class T>
inline bool ArrQueue<T>::empty() const
{
	return front == rear;
}

#endif // QUEUE_H
