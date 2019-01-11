#ifndef LINK_LIST_H
#define LINK_LIST_H

template <class T>
struct LinkListNode {
	T val;
	LinkListNode *next;
	LinkListNode(): val(), next(NULL) {}
	LinkListNode(T d, LinkListNode *n=NULL): val(d), next(n) {}
};

template <class T>
class LinkList;

template <class T>
std::ostream &operator<<(std::ostream &, const LinkList<T> &);

/* link list with no dummy head node
 * [0] -> [1] -> [2] -> NULL
 */
template <class T>
class LinkList {

	friend std::ostream &operator<< <T> (
			std::ostream &, const LinkList<T> &);

public:
	LinkList();
	LinkList(T *arr, size_t size);
	LinkList(const LinkList &);
	LinkList &operator=(const LinkList);
	~LinkList();
	
	bool empty() const;
	void clear();
	size_t size() const;
	void push_front(const T &val);
	void pop_front();
	// these two cannot change head
	void insert_after(LinkListNode<T> *p, const T &value);
	void erase_after(LinkListNode<T> *p);
	LinkListNode<T> *get(size_t i) const;

	// 将单链表逆置
	void reverse();
	// 得到链表倒数第 i 个元素；i == 0 时返回最后一个元素
	LinkListNode<T> *get_last(size_t i) const;

protected:
	LinkListNode<T> *head;
	size_t _size;

// -DDEBUG (must be public)
#ifdef DEBUG
public:
	static void test()
	{
		using namespace std;

		LinkList<int> list1;
		cout << "list1: " << list1 << endl;

		int arr[] = {0, 1, 2, 3, 4};
		LinkList<int> list2(arr, sizeof(arr)/sizeof(int));
		cout << "list2: " << list2 << endl;

		for (size_t i = 0; i != 6; ++i) {
			cout << "list2.get_last(" << i << "): ";
			LinkListNode<T> *p = list2.get_last(i);
			if (p)
				cout << p->val;
			else
				cout << "NULL";
			cout << endl;
		}
		list2.reverse();
		cout << "list2 reversed: " << list2 << endl;
	}
#endif
};

template <class T>
std::ostream &operator<<(std::ostream &os, const LinkList<T> &rhs)
{
	LinkListNode<T> *p = rhs.head;
	os << '[';
	if (p) {
		os << p->val;
		p = p->next;
		while (p) {
			os << ", " << p->val;
			p = p->next;
		}
	}
	return os << ']';
}

template <class T>
LinkList<T>::LinkList(): head(NULL), _size(0) {}

template <class T>
LinkList<T>::LinkList(T *arr, size_t s): head(NULL), _size(0)
{
	if (s > 0) {
		head = new LinkListNode<T>(arr[--s]);	// last element as head
		++_size;
		while (s--)							// _size-1 elements to push
			push_front(arr[s]);
	}
}

template <class T>
LinkList<T>::LinkList(const LinkList<T> &rhs): head(NULL), _size(rhs._size)
{
	LinkListNode<T> **p = head, *q = rhs.head;
	while (q) {
		*p = new LinkListNode<T>(q->val);
		p = &((*p)->next);
		q = q->next;
	}
}

template <class T>
LinkList<T> &LinkList<T>::operator=(const LinkList<T> rhs)
{
	LinkList<T> *tmp = rhs.head;
	rhs.head = head;
	head = tmp;
	_size = rhs._size;
	return *this;
}

template <class T>
LinkList<T>::~LinkList()
{
	clear();
}

template <class T>
inline bool LinkList<T>::empty() const
{
	return !head;
}

template <class T>
void LinkList<T>::clear()
{
	while (!empty())
		pop_front();
	_size = 0;
}

template <class T>
inline size_t LinkList<T>::size() const
{
	return _size;
}

template <class T>
void LinkList<T>::push_front(const T &val)
{
	head = new LinkListNode<T>(val, head);
	++_size;
}

template <class T>
void LinkList<T>::pop_front()
{
	if (!empty()) {
		LinkListNode<T> *tmp = head;
		head = head->next;
		delete tmp;
	}
	--_size;
}

template <class T>
void LinkList<T>::insert_after(LinkListNode<T> *p, const T &val)
{
	p->next = new LinkListNode<T>(val, p->next);
	++_size;
}

template <class T>
void LinkList<T>::erase_after(LinkListNode<T> *p)
{
	LinkListNode<T> *tmp = p->next;
	if (tmp)
		p->next = tmp->next;
	delete tmp;
	--_size;
}

template <class T>
LinkListNode<T> *LinkList<T>::get(size_t i) const
{
	LinkListNode<T> *p = head;
	while (p && i--)
		p = p->next;
	return p;
}

template <class T>
void LinkList<T>::reverse()
{
	LinkListNode<T> *p = head;
	head = NULL;
	while (p) {
		LinkListNode<T> *next = p->next;
		p->next = head;
		head = p;
		p = next;
	}
}

template <class T>
LinkListNode<T> *LinkList<T>::get_last(size_t i) const
{
	return get(_size-i-1);
}

#endif // LINK_LIST_H
