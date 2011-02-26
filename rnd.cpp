#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <tr1/random>
using namespace std;
int main(int argc, char* argv[])
{
	tr1::mt19937 rng;
	assert(argc==3);
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	while(n --> 0) {
		int kk = k;
		while(kk --> 0) {
			int c = 32 + rng() % (256-32);
			putchar(c);
		}
		putchar(10);
	}
}
