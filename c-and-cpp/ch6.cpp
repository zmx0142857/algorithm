#include <iostream>
#include "tree.h"
using namespace std;

template <class T>
size_t count_nodes(TreeNode<T> *node)
{
	if (!node)
		return 0;
	return count_nodes(node->child) + count_nodes(node->sibling) + 1;
}

void test_count()
{
	/*        1  		      2
	 *       / \ 		     /|\
	 *      3   4		    5 6 7
	 *     / \   		  / | |
	 *    8   9  		10 11 12
	 */
	int arr[] = {1, 3, 8, 0, 9, 0, 0, 4, 0, 0,
		2, 5, 10, 0, 11, 0, 0, 6, 12, 0, 0, 7, 0, 0, 0};
	Tree<int> t(arr, sizeof(arr)/sizeof(int));
	cout << "tree:" << t << endl;
	cout << "nodes: " << count_nodes(t.get_root());
}

int main()
{
	test_count();
	return 0;
}
