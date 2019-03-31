#ifndef ARRAY_H
#define ARRAY_H

#if __cplusplus < 201101
#error "require c++11"
#endif

#include <iostream>  // std::ostream
#include <stdexcept> // std::length_error
#include <algorithm> // std::sort(arr, arr+size)

// an auto-resizable array: aka vector
template <class T>
class Array {

protected:
	size_t capacity;
	size_t len;
	T *L;

public:
	// ---- consts ----
	static const size_t notfound = -1;

	// ---- constructors and memory management ----
	Array(size_t cap=1):
		capacity(valid_cap(cap)), len(0), L(new T[capacity]) {}

	Array(const T *arr, size_t n):
		capacity(valid_cap(n)), len(n), L(new T[capacity])
	{
		for (size_t i = 0; i != len; ++i)
			L[i] = arr[i];
	}

	Array(const Array &rhs, size_t cap):
		capacity(valid_cap(cap)), len(rhs.len), L(new T[capacity])
	{
		if (len > capacity)
			len = capacity;
		for (size_t i = 0; i != len; ++i)
			L[i] = rhs.L[i];
	}

	Array(const Array &rhs): // copy
		capacity(rhs.len), len(rhs.len), L(new T[capacity])
	{
		for (size_t i = 0; i != len; ++i)
			L[i] = rhs.L[i];
	}

	void swap(const Array &rhs) // swap
	{
		size_t tmp_len = capacity;
		capacity = rhs.capacity;
		rhs.capacity = tmp_len;
		tmp_len = len;
		len = rhs.len;
		rhs.len = tmp_len;
		const T *tmp = L;
		L = rhs.L;
		rhs.L = tmp;
	}

	~Array() // destruct
	{
		delete[] L;
	}

	// assign: consider self-assignment carefully
	Array &operator=(Array rhs) // pass rhs by value!
	{
		swap(rhs);
		return *this;
	}

	void realloc()
	{
		Array new_arr(*this, 2*capacity);
		swap(new_arr);
	}

	// ---- deal with len ----
	void clear()
	{
		len = 0;
	}

	size_t size() const
	{
		return len;
	}

	bool empty() const
	{
		return len == 0;
	}

	// ---- finder ----

	// return the first index i, such that condition(L[i]) == true
	// if reversed == true, find last instead of first
	size_t find_if(bool condition(const T &), bool reversed=false) const
	{
		if (!reversed) {
			for (size_t i = 0; i != len; ++i) {
				if (condition(L[i]))
					return i;
			}
		} else {
			for (size_t i = len-1; i != size_t(-1); --i) {
				if (condition(L[i]))
					return i;
			}
		}
		return notfound;
	}

	// return the first index of val in L
	// if reversed == true, find last instead of first
	size_t find(const T &val, bool reversed=false) const
	{
		return find_if(
			[val](const T &x){ return x == val; },
			reversed
		);
	}

	// return true iff L contains val
    bool contains(const T &val) const
	{
		return find(val) != notfound;
	}

	// ---- add & remove ----

	// add val to L as the last element
	void push_back(const T &val)
	{
		if (len == capacity)
			realloc();
		L[len++] = val;
	}

	// remove last element of L
	void pop_back()
	{
		if (len == 0)
			throw std::length_error("array is empty");
		--len;
	}

	// ---- indexing ----
	const T &operator[](size_t i) const
	{
		return L[i];
	}

	T &operator[](size_t i)
	{
		return const_cast<T&>(				// 3. 消除 const
			static_cast<const Array&>(*this)// 1. 把 Array 转为 const 版本
			[i]								// 2. 取下标
		);
	}

protected:
	static size_t valid_cap(size_t cap)
	{
		return cap > 1 ? cap : 1;
	}
};

template <class T>
std::ostream &operator<<(std::ostream &os, const Array<T> &rhs)
{
	os << '[';
	if (!rhs.empty()) {
		os << rhs[0];
		for (size_t i = 1; i < rhs.size(); ++i)
			os << ' ' << rhs[i];
	}
	return os << ']';
}

// a sorted array, suitable for set merge operations, etc
template <class T>
class SortedArray: public Array<T> {

	SortedArray(T *arr, size_t n): Array<T>(arr, n)
	{
		std::sort(this->L, this->L + this->len);
	}
	
	// merge: return a merged sorted array of lhs and rhs
	SortedArray(const SortedArray &lhs, const SortedArray &rhs):
		Array<T>(lhs.capacity+rhs.capacity)
	{
		size_t i = 0, j = 0;
		while (i < lhs.len && j < rhs.len) {
			if (lhs.L[i] > rhs.L[j])
				this->L[this->len++] = rhs.L[j++];
			else
				this->L[this->len++] = lhs.L[i++];
		}
		while (i < lhs.len)
			this->L[this->len++] = lhs.L[i++];
		while (j < rhs.len)
			this->L[this->len++] = rhs.L[j++];
	}

	// binary search
	// return the first index of val in L
	// return the index val should be inserted if not found
	//	[1, 3, 5].bsearch(0) = 0
	//	[1, 3, 5].bsearch(1) = 0
	//	[1, 3, 5].bsearch(4) = 2
	//	[1, 3, 5].bsearch(5) = 2
	//	[1, 3, 5].bsearch(6) = 3
	//	[1, 2, 2, 2, 3].bsearch(2) = 1
    size_t bsearch(const T &val) const
	{
        if (this->len == 0)
            return 0;
        size_t lo = 0;
        size_t hi = this->len-1;
        while (lo < hi) {
			// |_ (lo+hi)/2 _| may cause int overflow, furthermore,
			// if lo and hi are iterators, lo + hi is not possible
            size_t mid = lo + ((hi-lo) >> 1);
			if (val == this->L[mid]) {
				// replace following line with 'return mid;' if
				// there's no need to return first index of val
				hi = mid;
			} else if (val < this->L[mid]) {
				// following 2 lines are needed if using unsigned index
				if (mid == 0)
					break;
                hi = mid-1;
			} else {
                lo = mid+1;
			}
		}
		if (val <= this->L[lo])
			return lo;
		return lo+1;
	}

	// return the first index of val in L
	// return -1 if not found
	size_t find(const T &val) const
	{
		size_t i = bsearch(val);
		if (i < this->len && this->L[i] == val)
			return i;
		return this->notfound;
	}

	// return true iff there exists i, j such that
	// i != j and L[i] + L[j] == val
    bool two_sum(const T &val) const
	{
		if (this->len == 0)
			return false;
        size_t i = 0;
        size_t j = this->len-1;
        while (i < j) {
            if (this->L[i] + this->L[j] < val)
                ++i;
			else if (this->L[i] + this->L[j] > val)
                --j;
            else
                return true; // i, j
		}
        return false;
	}

	// insert val while L remains ordered
    void add(const T &val)
	{
		push_back(val);
		size_t i = this->len-1;
		while (i > 0 && this->L[i-1] > val) {
			this->L[i] = this->L[i-1];
			--i;
		}
		this->L[i] = val;
	}

	// remove val while L remains ordered
	// do nothing if val not in L
    void remove(const T &val)
	{
		size_t i = find(val);
        if (i != this->notfound) {
            while (i < this->len-1) {
                this->L[i] = this->L[i+1];
                ++i;
			}
			this->pop_back();
		}
	}
};

// a sorted array with distinct elements, can be used to represent set.
template <class T>
class SortedSet: public SortedArray<T> {

	// eliminate duplicates
	//! lacks efficiency
	SortedSet(const T *arr, size_t n): SortedArray<T>(arr, n)
	{
		if (this->len > 0) {
			size_t i = 0, j = 0;
			while (++j != this->len)
				if (this->L[i] != this->L[j])
					this->L[++i] = this->L[j];
			this->len = ++i;
		}
	}

	// merge: return a merged sorted set of lhs and rhs
	SortedSet(const SortedSet &lhs, const SortedSet &rhs):
		SortedArray<T>(lhs.capacity+rhs.capacity)
	{
		size_t i = 0, j = 0;
		while (i < lhs.len && j < rhs.len) {
			if (lhs.L[i] == rhs.L[j]) {
				this->L[this->len++] = lhs.L[i++];
				++j; // skip duplicate
			} else if (lhs.L[i] < rhs.L[j]) {
				this->L[this->len++] = lhs.L[i++];
			} else {
				this->L[this->len++] = rhs.L[j++];
			}
		}
		while (i < lhs.len)
			this->L[this->len++] = lhs.L[i++];
		while (j < rhs.len)
			this->L[this->len++] = rhs.L[j++];
	}

	// insert val while L remains ordered
    void add(const T &val)
	{
		// do nothing if L contains val
		if (!contains(val)) {
			push_back(val);
			size_t i = this->len-1;
			while (i > 0 && this->L[i-1] > val) {
				this->L[i] = this->L[i-1];
				--i;
			}
			this->L[i] = val;
		}
	}
};

#endif // ARRAY_H
