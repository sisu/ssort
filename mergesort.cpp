// -*- indent-tabs-mode: t -*-
#include <algorithm>
#include <cassert>
#include <cstring>
#include <utility>

#include "common.hpp"

void insertionSort(const uchar** arr, size_t n, int lcp, size_t *lcps);

namespace {

// Limits for reverting to insertion sort
const uint INS_LIMIT_MERGE = 16;


/**Compares suffixes of two 0-delimited strings.
 * @param s1 String to compare.
 * @param s2 Other string to compare.
 * @param start Starting index of the suffixes to compare.
 * @return Pair where the first member is the result of comparison
 *         (Negative number if s1 < s2. Positive if s1 > s2. Zero otherwise.)
 *         Second member is the length of the lcp.
 */
inline std::pair<int,int> lcpcmp(uchar *s1, uchar *s2, int start)
{
  uchar *begin = s1;
  s1 += start; s2 += start;
  while(*s1 && *s1 == *s2) {++s1; ++s2;}
  return std::make_pair(*s1 - *s2, s1 - begin);
}

} // empty namespace

void lcp_insertion_sort(uchar **begin, uchar **end, int *lcps) {
  if (begin >= end - 1) return;
  for(int j = 1; j < end - begin; ++j) {
    int i = j-1;
    uchar * cur = begin[j];
    std::pair<int, int> lcmp = lcpcmp(begin[j],begin[i], 0);
    while(i >= 0 && lcmp.first < 0) {
      std::swap(begin[i], begin[i+1]);
      std::swap(lcps[i], lcps[i+1]);
      if(i == 0) { 
        lcps[1] = lcmp.second; lcmp.second = 0;
      } else if(lcmp.second > lcps[i+1]) { 
        lcps[i] = lcpcmp(cur, begin[i-1], lcps[i+1]).second;
        break;
      } else if(lcmp.second == lcps[i+1]) {
        lcmp = lcpcmp(cur, begin[i-1], lcmp.second);
        lcps[i] = lcmp.second;
      }
      --i;
    }
    lcps[i+1] = lcmp.second;
  }
}

void lcp_insertion_sort(uchar **begin, uchar **end, uint lcp) {
  int *lcps = new int[end - begin];
  std::fill(lcps, lcps + (end - begin), lcp);
  lcp_insertion_sort(begin, end, lcps);
  delete [] lcps;
}

/**Merges two sorted 0-delimited strings arrays. Arrays must be in continuous
 * memory area. I.e. arrays are in range [begin..mid) and [mid..end).
 *
 * @param begin Start of the strings array.
 * @param end One past end of the strings array.
 * @param lcp Starting index of the suffixes.
 * @param temp_strings Temporary array for storing strings.
 * @param temp_lcps Temporary array for lcps.
 */
void string_merge(uchar **begin, uchar **mid, uchar **end, int *lcps,
									uchar **temp_strings, int *temp_lcps)
{
	int i = 0; const int m = mid - begin;
	int j = m; const int n = end - begin;;
	int curr = 0;
	while(i < m && j < n) {
		if(lcps[i] > lcps[j]) {
			temp_strings[curr] = begin[i];
			temp_lcps[curr++] = lcps[i++];
		} else if(lcps[i] < lcps[j]) {
			temp_strings[curr] = begin[j];
			temp_lcps[curr++] = lcps[j++];
		} else {
      std::pair<int,int> lcpp= lcpcmp(begin[i], begin[j], lcps[i]);
			if( lcpp.first < 0) {
				temp_strings[curr] = begin[i];
				temp_lcps[curr++] = lcps[i++];
				lcps[j] = lcpp.second;
			} else {
				temp_strings[curr] = begin[j];
				temp_lcps[curr++] = lcps[j++];
				lcps[i] = lcpp.second;
			}
		}
	}
	if(i < m) {
		assert(m - i == n - curr);
		std::copy(begin + i, begin + m, begin + curr);
		std::copy(lcps + i, lcps + m, lcps + curr);
	}
	/* At this point subarrays [lcps+curr..lcps+n) and [begin+curr..end)
	 * contain already the correct part of the final result */
	std::copy(temp_lcps, temp_lcps + curr, lcps);
	std::copy(temp_strings, temp_strings + curr, begin);

	assert(lcps[0] == 0);
}

void merge_sort(uchar **begin, uchar **end, int *lcps,
								uchar **temp_strings, int *temp_lcps)
{
	if(begin >= end - INS_LIMIT_MERGE) { 
		lcp_insertion_sort(begin, end, lcps);
		return;
	}
	int mid = (end - begin)/2;
	lcps[0] = lcps[mid] = 0;
	merge_sort(begin, begin + mid, lcps, temp_strings, temp_lcps);
	merge_sort(begin + mid, end, lcps + mid, temp_strings, temp_lcps);
	string_merge(begin, begin + mid, end, lcps, temp_strings, temp_lcps);
}

/* Continuous memory allocations are expensive in the case of
 * merge sort. For avoiding lots of memory allocations and deallocations
 * temporary arrays are passed as a parameters to actual sorting functions.
 */
void merge_sort(uchar **begin, size_t size)
{
  uchar **end = begin + size;
	uchar **temp_str = new uchar*[size];
	int *temp_lcps = new int[size];
	int *lcps = new int[size];
	std::fill(lcps, lcps + size, 0);
	merge_sort(begin, end, lcps, temp_str, temp_lcps);
	delete [] lcps; delete [] temp_str; delete [] temp_lcps;
}
