#include "common.hpp"
#include "insertion.hpp"
#include <iostream>
#include <cassert>
using namespace std;

#if 1

/** Sorts an array using LCP insertion sort.
 * Calling this function is only valid for arrays smaller than ISORT_SIZE.
 *
 * @param arr  the strings to be sorted
 * @param n    number of strings in arr
 * @param lcp  known lcp between all strings between arr
 */
void insertionSort(const uchar** arr, size_t n, int lcp) {
	size_t lcps[ISORT_SIZE];
	assert(n < ISORT_SIZE);
	for(size_t i=0; i<n; ++i) {
		const uchar* cur = arr[i];
//		cout<<"inserting "<<cur<<'\n';
		size_t l = lcp;
		size_t j;
		for(j=i; j; --j) {
			size_t pl = l;
			if (lcps[j-1]<l || !lcpCmp(cur, arr[j-1], &l)) {
//				cout<<"breaking: "<<lcps[j-1]<<' '<<l<<' '<<arr[j-1]<<'\n';
				if (lcps[j-1]>=pl) lcps[j-1] = l;
				l=pl;
				assert(!cmp(cur, arr[j-1]));
				break;
			}
//			cout<<"pushing before "<<l<<" : "<<arr[j-1]<<'\n';
			assert(cmp(cur, arr[j-1]));
			arr[j] = arr[j-1];
			lcps[j] = lcps[j-1];
		}
//		cout<<"pos: "<<j<<" ; "<<l<<'\n';
		assert(j==i || cmp(cur, arr[j+1]));
		assert(j==0 || !cmp(cur, arr[j-1]));
		arr[j] = cur;
		lcps[j] = l;
	}
}

#else
/** Sorts an array using standard (non-lcp) insertion sort. */
void insertionSort(const uchar** arr, size_t n, int lcp) {
	for(size_t i=0; i<n; ++i) {
		const uchar* cur = arr[i];
		size_t j;
		for(j=i; j; --j) {
			if (!cmp(cur+lcp, arr[j-1]+lcp)) {
				break;
			}
			arr[j] = arr[j-1];
		}
		arr[j] = cur;
	}
}
#endif

void insertionSort(const uchar** arr, size_t n) {
	return insertionSort(arr, n, 0);
}
/** Sorts the array using LCP insertion sort. */
void insertionSort(uchar** arr, size_t n) {
	insertionSort((const uchar**)arr, n, 0);
}
