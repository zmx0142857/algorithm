#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <queue>
#include <stack>

template <class T>
struct BinaryTreeNode {
	T val;
	BinaryTreeNode *left;
	BinaryTreeNode *right;
	BinaryTreeNode():
		val(), left(NULL), right(NULL) {}
	BinaryTreeNode(T v, BinaryTreeNode *l = NULL, BinaryTreeNode *r = NULL):
		val(v), left(l), right(r) {}
};

template <class T> class BinaryTree;
template <class T>
std::ostream &operator<<(std::ostream &, const BinaryTree<T> &);

template <class T>
class BinaryTree {
	friend std::ostream &operator<< <T>(std::ostream &os, const BinaryTree<T> &rhs);

public:
	BinaryTree();
	BinaryTree(const T &val, BinaryTree &left, BinaryTree &right);
	// 从前序序列构造二叉树， null 为指定的 null 值, 默认为 T 类型的默认值
	BinaryTree(const T *arr, size_t size, const T &null=T());
	BinaryTree(const BinaryTree<T> &);
	BinaryTree<T> &operator=(BinaryTree<T>);
	~BinaryTree();

	const BinaryTreeNode<T> *get_root() const { return root; }
	BinaryTreeNode<T> *get_root() { return root; }

	// 类型别名
	typedef std::queue<BinaryTreeNode<T>*> Queue;

	static void preorder(Queue &q, BinaryTreeNode<T> *node);
	static void inorder(Queue &q, BinaryTreeNode<T> *node);
	static void postorder(Queue &q, BinaryTreeNode<T> *node);
	static void levelorder(Queue &q, BinaryTreeNode<T> *node);
	static void preorder_nonrec(Queue &q, BinaryTreeNode<T> *node);
	static void inorder_nonrec(Queue &q, BinaryTreeNode<T> *node);
	static void postorder_nonrec(Queue &q, BinaryTreeNode<T> *node);

	BinaryTreeNode<T> *parent(BinaryTreeNode<T> *node);

	enum Display {PRE, IN, POST, LEVEL, PRE_N, IN_N, POST_N};
	static Display display;

private:
	void construct(const T *arr, size_t size, size_t &i, BinaryTreeNode<T> *&p, const T &null);
	void copy(BinaryTreeNode<T> *&dest, BinaryTreeNode<T> *src);
	void destruct(BinaryTreeNode<T> *node);

	BinaryTreeNode<T> *root;

#ifdef DEBUG
public:
	void test_traverse()
	{
		using namespace std;
		display = PRE;			cout << "PRE:   " << *this << endl;
		display = IN;			cout << "IN:    " << *this << endl;
		display = POST;			cout << "POST:  " << *this << endl;
		display = LEVEL;		cout << "LEVEL: " << *this << endl;
		display = PRE_N;		cout << "PRE_N: " << *this << endl;
		display = IN_N;			cout << "IN_N:  " << *this << endl;
		display = POST_N;		cout << "POST_N:" << *this << endl;
	}

	static void test_construct()
	{
		/*
		     7
		   /   \
		  5     6
		 / \   / \
		1   2 3   4
		preorder: 7 5 1 2 6 3 4
		inorder: 1 5 2 7 3 6 4
		postorder: 1 2 5 3 4 6 7
		levelorder: 7 5 6 1 2 3 4
		*/
		using namespace std;

		// 以默认方式 PRE 输出各树

		// 默认构造
		BinaryTree<int> null_tree;
		cout << null_tree << endl;

		// 第二个构造函数
		BinaryTree<int> t1(1, null_tree, null_tree);
		BinaryTree<int> t2(2, null_tree, null_tree);
		BinaryTree<int> t3(3, null_tree, null_tree);
		BinaryTree<int> t4(4, null_tree, null_tree);
		BinaryTree<int> t5(5, t1, t2);
		BinaryTree<int> t6(6, t3, t4);
		BinaryTree<int> t7(7, t5, t6);
		cout << t7 << endl;

		// 第三个构造函数
		int arr1[] = {0};
		int arr2[] = {1, 0, 0}; 
		int arr3[] = {7, 5, 1, 0, 0, 2, 0, 0, 6, 3, 0, 0, 4, 0, 0};
		
		BinaryTree<int> bt1(arr1, sizeof(arr1)/sizeof(int));
		BinaryTree<int> bt2(arr2, sizeof(arr2)/sizeof(int));
		BinaryTree<int> bt3(arr3, sizeof(arr3)/sizeof(int));

		bt1.test_traverse();
		bt2.test_traverse();
		bt3.test_traverse();
	}

	static void test_copy()
	{
		using namespace std;
		int arr[] = {7, 5, 1, -1, -1, 2, -1, -1, 6, 3, -1, -1, 4, -1, -1};
		BinaryTree<int> bt(arr, sizeof(arr)/sizeof(int), -1);
		BinaryTree<int> bt1(bt);
		bt1.get_root()->val = 111;
		BinaryTree<int> bt2;
		bt2 = bt1;
		bt2.get_root()->left->val = 222;
		cout << bt << endl;
		cout << bt1 << endl;
		cout << bt2 << endl;
	}

	static void test_parent()
	{
		using namespace std;
		int arr3[] = {7, 5, 1, -1, -1, 2, -1, -1, 6, 3, -1, -1, 4, -1, -1};
		BinaryTree<int> bt3(arr3, sizeof(arr3)/sizeof(int), -1);
		Queue q;
		bt3.levelorder(q, bt3.get_root());
		while (!q.empty()) {
			BinaryTreeNode<T> *node = q.front();
			q.pop();
			BinaryTreeNode<T> *p = bt3.parent(node);
			cout << (!p ? 0 : p->val) << endl;
		}
	}
#endif
};

template <class T>
typename BinaryTree<T>::Display BinaryTree<T>::display = PRE;

template <class T>
std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &rhs)
{
	typename BinaryTree<T>::Queue q;
	switch (BinaryTree<T>::display) {
		case BinaryTree<T>::PRE: rhs.preorder(q, rhs.root); break;
		case BinaryTree<T>::IN: rhs.inorder(q, rhs.root); break;
		case BinaryTree<T>::POST: rhs.postorder(q, rhs.root); break;
		case BinaryTree<T>::LEVEL: rhs.levelorder(q, rhs.root); break;
		case BinaryTree<T>::PRE_N: rhs.preorder_nonrec(q, rhs.root); break;
		case BinaryTree<T>::IN_N: rhs.inorder_nonrec(q, rhs.root); break;
		case BinaryTree<T>::POST_N: rhs.postorder_nonrec(q, rhs.root); break;
	}
	os << '[';
	if (!q.empty()) {
		os << q.front()->val;
		q.pop();
	}
	while (!q.empty()) {
		os << ", " << q.front()->val;
		q.pop();
	}
	return os << ']';
}

template <class T>
BinaryTree<T>::BinaryTree(): root(NULL) {}

template <class T>
BinaryTree<T>::BinaryTree(const T &val, BinaryTree &left, BinaryTree &right):
	root(new BinaryTreeNode<T>(val, left.root, right.root))
{
	left.root = NULL;
	right.root = NULL;
}

// 从扩充前序序列构造二叉树
// size > 0, 因为空树的扩充表示是 [null]
// 这里 null 是 T 类型的默认值, 比如 T = int 时, null 就是 0
template <class T>
BinaryTree<T>::BinaryTree(const T *arr, size_t size, const T &null): root(NULL)
{
	size_t i = 0;
	construct(arr, size, i, root, null);
}

template <class T>
BinaryTree<T>::BinaryTree(const BinaryTree<T> &rhs): root(NULL)
{
	copy(root, rhs.root);
}

template <class T>
BinaryTree<T> &BinaryTree<T>::operator=(BinaryTree<T> rhs)
{
	BinaryTreeNode<T> *tmp = rhs.root;
	rhs.root = root;
	root = tmp;
	return *this;
}

template <class T>
BinaryTree<T>::~BinaryTree()
{
	destruct(root);
}

template <class T>
void BinaryTree<T>::preorder(Queue &q, BinaryTreeNode<T> *node)
{
	if (node) {
		q.push(node);
		preorder(q, node->left);
		preorder(q, node->right);
	}
}

template <class T>
void BinaryTree<T>::inorder(Queue &q, BinaryTreeNode<T> *node)
{
	if (node) {
		inorder(q, node->left);
		q.push(node);
		inorder(q, node->right);
	}
}

template <class T>
void BinaryTree<T>::postorder(Queue &q, BinaryTreeNode<T> *node)
{
	if (node) {
		postorder(q, node->left);
		postorder(q, node->right);
		q.push(node);
	}
}

template <class T>
void BinaryTree<T>::levelorder(Queue &q, BinaryTreeNode<T> *node)
{
	BinaryTreeNode<T> *p = node;
	Queue q0;
	q0.push(p);
	while (!q0.empty() && p) {
		p = q0.front();
		q0.pop();
		q.push(p);
		if (p->left)
			q0.push(p->left);
		if (p->right)
			q0.push(p->right);
	}
}

template <class T>
void BinaryTree<T>::preorder_nonrec(Queue &q, BinaryTreeNode<T> *node)
{
	std::stack<BinaryTreeNode<T>*> s; 
	s.push(NULL);
	while (node) {
		q.push(node);
		if (node->right)
			s.push(node->right);
		if (node->left)
			node = node->left;
		else {
			node = s.top();
			s.pop();
		}
	}
}

template <class T>
void BinaryTree<T>::inorder_nonrec(Queue &q, BinaryTreeNode<T> *node)
{
	std::stack<BinaryTreeNode<T>*> s;
	while (!s.empty() || node) {
		if (node) {
			s.push(node);
			node = node->left;
		} else {
			node = s.top();
			s.pop();
			q.push(node);
			node = node->right;
		}
	}
}

template <class T>
void BinaryTree<T>::postorder_nonrec(Queue &q, BinaryTreeNode<T> *node)
{
	std::stack<BinaryTreeNode<T>*> s;
	std::stack<char> s_choice;
	while (!s.empty() || node) {
		while (node) {
			s.push(node);
			s_choice.push('L');
			node = node->left;
		}
		node = s.top();
		s.pop();
		char choice = s_choice.top();
		s_choice.pop();
		if (choice == 'L') {
			s.push(node);
			s_choice.push('R');
			node = node->right;
		} else {
			q.push(node);
			node = NULL;
		}
	}
}

template <class T>
BinaryTreeNode<T> *BinaryTree<T>::parent(BinaryTreeNode<T> *node)
{
	BinaryTreeNode<T> *p = root;
	if (!node) {
		std::cout << "node is invalid\n";
		return NULL;
	}
	if (node == root) {
		return root; // parent of root is root itself
	}
	// 中序遍历
	std::stack<BinaryTreeNode<T>*> s;
	while (!s.empty() || p) {
		if (p) {
			if (node == p->left || node == p->right)
				return p;
			s.push(p);
			p = p->left;
		} else {
			p = s.top();
			s.pop();
			p = p->right;
		}
	}
	// cannot find parent
	return NULL;
}

// p 是指针的引用, 用引用而不用指针, 好处是少写一个 *
template <class T>
void BinaryTree<T>::construct(const T *arr, size_t size, size_t &i, BinaryTreeNode<T> *&p, const T &null)
{
	if (arr[i++] != null) {
		p = new BinaryTreeNode<T>(arr[i-1], NULL, NULL);
		if (i == size)
			return;
		construct(arr, size, i, p->left, null);
		construct(arr, size, i, p->right, null);
	}
}

template <class T>
void BinaryTree<T>::copy(BinaryTreeNode<T> *&dest, BinaryTreeNode<T> *src)
{
	if (src) {
		dest = new BinaryTreeNode<T>(src->val, src->left, src->right);
		copy(dest->left, src->left);
		copy(dest->right, src->right);
	}
}

template <class T>
void BinaryTree<T>::destruct(BinaryTreeNode<T> *node)
{
	if (node) {
		destruct(node->left);
		destruct(node->right);
		delete node;
	}
}

#endif // BINARY_TREE_H
