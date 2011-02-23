#include "common.hpp"
#include "insertion.hpp"
#include <iostream>
#include <cassert>
using namespace std;

/** Partitions an array to elements smaller, equal and greater to given pivot.
 *
 * @param arr  the array to partition
 * @param n    number of elements in arr
 * @param p    the pivot for which we do the partition
 * @param l    index of the character in the strings which we compare to p
 * @param resA pointer where we store the number of elements less than p
 * @param resB pointer where we store the number of elements <= p
 */
void partition3(const uchar** arr, size_t n, uchar p, int l, size_t* resA, size_t* resB) {
	size_t a=0;
	while(a<n && arr[a][l]<p) ++a;
	size_t b=a;
//	while(b<n && arr[b][l]==p) ++b;
	while(b<n) {
		if (arr[b][l]<p) {
			swap(arr[a++], arr[b++]);
		} else if (arr[b][l]==p) {
			++b;
		} else {
			break;
		}
	}
	if (b==n) {
		*resA = a;
		*resB = b;
		return;
	}
	size_t c = n-1;
	while(c>=0 && arr[c][l] > p) --c;
	while(b <= c) {
		assert(arr[b][l] > p);
		assert(arr[c][l] <= p);

		if (arr[c][l]==p) {
			swap(arr[b++], arr[c--]);
		} else if (a==b) {
			swap(arr[a++], arr[c--]);
			b=a;
		} else {
			const uchar* tmp = arr[a];
			arr[a] = arr[c];
			arr[c] = arr[b];
			arr[b] = tmp;
			++a,++b,--c;
		}

		while(1) {
			if (arr[b][l]==p) {
				++b;
			} else if (arr[b][l]<p) {
				swap(arr[a++], arr[b++]);
			} else {
				break;
			}
		}
		while(arr[c][l]>p) --c;
	}
	*resA = a;
	*resB = b;
}

/** Chooses the pivot element to be used in partitioning.
 * @param arr  array from which the pivot is chosen
 * @param n    number of elements in arr
 * @param lcp  index of the character which we are looking at in the strings
 * @return     index of the pivot element in arr
 */
size_t choosePivot(const uchar** arr, size_t n, size_t lcp) {
	const int M = 3;
	size_t pos[M];
	for(int i=0; i<M; ++i)
		pos[i] = i*n/(M-1);
	--pos[M-1];
	for(int i=1; i<M; ++i) {
		int j;
		size_t p = pos[i];
		uchar c = arr[p][lcp];
		for(j=i; j; --j) {
			if (arr[pos[j-1]][lcp] <= c) {
				break;
			}
			pos[j] = pos[j-1];
		}
		pos[j] = p;
	}
	return pos[M/2];
}

/** Recursive multikey quicksort.
 *
 * @param arr   the array to be sorted
 * @param n     number of elements in arr
 * @param lcp   index of the current key
 */
void sqsort(const uchar** arr, size_t n, size_t lcp) {
	if (n<=ISORT_SIZE) {
		insertionSort(arr, n, lcp);
		return;
	}
	if (n<=1) return;
	uchar p;
	while(1) {
		int x = choosePivot(arr, n, lcp);
		p = arr[x][lcp];
		if (p) break;
		swap(arr[0], arr[x]);
		++arr;
		--n;
		if (n<=1) return;
	}
	size_t a, b;
	partition3(arr, n, p, lcp, &a, &b);
	sqsort(arr, a, lcp);
	sqsort(arr+a, b-a, lcp+1);
	sqsort(arr+b, n-b, lcp);
}

/** Sorts the array using string quick sort. */
void sqsort(const uchar** arr, size_t n) {
	sqsort(arr, n, 0);
}
/** Sorts the array using string quick sort. */
void sqsort(uchar** arr, size_t n) {
	sqsort((const uchar**)arr, n);
}
