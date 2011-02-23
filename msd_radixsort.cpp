// -*- indent-tabs-mode: t -*-
#include <algorithm>

#include "common.hpp"

void sqsort(const uchar** arr, size_t n, size_t lcp);

namespace {

// Limits for reverting to qsort/normal alphabet
const uint RADIX_LIMIT = 100;
const uint SUPER_ALPHABET_LIMIT = 1 << 14;

} // empty namespace

namespace {
/**Distributes string pointers to buckets. Common part of
 * the radixsort algorithms for different size of alphabets.
 *
 * @param begin Start of the range of the strings.
 * @param size Size of the range.
 * @param sorted Helper array for bucket sort. Must have at least length of
 *							 size.
 * @param oracle Oracle-array for msd radixsort.
 * @param bucket_size Array where the sizes of the buckets are going to
 *										be stored.
 * @param bucket_index Array where the starting indices of the buckets are
 *										 going to be stored.
 * @param buckets Number of buckets.
 */
template <typename T>
inline void msd_distr_to_buckets(uchar **begin, uint size,
																 uchar *__restrict__* sorted,
																 T *__restrict__ oracle,
																 uint * __restrict__ bucket_size,
																 int *__restrict__ bucket_index, uint buckets)
{
	std::copy(begin, begin + size, sorted);
	for(uint i = 0; i < size; ++i)
		++bucket_size[oracle[i]];
	bucket_index[0] = 0;
	for(uint i = 1; i < buckets; i++)
			bucket_index[i] = bucket_index[i-1] + bucket_size[i-1];
	for(uint i = 0; i < size; ++i) {
		begin[bucket_index[oracle[i]]++] = sorted[i];
	}
}
uchar **sorted;
ushort *oracle2;
uchar *oracle;
} //empty namespace

void msd_radixsort(uchar ** begin, size_t size, uint lcp)
{
	if( size <= RADIX_LIMIT) {
		sqsort((const uchar**)begin, size, lcp);
		return;
	}
	uint bucket_size[0x100] = {0};
	for(uint i = 0; i < size; ++i)
		oracle[i] = begin[i][lcp];
	static int bucket_index[0x100];

	msd_distr_to_buckets(begin, size, sorted, oracle, bucket_size, bucket_index,
											 0x100);

	int bsum = bucket_size[0];
	++lcp;
	for(int i = 1; i < 0x100; ++i) {
		if(bucket_size[i] == 0) continue;
		msd_radixsort(begin + bsum, bucket_size[i], lcp);
		bsum += bucket_size[i];
	}
}

void msd_radixsort2(uchar **begin, size_t size, uint lcp)
{
	uint *__restrict__ bucket_size = new uint[0x10000];
	std::fill(bucket_size, bucket_size + 0x10000, 0);
	for(uint i = 0; i < size; ++i) {
		ushort pair = begin[i][lcp];
		pair <<= 8;
		if (pair != 0) pair |=	begin[i][lcp+1];
		
		oracle2[i] = pair;
	}
	static int bucket_index[0x10000];

	msd_distr_to_buckets(begin, size, sorted, oracle2, bucket_size, bucket_index,
											 0x10000);

	lcp += 2;
	int bsum = bucket_size[0];
	for(uint i = 1; i < 0x10000; ++i) {
		if(bucket_size[i] == 0) continue;
		else if((i&0x00ff) != 0x00 && bucket_size[i] < SUPER_ALPHABET_LIMIT)
			msd_radixsort(begin + bsum, bucket_size[i], lcp);
		else if((i&0x00ff) != 0x00)
			msd_radixsort2(begin + bsum, bucket_size[i], lcp);
		bsum += bucket_size[i];
	}
	delete[] bucket_size;
}

void msd_radixsort(uchar **begin, size_t size) {
	sorted = new uchar*[size];
	oracle = new uchar[size];
	oracle2 = new ushort[size];
	if(size >= SUPER_ALPHABET_LIMIT) msd_radixsort2(begin, size, 0);
	else msd_radixsort(begin, size, 0);

	delete[] sorted;
	delete[] oracle;
	delete[] oracle2;
}
