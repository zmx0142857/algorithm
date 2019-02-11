#include <iostream>
#include "stack.h"
#include "queue.h"

using namespace std;

void reverse1(ArrStack &s)
{
	ArrStack s1(s.size()), s2(s.size());
	while (!s.empty())
		s1.push(s.pop());
	while (!s1.empty())
		s2.push(s1.pop());
	while (!s2.empty())
		s.push(s2.pop());
}

void reverse2(ArrStack &s)
{
	ArrQueue q(s.size());
	while (!s.empty())
		q.enqueue(s.pop());
	while (!q.empty())
		s.push(q.dequeue());
}

void reverse3(ArrStack &s)
{
	size_t n = s.size();
	ArrStack s1(n);
	if (n == 0)
		return;
	while (--n) {
		int temp = s.pop();
		for (size_t i = 0; i != n; ++i)
			s1.push(s.pop());
		s.push(temp);
		while (!s1.empty())
			s.push(s1.pop());
	}
}

// 归并排序 O(nlog(n))
// 先两个两个排好，再四个四个排好；
// 已经排好序的两个队列归并后也有序。
void sort1(ArrQueue &q)
{
	ArrQueue q1(q.size()), q2(q.size());
	for (size_t gap = 1; gap < q.size(); gap <<=1) {
		// divide
		while (!q.empty()) {
			size_t n = gap;
			while (!q.empty() && n--)
				q1.enqueue(q.dequeue());
			n = gap;
			while (!q.empty() && n--)
				q2.enqueue(q.dequeue());
		}
		// merge
		while (!q1.empty() && !q2.empty()) {
			size_t i = gap, j = gap;
			while (!q1.empty() && !q2.empty() && i && j) {
				if (q1.get() <= q2.get()) {
					q.enqueue(q1.dequeue());
					--i;
				} else {
					q.enqueue(q2.dequeue());
					--j;
				}
			}
			while (!q1.empty() && i--)
				q.enqueue(q1.dequeue());
			while (!q2.empty() && j--)
				q.enqueue(q2.dequeue());
		}
		while (!q1.empty())
			q.enqueue(q1.dequeue());
		while (!q2.empty())
			q.enqueue(q2.dequeue());
	}
}

// 选择排序 O(n^2)
// 助手队列原地转
void sort2(ArrQueue &q)
{
	if (q.size() < 2)
		return;
	ArrQueue q1(q.size());
	int min = q.get();
	q1.enqueue(q.dequeue());
	while (!q.empty()) {
		if (q.get() < min) {
			min = q.get();
		}
		q1.enqueue(q.dequeue());
	}
	while (!q1.empty()) {
		int last_min = min;
		min = q1.get();
		for (size_t i = 0; !q1.empty() && i < q1.size(); ++i) {
			if (q1.get() == last_min) {
				q.enqueue(q1.dequeue());
			} else {
				q1.enqueue(q1.dequeue());
				if (q1.get() < min)
					min = q1.get();
			}
		}
	}
}

// O(n)
void move1(ArrStack &s1, ArrStack &s2)
{
	ArrStack s(s1.size());
	while (!s1.empty())
		s.push(s1.pop());
	while (!s.empty())
		s2.push(s.pop());
}

// O(n^2)
void move2(ArrStack &s1, ArrStack &s2)
{
	size_t n = s1.size();
	while (n--) {
		for (size_t i = 0; i < n; i++)
			s2.push(s1.pop());
		int temp = s1.pop();
		for (size_t i = 0; i < n; i++)
			s1.push(s2.pop());
		s2.push(temp);
	}
}

// 以下为测试函数
void test_reverse(void reverse(ArrStack &))
{
	int arr[] = {0};
	ArrStack s(arr, 0);
	cout << s << endl;
	reverse(s);
	cout << s << endl;

	int arr1[] = {0};
	ArrStack s1(arr1, 1);
	cout << s1 << endl;
	reverse(s1);
	cout << s1 << endl;

	int arr2[] = {0, 1, 2, 3, 4};
	ArrStack s2(arr2, 5);
	cout << s2 << endl;
	reverse(s2);
	cout << s2 << endl;
}

void test_sort(void sort(ArrQueue &))
{
	int arr[] = {2, 4, 7, 6, 1, 5, 9, 10, 1, 3, 8};
	//int arr[] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0};
	ArrQueue q(arr, 11);
	cout << q << endl;
	sort(q);
	cout << q << endl;

	int arr1[] = {0};
	ArrQueue q1(arr1, 1);
	cout << q1 << endl;
	sort(q1);
	cout << q1 << endl;

	int arr2[] = {0};
	ArrQueue q2(arr2, 0);
	cout << q2 << endl;
	sort(q2);
	cout << q2 << endl;
}

void test_move(void move(ArrStack &, ArrStack &))
{
	int arr2[] = {0, 1, 2, 3, 4};
	ArrStack s2(arr2, 5), t2(5);
	cout << s2 << endl << t2 << endl;
	move(s2, t2);
	cout << s2 << endl << t2 << endl;

	int arr1[] = {0};
	ArrStack s1(arr1, 1), t1(1);
	cout << s1 << endl << t1 << endl;
	move(s1, t1);
	cout << s1 << endl << t1 << endl;

	int arr[] = {0};
	ArrStack s(arr, 0), t(0);
	cout << s << endl << t << endl;
	move(s, t);
	cout << s << endl << t << endl;
}

int main()
{
	//test_reverse(reverse1);
	//test_reverse(reverse2);
	//test_reverse(reverse3);
	//test_sort(sort1);
	//test_sort(sort2);
	test_move(move1);
	test_move(move2);
	return 0;
}