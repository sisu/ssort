#include <algorithm>
#include <cassert>
#include <cstring>
#include "Vector.hpp"
#include "common.hpp"
#include "sqs.hpp"
using namespace std;
typedef unsigned char uchar;

/** Size of the alphabet used in the burst trie. */
int ASIZE = 256;

/** Contains a node of a burst trie of strings.
 * A node contains either a bucket of strings 
 */
struct BurstTrie {
	/** Pointers to the child nodes of this node. NULL if this node
	 * hasn't been burst yet. */
	BurstTrie* children;
	/** The bucket of strings added to this node. NULL if this node
	 * has been burst. */
	Vector<const uchar*, 16, 8> bucket;
	/** Pointers to strings ending at this node. */
	Vector<const uchar*, 8, 2> ends;

	/** Adds a new string to the trie. */
	void insert(const uchar* s) {
		if (!*s) {
			ends.add(s);
		} else if (children) {
			children[(int)s[0]].insert(s+1);
		} else {
			bucket.add(s);
			if (bucket.size() >= 8192) burst();
		}
	}
	void burst() {
		children = new BurstTrie[ASIZE];
		for(size_t i=0; i<bucket.size(); ++i) {
			const uchar* s = bucket[i];
			children[(int)s[0]].insert(s+1);
		}
		bucket.clear();
	}
	/** Writes the strings in this subtree to the given array sorted. */
	void output(const uchar** out) {
		output(out, 0);
	}
	/** Writes the strings in this subtree to the given array sorted.
	 *
	 * @param out   output array to store the sorted strings to
	 * @param level current depth in the trie
	 * @return      first element of out where we haven't written
	 */
	const uchar** output(const uchar** out, size_t level) {
		for(size_t i=0; i<ends.size(); ++i)
			*out++ = ends[i] - level;
		if (children) {
			for(int i=0; i<ASIZE; ++i) {
				out = children[i].output(out, level+1);
			}
		} else {
			const uchar** out0 = out;
			for(size_t i=0; i<bucket.size(); ++i) {
				*out++ = bucket[i];
			}
//			sort(out0, out, cmp);
			sqsort(out0, out-out0);
			for(const uchar** i=out0; i!=out; ++i) *i-= level;
		}
		return out;
	}

	BurstTrie(): children(0) {}
	~BurstTrie() {
		delete[] children;
	}
};

/** Sorts an array of strings using burstsort. */
void burstsort(uchar** arr, size_t n) {
	BurstTrie t;
	for(size_t i=0; i<n; ++i) {
		t.insert(arr[i]);
	}
	t.output((const uchar**)arr);
}
