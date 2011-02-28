// -*- indent-tabs-mode: t -*-
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cassert>
#include <cstring>
#include <algorithm>
#include "common.hpp"
using namespace std;

void burstsort(uchar** arr, size_t n);
void sqsort(uchar** arr, size_t n);
void insertionSort(uchar** arr, size_t n);
void msd_radixsort(uchar** arr, size_t n);
void msd_radixsort_super(uchar** arr, size_t n);
void merge_sort(uchar** arr, size_t n);

uchar* readfile(const char* name, size_t* plen) {
	ifstream in(name);
	assert(in);
	in.seekg(0, ios::end);
	size_t len = in.tellg();
	in.seekg(0, ios::beg);
	uchar* buf = new uchar[len+1];
	in.read((char*)buf, len);
	*plen = len;
	buf[len] = 0;
	return buf;
}
uchar** splitString(uchar* str, size_t len, size_t* pcnt) {
	size_t mcnt=0;
	for(size_t i=0; i<len; ++i) mcnt += str[i]=='\n';
	uchar** ptrs = new uchar*[mcnt+1];

	uchar* cur = str;
	size_t cnt=0;
	while(1) {
//		while(cur<str+len && *cur=='\n') ++cur;
		if (cur>=str+len) break;
		ptrs[cnt++] = cur;
		uchar* end = (uchar*)strchrnul((char*)cur, '\n');
		*end = 0;
		cur = end+1;
	}
	*pcnt = cnt;
	return ptrs;
}

void stdsort(uchar** arr, size_t n) {
	sort(arr, arr+n, cmp);
}

struct Algo {
	void(*sort)(uchar**, size_t);
	const char* sname;
	const char* name;
} algos[] = {
	{burstsort, "bs", "burstsort"},
	{sqsort, "qs", "string quicksort"},
	{msd_radixsort_super, "rss", "msd radixsort with super alphabet"},
	{msd_radixsort, "rs", "msd radixsort"},
	{merge_sort, "ms", "merge sort"},
	{stdsort, "std", "std::sort"}
//	{insertionSort, "is", "insertion sort"}
};
int main(int argc, char* argv[])
{
	if (argc<=1) {
		cout<<"Usage: "<<argv[0]<<" [<algorith>] filename [filename2 ...]\n";
		cout<<"<algorithm> can be one of:\n";
		for(size_t i=0; i<sizeof(algos)/sizeof(algos[0]); ++i) {
			Algo a = algos[i];
			cout<<"\t"<<a.sname<<"\t - "<<a.name<<'\n';
		}
	}
	Algo algo = algos[0];
	bool suffix=0;
	for(int i=1; i<argc; ++i) {
		bool change=0;
		for(size_t j=0; j<sizeof(algos)/sizeof(algos[0]); ++j) {
			Algo a = algos[j];
			if (!strcmp(argv[i], a.sname)) {
				algo = a;
				change=1;
				break;
			}
		}
		if (!strcmp(argv[i], "s")) {
			suffix=1;
			change=1;
		}
		if (change) continue;

		size_t len;
		uchar* buf = readfile(argv[i], &len);
		size_t cnt;
		uchar** ptrs;
		if (suffix) {
			cnt = len;
			ptrs = new uchar*[len];
			for(size_t i=0; i<len; ++i)
				ptrs[i] = buf;
		} else {
			ptrs = splitString(buf, len, &cnt);
		}
//		for(size_t i=0; i<cnt; ++i) cout<<ptrs[i]<<'\n';

		clock_t start = clock();
		algo.sort(ptrs, cnt);
		clock_t end = clock();

		cout<<fixed<<"Sorted "<<cnt<<' '<<(suffix?"suffixes":"strings")<<" in time "
				<<double(end-start)/CLOCKS_PER_SEC<<" with "<<algo.name<<'\n';
//		cout<<start<<' '<<end<<'\n';

//		for(size_t i=0; i<cnt; ++i) cout<<ptrs[i]<<'\n';

		size_t dp=0;
		size_t prevd=0;
		for(size_t i=1; i<cnt; ++i) {
			size_t l=0;
			if (lcpCmp(ptrs[i],ptrs[i-1], &l)) {
				cout<<"sorting failed at position "<<i<<'\n';
				for(size_t j=max(i,size_t(5))-5; j<min(i+5,cnt); ++j)
					cout<<j<<' '<<ptrs[j]<<'\n';
				abort();
			}
			dp += max(l, prevd);
			if (strlen((char*)ptrs[i-1])!=l) ++dp;
			prevd = l;
		}
		dp += prevd;
		if (strlen((char*)ptrs[cnt-1])!=prevd) ++dp;
		cout<<"dp: "<<dp<<'\n';

		delete[] buf;
		delete[] ptrs;
	}
}
