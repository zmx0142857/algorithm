#ifndef TREE_H
#define TREE_H

#include <cstdio>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>

template <class T>
struct TreeNode {
	T val;
	TreeNode<T> *child;
	TreeNode<T> *sibling;

	TreeNode():
		val(), child(NULL), sibling(NULL) {}
	TreeNode(const T &v, TreeNode *c = NULL, TreeNode *s = NULL):
		val(v), child(c), sibling(s) {}

	bool is_leaf() const
	{
		return !child;
	}

	// insert node as first child of *this
	void insert(TreeNode<T> *node)
	{
		if (child)
			node->sibling = child;
		child = node;
	}
};

template <class T>
struct ZipTreeNode {
	T val;
	bool has_child;
	bool has_sibling;
	ZipTreeNode() {}
	ZipTreeNode(const T &v, bool hc, bool hs):
		val(v), has_child(hc), has_sibling(hs) {}
};

template <class T>
struct DegreeZipTreeNode {
	T val;
	size_t degree; // number of children
	DegreeZipTreeNode() {}
	DegreeZipTreeNode(const T &v, size_t d):
		val(v), degree(d) {}
};

template <class T>
class Tree;

template <class T>
std::ostream &operator<<(std::ostream &, const Tree<T> &);

template <class T>
class Tree { // in fact, forest
	friend std::ostream &operator<< <T>(std::ostream &, const Tree<T> &);

public:
	enum Display {VERBOSE, PRE, POST, LEVEL, PRE_N, POST_N};

	Tree();
	Tree(const T &val, Tree &child, Tree &sibling=Tree());

	// construct tree from equalant binary tree preorder sequence.
	// null is a given special value, by default is the default value of class T
	// 从等价二叉树的前序序列构造树， null 为指定的 null 值, 默认为 T 类型的默认值
	Tree(const T *arr, size_t size, const T &null=T());

	// construct from zip tree sequence
	Tree(const ZipTreeNode<T> *, size_t, Display);
	Tree(const DegreeZipTreeNode<T> *, size_t, Display);

	// load tree from local file
	Tree(const std::string &file_name);

	Tree(const Tree<T> &);
	Tree<T> &operator=(const Tree<T>);
	~Tree();

	const TreeNode<T> *get_root() const { return root; }
	TreeNode<T> *get_root() { return root; }
	size_t size() const;

	// 类型别名
	typedef std::queue<TreeNode<T>*> Queue;

	static void preorder(Queue &q, TreeNode<T> *node);
	static void postorder(Queue &q, TreeNode<T> *node);
	static void levelorder(Queue &q, TreeNode<T> *node);
	static void preorder_nonrec(Queue &q, TreeNode<T> *node);
	static void postorder_nonrec(Queue &q, TreeNode<T> *node);

	TreeNode<T> *parent(TreeNode<T> *node);
	TreeNode<T> *elder_brother(TreeNode<T> *node);
	size_t count_child(TreeNode<T> *node);
	void erase_subtree(TreeNode<T> *node);

	// return false if the array space is insufficient
	bool zip(ZipTreeNode<T> *, size_t, Display);
	bool degree_zip(DegreeZipTreeNode<T> *, size_t, Display);

	// flag == 0: zip; flag == 1: degree_zip
	void save(const std::string &file_name, bool flag, Display);

	static Display display;

protected:
	void construct(const T *arr, size_t size, size_t &i, TreeNode<T> *&p, const T &null);
	void copy(TreeNode<T> *&dest, TreeNode<T> *src);
	void destruct(TreeNode<T> *node);
	std::ostream &verbose(std::ostream &os, std::vector<TreeNode<T>*> &node) const;
	size_t count_nodes(TreeNode<T> *node) const;

	TreeNode<T> *root;

#ifdef DEBUG

// empty tree
#define INIT_T1 int arr1[] = {0}; Tree<int> t1(arr1, sizeof(arr1)/sizeof(int))

/*        1  		      2
 *       / \ 		     /|\
 *      3   4		    5 6 7
 *     / \   		  / | |
 *    8   9  		10 11 12
 */
#define INIT_T2 int arr2[] = {1, 3, 8, 0, 9, 0, 0, 4, 0, 0, 2, 5, 10, 0, 11, 0, 0, 6, 12, 0, 0, 7, 0, 0, 0}; Tree<int> t2(arr2, sizeof(arr2)/sizeof(int))

// just a copy of t2
#define INIT_T3 Tree<int> t3(t2)

public:
	static void test_io()
	{
		using namespace std;

		INIT_T1;
		cout << "t1:" << t1 << endl;

		INIT_T2;
		cout << "t2:" << t2 << endl;
		display = PRE;
		cout << "t2:" << t2 << endl;

		INIT_T3;
		cout << "t3:" << t3 << endl;
		display = POST; cout << "post t3:" << t3 << endl;
		display = LEVEL; cout << "level t3:" << t3 << endl;
		display = PRE_N; cout << "pre_n t3:" << t3 << endl;
		display = POST_N; cout << "post_n t3:" << t3 << endl;
	}

#define PRINT_OR_NULL if (p) cout << p->val << endl; else cout << "NULL" << endl
	static void test_find_parent_brother()
	{
		using namespace std;
		INIT_T2;
		TreeNode<int> *p;
		p = t2.parent(t2.get_root());
		PRINT_OR_NULL; // NULL
		p = t2.parent(t2.get_root()->child->sibling);
		PRINT_OR_NULL; // 1
		p = t2.parent(t2.get_root()->child->child->sibling);
		PRINT_OR_NULL; // 3

		p = t2.elder_brother(t2.get_root()->sibling->child);
		PRINT_OR_NULL; // NULL
		p = t2.elder_brother(t2.get_root()->sibling);
		PRINT_OR_NULL; // 1
		p = t2.elder_brother(t2.get_root()->sibling->child->sibling->sibling);
		PRINT_OR_NULL; // 6
	}
#undef PRINT_OR_NULL

	static void test_erase_subtree()
	{
		using namespace std;
		INIT_T2;
		t2.erase_subtree(t2.get_root()->child);
		cout << "3 and its subtree removed:" << t2 << endl;
		t2.erase_subtree(t2.get_root()->sibling->child->sibling);
		cout << "6 and its subtree removed:" << t2 << endl;
	}

	static void test_zip(Display display)
	{
		using namespace std;
		INIT_T2;
		const size_t size = 12;
		cout << "t2:" << t2 << endl;
		ZipTreeNode<int> ziptree[size];

		bool ret = t2.zip(ziptree, size, display);
		cout << "zip returned " << (ret ? "true" : "false") << endl;
		if (ret) {
			cout << "dump: " << endl;
			for (size_t i = 0; i != size; ++i) {
				cout << ziptree[i].val << '\t'
					 << ziptree[i].has_child << '\t'
					 << ziptree[i].has_sibling << endl;
			}
			Tree<int> t3(ziptree, size, display);
			cout << "t3:" << t3 << endl;
		}
	}

	static void test_degree_zip(Display display)
	{
		using namespace std;
		INIT_T2;
		const size_t size = 12;
		cout << "t2:" << t2 << endl;
		DegreeZipTreeNode<int> ziptree[size];

		bool ret = t2.degree_zip(ziptree, size, display);
		cout << "degree zip returned " << (ret ? "true" : "false") << endl;
		if (ret) {
			cout << "dump: " << endl;
			for (size_t i = 0; i != size; ++i) {
				cout << ziptree[i].val << '\t'
					 << ziptree[i].degree << endl;
			}
			Tree<int> t3(ziptree, size, display);
			cout << "t3:" << t3 << endl;
		}
	}

	static void test_do_zip_file(bool flag, Display display)
	{
		using namespace std;
		INIT_T2;
		t2.save("t2", flag, display);
		Tree<int> t3("t2");
		cout << "t3: ---------------" << t3 << endl;
	}

	static void test_zip_file()
	{
		using namespace std;
		Tree<int>::test_do_zip_file(false, Tree<int>::PRE);
		Tree<int>::test_do_zip_file(true, Tree<int>::PRE);
		Tree<int>::test_do_zip_file(false, Tree<int>::LEVEL);
		Tree<int>::test_do_zip_file(true, Tree<int>::POST);
		Tree<int>::test_do_zip_file(true, Tree<int>::LEVEL);
	}
#undef INIT_T1
//#undef INIT_T2
#undef INIT_T3
#endif // DEBUG
};

template <class T>
typename Tree<T>::Display Tree<T>::display = VERBOSE;

template <class T>
std::ostream &operator<<(std::ostream &os, const Tree<T> &rhs)
{
	if (Tree<T>::display == Tree<T>::VERBOSE) {
		if (!rhs.root)
			return os << "\n<empty tree>";
		std::vector<TreeNode<T>*> v;
		v.push_back(rhs.root);
		return rhs.verbose(os, v);
	}
	
	typename Tree<T>::Queue q;
	switch (Tree<T>::display) {
		case Tree<T>::VERBOSE: break;
		case Tree<T>::PRE: rhs.preorder(q, rhs.root); break;
		case Tree<T>::POST: rhs.postorder(q, rhs.root); break;
		case Tree<T>::LEVEL: rhs.levelorder(q, rhs.root); break;
		case Tree<T>::PRE_N: rhs.preorder_nonrec(q, rhs.root); break;
		case Tree<T>::POST_N: rhs.postorder_nonrec(q, rhs.root); break;
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
Tree<T>::Tree(): root(NULL) {}

template <class T>
Tree<T>::Tree(const T &val, Tree &child, Tree &sibling):
	root(new TreeNode<T>(val, child.root, sibling.root))
{
	child.root = NULL;
	sibling.root = NULL;
}

// 从等价二叉树的扩充前序序列构造树
// size > 0, 因为空树的扩充表示是 [null]
// 这里 null 是 T 类型的默认值, 比如 T = int 时, null 就是 0
template <class T>
Tree<T>::Tree(const T *arr, size_t size, const T &null): root(NULL)
{
	size_t i = 0;
	construct(arr, size, i, root, null);
}

template <class T>
Tree<T>::Tree(const ZipTreeNode<T> *ziptree, size_t size, Display display): root(NULL)
{
	std::stack<TreeNode<T>**> s; 
	std::queue<TreeNode<T>**> q; 
	TreeNode<T> **p = &root;
	switch (display) {
		case LEVEL:
			for (size_t i = 0; i != size; ++i) {
				*p = new TreeNode<T>(ziptree[i].val);
				if (ziptree[i].has_child)
					q.push(&((*p)->child));
				if (ziptree[i].has_sibling)
					p = &((*p)->sibling);
				else {
					p = q.front();
					q.pop();
				}
			}
			break;

		default: // PRE
			s.push(NULL);
			for (size_t i = 0; i != size; ++i) {
				*p = new TreeNode<T>(ziptree[i].val);
				if (ziptree[i].has_sibling)
					s.push(&((*p)->sibling));
				if (ziptree[i].has_child)
					p = &((*p)->child);
				else {
					p = s.top();
					s.pop();
				}
			}
			break;
		/* the postorder ziptree is useless because there is more than one
		 * possible tree for this example:
		 * node			A B C
		 * has_child	0 0 1
		 * has_sibling	1 0 0
		 *
		 *      C      A  C
		 *     / \       /
		 *    A   B     B
		 */
	}
}

template <class T>
Tree<T>::Tree(const DegreeZipTreeNode<T> *ziptree, size_t size, Display display)
{
	std::stack<TreeNode<T>*> s; 
	std::stack<TreeNode<T>*> s_has_child;	// stores nodes who have child to connect
	std::stack<size_t> s_deg;				// stores the degree
	switch (display) {
		case POST:
			for (size_t i = 0; i != size; ++i) {
				root = new TreeNode<T>(ziptree[i].val);
				TreeNode<T> *p = NULL;
				size_t deg = ziptree[i].degree;
				while (deg--) {
					s.top()->sibling = p;
					p = s.top();
					s.pop();
				}
				root->child = p;
				s.push(root);
			}
			root = NULL;
			while (!s.empty()) {
				s.top()->sibling = root;
				root = s.top();
				s.pop();
			}
			break;
		case LEVEL:
			for (size_t i = 0; i != size; ++i) {
				TreeNode<T> *p = new TreeNode<T>(ziptree[i].val);
				s.push(p);
				if (ziptree[i].degree) {
					s_has_child.push(p);
					s_deg.push(ziptree[i].degree);
				}
			}
			// now connect the nodes - it's just like a puzzle game
			while (!s_has_child.empty()) {
				s_has_child.top()->insert(s.top());
				s.pop();
				--s_deg.top();
				if (s_deg.top() == 0) {
					s_has_child.pop();
					s_deg.pop();
				}
			}
			root = NULL;
			while (!s.empty()) {
				s.top()->sibling = root;
				root = s.top();
				s.pop();
			}
			break;
		default: /* PRE */
			if (size > 0) {
				root = new TreeNode<T>(ziptree[0].val);
				s.push(root);
				s_deg.push(ziptree[0].degree);
				for (size_t i = 1; i != size; ++i) {
					TreeNode<T> *p = new TreeNode<T>(ziptree[i].val);
					if (s_deg.top() == 0) {
						TreeNode<T> *q;
						while (!s.empty() && s_deg.top() == 0) {
							q = s.top();
							s.pop();
							s_deg.pop();
						}
						q->sibling = p;
					} else {
						s.top()->child = p;
					}
					if (!s.empty())
						--s_deg.top();
					s.push(p);
					s_deg.push(ziptree[i].degree);
				}
			}
			break;
	}
}

template <class T>
Tree<T>::Tree(const std::string &file_name): root(NULL)
{
	std::ifstream ifs(file_name.c_str(), ifs.binary);
	if (!ifs.is_open()) {
		std::cout << "failed to open " << file_name << '\n';
		return;
	}

	char info[8];
	for (size_t i = 0; i != 8; ++i)
		info[i] = ifs.get();

	Display display = PRE;
	switch (info[4]) {
		case 'O': display = POST; break;
		case 'E': display = LEVEL; break;
		default: break;
	}

	size_t sz;
	ifs.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
	if (info[0] == 'd') { // degree zip
		DegreeZipTreeNode<T> *arr = new DegreeZipTreeNode<T>[sz];
		ifs.read(reinterpret_cast<char*>(arr), sz*sizeof(DegreeZipTreeNode<T>));
		Tree<T> t(arr, sz, display);
		TreeNode<T> *p = t.root;
		t.root = root;
		root = p;
		delete[] arr;
	} else { // zip
		ZipTreeNode<T> *arr = new ZipTreeNode<T>[sz];
		ifs.read(reinterpret_cast<char*>(arr), sz*sizeof(ZipTreeNode<T>));
		Tree<T> t(arr, sz, display);
		TreeNode<T> *p = t.root;
		t.root = root;
		root = p;
		delete[] arr;
	}
	ifs.close();
}

template <class T>
Tree<T>::Tree(const Tree<T> &rhs): root(NULL)
{
	copy(root, rhs.root);
}

template <class T>
Tree<T> &Tree<T>::operator=(Tree<T> rhs)
{
	TreeNode<T> *tmp = rhs.root;
	rhs.root = root;
	root = tmp;
	return *this;
}

template <class T>
Tree<T>::~Tree()
{
	destruct(root);
}

template <class T>
size_t Tree<T>::size() const
{
	return count_nodes(root);
}

template <class T>
void Tree<T>::preorder(Queue &q, TreeNode<T> *node)
{
	while (node) {
		q.push(node);
		preorder(q, node->child);
		node = node->sibling;
	}
}

template <class T>
void Tree<T>::postorder(Queue &q, TreeNode<T> *node)
{
	while (node) {
		postorder(q, node->child);
		q.push(node);
		node = node->sibling;
	}
}

template <class T>
void Tree<T>::levelorder(Queue &q, TreeNode<T> *node)
{
	TreeNode<T> *p = node;
	Queue to_visit;
	while (true) {
		while (p) {
			to_visit.push(p);
			p = p->sibling;
		}
		if (to_visit.empty())
			break;
		p = to_visit.front();
		to_visit.pop();
		q.push(p);
		p = p->child;
	}
}

template <class T>
void Tree<T>::preorder_nonrec(Queue &q, TreeNode<T> *node)
{
	std::stack<TreeNode<T>*> s; 
	s.push(NULL);
	while (node) {
		q.push(node);
		if (node->sibling)
			s.push(node->sibling);
		if (node->child)
			node = node->child;
		else {
			node = s.top();
			s.pop();
		}
	}
}

template <class T>
void Tree<T>::postorder_nonrec(Queue &q, TreeNode<T> *node)
{
	std::stack<TreeNode<T>*> s;
	while (!s.empty() || node) {
		if (node) {
			s.push(node);
			node = node->child;
		} else {
			node = s.top();
			s.pop();
			q.push(node);
			node = node->sibling;
		}
	}
}

template <class T>
TreeNode<T> *Tree<T>::parent(TreeNode<T> *node)
{
	if (!node)
		return NULL;

	// level order traversal
	TreeNode<T> *p = root;
	TreeNode<T> *ret = NULL; // roots has no parent
	Queue to_visit;
	while (true) {
		while (p) {
			if (p == node)
				return ret;
			to_visit.push(p);
			p = p->sibling;
		}
		if (to_visit.empty())
			break;
		p = to_visit.front();
		to_visit.pop();
		ret = p;		// save current level pointer
		p = p->child;	// dive into next level
	}

	// cannot find parent
	return NULL;
}

template <class T>
TreeNode<T> *Tree<T>::elder_brother(TreeNode<T> *node)
{
	if (!node)
		return NULL;

	// level order traversal
	TreeNode<T> *p = root;
	Queue to_visit;
	while (true) {
		if (p == node)
			return NULL; // first child has no elder brother
		while (p) {
			if (p->sibling == node)
				return p;
			to_visit.push(p);
			p = p->sibling;
		}
		if (to_visit.empty())
			break;
		p = to_visit.front();
		to_visit.pop();
		p = p->child;	// dive into next level
	}

	// cannot find elder brother
	return NULL;
}

template <class T>
size_t Tree<T>::count_child(TreeNode<T> *node)
{
	if (!node || !node->child)
		return 0;
	TreeNode<T> *p = node->child;
	size_t ret = 1;
	while (p->sibling) {
		++ret;
		p = p->sibling;
	}
	return ret;
}

template <class T>
void Tree<T>::erase_subtree(TreeNode<T> *node)
{
	// node is NULL: nothing to do
	if (!node)
		return;
	// node is root: move root to its sibling
	else if (node == root)
		root = root->sibling;
	else {
		TreeNode<T> *p = parent(node);
		// node is first child of p
		if (p->child == node)
			p->child = node->sibling;
		else {
			p = p->child;
			while (p->sibling != node)
				p = p->sibling;
			p->sibling = node->sibling;
		}
	}
	node->sibling = NULL;	// disconnect
	destruct(node);
}

template <class T>
bool Tree<T>::zip(ZipTreeNode<T> *ziptree, size_t size, Display display)
{
	Queue q;
	switch (display) {
		case POST: postorder(q, root); break;
		case LEVEL: levelorder(q, root); break;
		default: preorder(q, root); break;
	}
	size_t i = 0;
	while (!q.empty()) {
		if (i == size)
			return false;
		TreeNode<T> *p = q.front();
		q.pop();
		ziptree[i++] = ZipTreeNode<T>(p->val, p->child, p->sibling);
	}
	return true;
}

template <class T>
bool Tree<T>::degree_zip(DegreeZipTreeNode<T> *ziptree, size_t size, Display display)
{
	Queue q;
	switch (display) {
		case POST: postorder(q, root); break;
		case LEVEL: levelorder(q, root); break;
		default: preorder(q, root); break;
	}
	size_t i = 0;
	while (!q.empty()) {
		if (i == size)
			return false;
		TreeNode<T> *p = q.front();
		q.pop();
		ziptree[i++] = DegreeZipTreeNode<T>(p->val, count_child(p));
	}
	return true;
}

template <class T>
void Tree<T>::save(const std::string &file_name, bool flag, Display display)
{
	std::ofstream ofs(file_name.c_str(), ofs.binary);
	if (!ofs.is_open()) {
		std::cout << "failed to open " << file_name << '\n';
		return;
	} 

	std::string disp;
	switch (display) {
		case PRE: disp = "PRE  "; break;
		case POST: disp = "POST "; break;
		case LEVEL: disp = "LEVEL"; break;
		default: return;
	}

	const size_t sz = size();
	if (flag) { // degree_zip
		DegreeZipTreeNode<T> *arr = new DegreeZipTreeNode<T>[sz];
		degree_zip(arr, sz, display);
		ofs << "deg" << disp;
		ofs.write(reinterpret_cast<const char*>(&sz), sizeof(size_t));
		ofs.write(reinterpret_cast<const char*>(arr), sz*sizeof(DegreeZipTreeNode<T>));
		delete[] arr;
	} else { // zip
		ZipTreeNode<T> *arr = new ZipTreeNode<T>[sz];
		zip(arr, sz, display);
		ofs << "   " << disp;
		ofs.write(reinterpret_cast<const char*>(&sz), sizeof(size_t));
		ofs.write(reinterpret_cast<const char*>(arr), sz*sizeof(ZipTreeNode<T>));
		delete[] arr;
	}
	ofs.close();
}

// p 是指针的引用, 用引用而不用指针, 好处是少写一个 *
template <class T>
void Tree<T>::construct(const T *arr, size_t size, size_t &i, TreeNode<T> *&p, const T &null)
{
	if (arr[i++] != null) {
		p = new TreeNode<T>(arr[i-1], NULL, NULL);
		if (i == size)
			return;
		construct(arr, size, i, p->child, null);
		construct(arr, size, i, p->sibling, null);
	}
}

template <class T>
void Tree<T>::copy(TreeNode<T> *&dest, TreeNode<T> *src)
{
	if (src) {
		dest = new TreeNode<T>(src->val, src->child, src->sibling);
		copy(dest->child, src->child);
		copy(dest->sibling, src->sibling);
	}
}

template <class T>
void Tree<T>::destruct(TreeNode<T> *node)
{
	if (node) {
		destruct(node->child);
		destruct(node->sibling);
		delete node;
	}
}

template <class T>
std::ostream &Tree<T>::verbose(
		std::ostream &os, std::vector<TreeNode<T>*> &nodes) const
{
	// nodes is guaranteed not to be empty
	while (nodes.back()) {
		os << '\n';
		// output none for first level
		// output "│ " if an ancester has sibling
		// output "├ " if this node has sibling
		// output "└ " if this node has no sibling
		if (nodes.size() > 1) {
			for (size_t i = 1; i < nodes.size()-1; ++i)
				printf(nodes[i]->sibling ? "│ " : "  ");
			printf(nodes.back()->sibling ? "├ " : "└ ");
		}
		os << nodes.back()->val;

		// descend into next level
		nodes.push_back(nodes.back()->child);
		verbose(os, nodes);
		nodes.pop_back();

		nodes.back() = nodes.back()->sibling;
	}
	return os;
}

template <class T>
size_t Tree<T>::count_nodes(TreeNode<T> *node) const
{
	if (!node)
		return 0;
	return count_nodes(node->child) + count_nodes(node->sibling) + 1;
}

#endif // TREE_H
