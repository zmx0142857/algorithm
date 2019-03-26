#include <iostream>
using namespace std;

struct ForwardListNode {
	int val;
	ForwardListNode *next;

	ForwardListNode():
		val(), next(NULL) {}
	ForwardListNode(int v, ForwardListNode *n=NULL):
		val(v), next(n) {}
};

class ForwardList {

	ForwardListNode *head;

public:
	ForwardList(int *arr, size_t size): head(NULL)
	{
		while (size)
			push_front(arr[--size]);
	}

	~ForwardList()
	{
		while (!empty())
			pop_front();
	}

	bool empty() const
	{
		return head == NULL;
	}

	ForwardListNode *get(size_t i) const
	{
		ForwardListNode *p = head;
		while (p && i--)
			p = p->next;
		return p;
	}

	void insert_after(ForwardListNode *p, int val)
	{
		if (!p)
			return;
		p->next = new ForwardListNode(val, p->next);
	}

	void erase_after(ForwardListNode *p)
	{
		if (!p || !p->next)
			return;
		ForwardListNode *tmp = p->next;
		p->next = tmp->next;
		delete tmp;
	}

	void push_front(int val)
	{
		head = new ForwardListNode(val, head);
	}

	void pop_front()
	{
		if (empty())
			return;
		ForwardListNode *tmp = head;
		head = head->next;
		delete tmp;
	}

	friend ostream &operator<<(ostream &os, const ForwardList &rhs)
	{
		ForwardListNode *p = rhs.head;
		os << '[';
		if (p) {
			os << p->val;
			p = p->next;
		}
		while (p) {
			os << ", " << p->val;
			p = p->next;
		}
		return os << ']';
	}

#ifdef DEBUG
	static void test()
	{
		int arr[] = {2, 3, 4, 1, 2, 3, 0, 5};
		ForwardList fl(arr, sizeof(arr)/sizeof(int));	cout << fl << endl;
		cout << fl.get(8) << endl;
		cout << fl.get(6)->val << endl;
		fl.insert_after(fl.get(7), 42);					cout << fl << endl;
		fl.push_front(100);								cout << fl << endl;
		fl.pop_front();									cout << fl << endl;
		fl.erase_after(fl.get(3));						cout << fl << endl;
		fl.erase_after(fl.get(6));						cout << fl << endl;
	}
#endif
};

// same as ForwardListNode
struct CirListNode {
	int val;
	CirListNode *next;

	CirListNode():
		val(), next(NULL) {}
	CirListNode(int v, CirListNode *n=NULL):
		val(v), next(n) {}
};

class CirList {

	CirListNode *head;

public:
	CirList(int *arr, size_t size): head(NULL)
	{
		if (size) {
			insert_after(head, arr[0]);
			while (size > 1)
				insert_after(head, arr[--size]);
		}
	}

	~CirList()
	{
		while (!empty())
			erase_after(head);
	}

	bool empty() const
	{
		return head == NULL;
	}

	bool singular() const
	{
		return !empty() && head->next == head;
	}

	// find ith node of list, i counts from 0
	// return head if i == 0
	// return get(i % len) if i >= length of list
	CirListNode *get(size_t i) const
	{
		return next(head, i);
	}

	// next ith node of p
	// return NULL if i is invalid
	// return p if i == 0
	CirListNode *next(CirListNode *p, size_t i) const
	{
		if (empty())
			return NULL;
		while (i--)
			p = p->next;
		return p;
	}

	void insert_after(CirListNode *p, int val)
	{
		if (p) {
			p->next = new CirListNode(val, p->next);
		} else if (empty()) {
			head = new CirListNode(val);
			head->next = head;
		}
	}

	void erase_after(CirListNode *p)
	{
		if (!p)
			return;
		CirListNode *tmp = p->next;
		p->next = tmp->next;
		delete tmp;
		if (tmp == head) {
			if (singular())
				head = NULL;
			else
				head = p->next;
		}
	}

	friend ostream &operator<<(ostream &os, const CirList &rhs)
	{
		os << '[';
		CirListNode *p = rhs.head;
		if (p) {
			os << p->val;
			p = p->next;
		}
		while (p != rhs.head) {
			os << ", " << p->val;
			p = p->next;
		}
		return os << ']';
	}

#ifdef DEBUG
	static void test()
	{
		int arr[] = {2, 3, 4, 1, 2, 3, 0, 5};
		CirList cl(arr, sizeof(arr)/sizeof(int));	cout << cl << endl;
		cout << cl.get(8)->val << endl;
		cout << cl.get(6)->val << endl;
		cl.insert_after(cl.get(7), 42);					cout << cl << endl;
		cl.erase_after(cl.get(3));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.erase_after(cl.get(6));						cout << cl << endl;
		cl.insert_after(cl.get(100), 222);				cout << cl << endl;
		cl.insert_after(cl.get(100), 666);				cout << cl << endl;
		cl.insert_after(cl.get(0), 555);				cout << cl << endl;
		cl.insert_after(cl.get(1), 111);				cout << cl << endl;
	}
#endif
};

struct DualListNode {
	int val;
	DualListNode *next;
	DualListNode *prev;

	DualListNode():
		val(), next(NULL), prev(NULL) {}
	DualListNode(int v, DualListNode *n=NULL, DualListNode *p=NULL):
		val(v), next(n), prev(p) {}
};

class DualList {
	
	DualListNode *head;
	DualListNode *tail; // dummy tail node

public:
	DualList(int *arr, size_t size):
		head(new DualListNode()), tail(head)
	{
		while (size)
			push_front(arr[--size]);
	}

	~DualList()
	{
		while (!empty())
			pop_front();
	}

	bool empty() const
	{
		return head == tail;
	}

	// find ith node of list, i counts from 0
	// return tail if i is invalid
	// return head if i == 0
	DualListNode *get(size_t i) const
	{
		DualListNode *p = head;
		while (p != tail && i--)
			p = p->next;
		return p;
	}

	// insert a node before p, p can be dummy tail node
	void insert(DualListNode *p, int val)
	{
		if (!p)
			return;
		else if (p == head)
			push_front(val);
		else {
			DualListNode *node = new DualListNode(val, p, p->prev);
			p->prev->next = node;
			p->prev = node;
		}

	}

	// delete the node p
	void erase(DualListNode *p)
	{
		if (!p || p == tail)
			return;
		else if (p == head)
			pop_front();
		else {
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
		}
	}

	void push_front(int val)
	{
		head->prev = new DualListNode(val, head);
		head = head->prev;
	}

	void pop_front()
	{
		if (empty())
			return;
		head = head->next;
		delete head->prev;
		head->prev = NULL;
	}

	friend ostream &operator<<(ostream &os, const DualList &rhs)
	{
		os << '[';
		DualListNode *p = rhs.head;
		if (p != rhs.tail) {
			os << p->val;
			p = p->next;
		}
		while (p != rhs.tail) {
			os << ", " << p->val;
			p = p->next;
		}
		return os << ']';
	}

#ifdef DEBUG
	static void test()
	{
		int arr[] = {0, 1, 2, 3, 4, 5};
		DualList dl(arr, sizeof(arr)/sizeof(int));
		cout << dl << endl;
		DualListNode *p = dl.get(6);
		while (p) {
			cout << p->val << endl;
			p = p->prev;
		}
		dl.insert(dl.get(0), 11);		cout << dl << endl;
		dl.insert(dl.get(1), 12);		cout << dl << endl;
		dl.insert(dl.get(7), 13);		cout << dl << endl;
		dl.insert(dl.get(100), 100);	cout << dl << endl;
		dl.erase(dl.get(100));			cout << dl << endl;
		dl.erase(dl.get(8));			cout << dl << endl;
		for (int i = 0; i < 9; ++i) {
			dl.pop_front();
			cout << dl << endl;
		}
		for (int i = 0; i < 9; ++i) {
			dl.push_front(i*i);
			cout << dl << endl;
		}
	}
#endif
};

int main()
{
	CirList::test();
	return 0;
}
