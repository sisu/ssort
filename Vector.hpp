// -*- indent-tabs-mode: t -*-
#pragma once

#include <cstdlib>

template<class T, size_t Start, size_t Grow>
struct Vector {
	Vector(): arr(0), csize(0), msize(0) {}
	~Vector() {
		clear();
	}

	void add(const T& t) {
		if (!arr) {
			msize = Start;
			arr = (T*)malloc(msize * sizeof(T));
		} else if (csize==msize) {
			msize *= Grow;
			arr = (T*)realloc(arr, msize * sizeof(T));
		}
		arr[csize++] = t;
	}
	size_t size() const {
		return csize;
	}
	T& operator[](size_t i) {
		return arr[i];
	}
	void clear() {
		free(arr);
		arr=0;
		csize=msize=0;
	}

	T* arr;
	size_t csize;
	size_t msize;
};

