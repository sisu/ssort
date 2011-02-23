// -*- indent-tabs-mode: t -*-
#pragma once
#include <cstddef>
const size_t ISORT_SIZE = 16;
void insertionSort(const uchar** arr, size_t n, int lcp, size_t *lcps);
void insertionSort(const uchar** arr, size_t n, int lcp);
void insertionSort(const uchar** arr, size_t n);
void insertionSort(uchar** arr, size_t n);
