#pragma once
#include <cstddef>

typedef unsigned char uchar;

inline bool cmp(const uchar* a, const uchar* b) {
	size_t x=0;
	while(a[x]==b[x] && a[x]) ++x;
	return a[x]<b[x];
}

inline bool lcpCmp(const uchar* a, const uchar* b, size_t* lcp) {
	int x = *lcp;
	while(a[x] && a[x]==b[x]) ++x;
	*lcp = x;
	return a[x]<b[x];
}
