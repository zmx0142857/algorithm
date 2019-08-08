#ifndef CIR_LIST_H
#define CIR_LIST_H

template <class T>
struct CirListNode {
	T val;
	CirListNode<T> *next;

	CirListNode(): val(), next(NULL) {}
	CirListNode(const T &v, CirListNode<T> *n=NULL): val(v), next(n) {}
};

template <class T> class CirList;
template <class T> std::ostream &operator<<(std::ostream &, const CirList<T> &);

template <class T>
class CirList {

	friend std::ostream &operator<< <>(std::ostream &, const CirList<T> &);

public:
	CirList(const T *arr, size_t size);
	CirList(const CirList<T> &);
	CirList<T> &operator=(CirList<T>);
	~CirList();

	bool empty() const;
	bool singular() const;
	CirListNode<T> *get(size_t i) const;
	CirListNode<T> *next(CirListNode<T> *p, size_t i) const;
	void insert_after(CirListNode<T> *p, const T &val);
	void erase_after(CirListNode<T> *p);

	// 模拟 Josephus 问题的求解
	// 编号为 1 ... n 的 n 个人
	static void emulate_josephus(int n_people, int start, int step);

protected:
	CirListNode<T> *head;

#ifdef DEBUG
public:
	void static test()
	{
		using namespace std;
		emulate_josephus(10, 1, 7);		// 7 4 2 1 3 6 10 5 8 9 
	}
#endif
};

template <class T>
std::ostream &operator<<(std::ostream &os, const CirList<T> &rhs)
{
	os << "[";
	CirListNode<T> *p = rhs.head;
	if (p) {
		os << p->val;
		p = p->next;
	}
	while (p != rhs.head) {
		os << ", " << p->val;
		p = p->next;
	}
	return os << "]";
}

template <class T>
CirList<T>::CirList(const T *arr, size_t size): head(NULL)
{
	if (size > 0) {
		insert_after(head, arr[0]);
		// still size-1 nodes to insert
		while (size > 1)
			insert_after(head, arr[--size]);
	}
}

template <class T>
CirList<T>::CirList(const CirList<T> &rhs): head(NULL)
{
	if (!rhs.empty()) {
		insert_after(head, rhs.head->val);
		CirListNode<T> *src = rhs.head->next, *dest = head;
		while (src != rhs.head) {
			insert_after(dest, src->val);
			dest = dest->next;
			src = src->next;
		}
	}
}

template <class T>
CirList<T> &CirList<T>::operator=(CirList<T> rhs)
{
	CirListNode<T> *tmp = rhs.head;
	rhs.head = head;
	head = tmp;
	return *this;
}

template <class T>
CirList<T>::~CirList()
{
	while (!empty())
		erase_after(head);
}

template <class T>
inline bool CirList<T>::empty() const
{
	return !head;
}

template <class T>
inline bool CirList<T>::singular() const
{
	return head && head->next == head;
}

template <class T>
CirListNode<T> *CirList<T>::next(CirListNode<T> *p, size_t i) const
{
	if (empty())
		return NULL;
	while (i--)
		p = p->next;
	return p;	
}

template <class T>
CirListNode<T> *CirList<T>::get(size_t i) const
{
	return next(head, i);
}

template <class T>
void CirList<T>::insert_after(CirListNode<T> *p, const T &val)
{
	if (p) {
		p->next = new CirListNode<T>(val, p->next);
	} else if (empty()) {
		head = new CirListNode<T>(val);
		head->next = head;
	}
}

template <class T>
void CirList<T>::erase_after(CirListNode<T> *p)
{
	if (!p)
		return;
	CirListNode<T> *temp = p->next;
	p->next = temp->next;
	if (temp == head) {
		if (singular()) {
			delete head;
			head = NULL;
			return;
		} else
			head = p->next;
	}
	delete temp;
}

template <class T>
void CirList<T>::emulate_josephus(int n, int s, int m)
{
	int *arr = new int[n];
	for (int i = 0; i < n; i++)
		arr[i] = i+1;
	CirList<int> cl(arr, n);
	CirListNode<int> *p;
	p = cl.get(n-1);
	p = cl.next(p, s-1);
	while (!cl.empty()) {
		p = cl.next(p, m-1);
		std::cout << p->next->val << ' ';
		cl.erase_after(p);
	}
	std::cout << '\n';
	delete[] arr;
}

#endif // CIR_LIST_H
