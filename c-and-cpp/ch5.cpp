#include <iostream>
#define DEBUG
#include "binary_tree.h"

using namespace std;

template <class T>
size_t count_node(BinaryTreeNode<T> *root)
{
	if (!root)
		return 0;
	else
		return count_node(root->left) + count_node(root->right) + 1;
}

template <class T>
size_t count_leaf(BinaryTreeNode<T> *root)
{
	if (!root)
		return 0;
	else if (!root->left && !root->right)
		return 1;
	else
		return count_leaf(root->left) + count_leaf(root->right);
}

template <class T>
size_t height(BinaryTreeNode<T> *root)
{
	if (!root)
		return 0;
	return max(height(root->left), height(root->right)) + 1;
}

template <class T>
void mirror(BinaryTreeNode<T> *root)
{
	if (!root)
		return;
	mirror(root->left);
	mirror(root->right);
	BinaryTreeNode<T> *temp = root->left;
	root->left = root->right;
	root->right = temp;
}

template <class T>
bool search(BinaryTreeNode<T> *root, const T &K)
{
	if (!root)
		return false;
	if (root->val == K)
		return true;
	return search(root->left, K) || search(root->right, K);
}

template <class T>
bool search_nonrec(BinaryTreeNode<T> *root, const T &K)
{
	stack <BinaryTreeNode<T> *> s;
	s.push(NULL);
	while (root) {
		if (root->val == K)
			return true;
		if (root->right)
			s.push(root->right);
		if (root->left)
			root = root->left;
		else {
			root = s.top();
			s.pop();
		}
	}
	return false;
}

template <class T>
bool is_bst(BinaryTreeNode<T> *root)
{
	if (!root)
		return true;
	if (root->left && root->left->val >= root->val)
		return false;
	if (root->right && root->right->val <= root->val)
		return false;
	return is_bst(root->left) && is_bst(root->right);
}

bool do_is_min_heap(int *arr, size_t i, size_t size)
{
	// 2*i+1 left; 2*i+2 right
	if (i >= size) // empty heap
		return true;
	if (2*i+1 < size && arr[2*i+1] < arr[i])
		return false;
	if (2*i+2 < size && arr[2*i+2] < arr[i])
		return false;
	return do_is_min_heap(arr, 2*i+1, size)
		&& do_is_min_heap(arr, 2*i+2, size);
}

bool is_min_heap(int *arr, size_t size)
{
	return do_is_min_heap(arr, 0, size);
}

int arr1[] = {0};
BinaryTree<int> bt1(arr1, sizeof(arr1)/sizeof(int));

int arr2[] = {1, 0, 0};
BinaryTree<int> bt2(arr2, sizeof(arr2)/sizeof(int));

/*
	 7      	 		 7     
   /   \     		   /   \
  5     6   mirror->  6     5
 /     / \   		 / \     \
1     3   4 	 	4   3     1
*/
int arr3[] = {7, 5, 1, 0, 0, 0, 6, 3, 0, 0, 4, 0, 0};
BinaryTree<int> bt3(arr3, sizeof(arr3)/sizeof(int));

/*
	 7
   /   \
  5     8
 /     / \
1     2   9
*/
int arr4[] = {7, 5, 1, 0, 0, 0, 8, 2, 0, 0, 9, 0, 0};
BinaryTree<int> bt4(arr4, sizeof(arr4)/sizeof(int));

void test_count()
{
	cout << bt1 << endl;
	cout << "count_node: " << count_node(bt1.get_root()) << endl;
	cout << "count_leaf: " << count_leaf(bt1.get_root()) << endl;
	cout << "height: " << height(bt1.get_root()) << endl;

	cout << bt2 << endl;
	cout << "count_node: " << count_node(bt2.get_root()) << endl;
	cout << "count_leaf: " << count_leaf(bt2.get_root()) << endl;
	cout << "height: " << height(bt2.get_root()) << endl;

	cout << bt3 << endl;
	cout << "count_node: " << count_node(bt3.get_root()) << endl;
	cout << "count_leaf: " << count_leaf(bt3.get_root()) << endl;
	cout << "height: " << height(bt3.get_root()) << endl;
}

void test_mirror()
{
	cout << bt3 << endl;
	mirror(bt3.get_root());
	cout << "mirrored: " << bt3 << endl;
	mirror(bt3.get_root());
	cout << "restored: " << bt3 << endl;
}

void test_search()
{
	cout << "search for 1: " << search(bt3.get_root(), 1) << endl;
	cout << "search for 2: " << search(bt3.get_root(), 2) << endl;
	cout << "search for 3: " << search(bt3.get_root(), 3) << endl;
 	
	cout << "search_nonrec for 1: " << search_nonrec(bt3.get_root(), 1) << endl;
	cout << "search_nonrec for 2: " << search_nonrec(bt3.get_root(), 2) << endl;
	cout << "search_nonrec for 3: " << search_nonrec(bt3.get_root(), 3) << endl;
}

void test_is_bst()
{
	cout << bt3 << endl;
	cout << "is_bst: " << is_bst(bt3.get_root()) << endl;

	cout << bt4 << endl;
	cout << "is_bst: " << is_bst(bt4.get_root()) << endl;
}

void test_is_min_heap()
{
	/*
		 1
	   /   \
	  2     3
	 / \   /
	5   6 7
	*/
	int arr5[] = {1, 2, 3, 5, 6, 7};

	cout << "is_min_heap: " << is_min_heap(arr5, sizeof(arr5)/sizeof(int)) << endl;
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		cout << "usage: ch5 [option]\n"
		"-c\ttest_count\n"
		"-m\ttest_mirror\n"
		"-s\ttest_search\n"
		"-b\ttest_is_bst\n"
		"-h\ttest_is_min_heap\n";
	} else if (argc == 2) {
		switch (argv[1][1]) {
			case 'c': test_count(); break;
			case 'm': test_mirror(); break;
			case 's': test_search(); break;
			case 'b': test_is_bst(); break;
			case 'h': test_is_min_heap(); break;
			default: break;
		}
	} else {
		cout << "too many arguments\n";
	}
	return 0;
}
/*
int main(int argc, char **argv)
{
	if (argc == 1) {
		cout << "usage: ch4 [option]\n"
		"-s\ttest_shuffle\n"
		"-a\ttest_atoi\n"
		"-i\ttest_count_int\n"
		"-c\ttest_are_cyclic_permutations\n"
		"-r\ttest_longest_repeat_substr\n";
	} else if (argc == 2) {
		switch (argv[1][1]) {
			case 's': test_shuffle(); break;
			case 'a': test_my_atoi(); break;
			case 'i': test_count_int(); break;
			case 'c': test_are_cyclic_permutations(); break;
			case 'r': test_longest_repeat_substr(); break;
			default: break;
		}
	} else {
		cout << "too many arguments\n";
	}
	return 0;
}
*/
