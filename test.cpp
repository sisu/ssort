#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <tr1/random>
#include "common.hpp"
using namespace std;

tr1::mt19937 rng;

void burstsort(uchar** arr, size_t n);
void sqsort(uchar** arr, size_t n);

uchar* randomStr(int n, int from='a', int to='z'+1) {
	uchar* s = new uchar[n+1];
	for(int i=0; i<n; ++i) {
		s[i] = from + rng()%(from-to+1);
	}
	s[n] = 0;
	return s;
}

void testSorted(uchar** strs, int n) {
	for(int i=1; i<n; ++i) {
		if (cmp(strs[i], strs[i-1])) {
			ostringstream os;
			os<<"Strings not sorted at position "<<i<<'\n';
			throw logic_error(os.str());
		}
	}
}

void testRandomStrings() {
	typedef void(*Alg)(uchar**, size_t);
	Alg algos[] = {burstsort, sqsort};
	int sizes[] = {1, 10, 100, 1000};
	for(size_t i=0; i<sizeof(sizes)/sizeof(sizes[0]); ++i) {
		int n = sizes[i];
		uchar** strs = new uchar*[n];
		for(int j=0; j<n; ++j) strs[j] = randomStr(n);
		for(size_t j=0; j<sizeof(algos)/sizeof(algos[0]); ++j) {
			cout<<"Testing sorting of "<<n<<" strings with algo "<<j<<'\n';
			uchar** ss = new uchar*[n];
			for(int k=0; k<n; ++k) ss[k] = strs[k];
			algos[j](ss, n);
			testSorted(ss, n);
			delete[] ss;
		}
		for(int j=0; j<n; ++j) delete[] strs[j];
		delete[] strs;
	}
}

int main()
{
	try {
		testRandomStrings();
		cout<<"All tests OK\n";
	} catch(const exception& e) {
		cout<<"Test failed: "<<e.what()<<'\n';
	}
}
