#ifndef _SUFFIX_ARRAY_MANBER_
#define _SUFFIX_ARRAY_MANBER_

#include <cstdio>
#include <algorithm>

#include "alphabet_index.h"

struct Manber {
	int *sa; // Suffix Array
	int *rank; // Rank
	int *temp; // Temporary array (temporary rank or temporary suffix array
	int n;
	
	int *count; // Counter to countingSort
	
	Manber(const char *st, int m, Alphabet &alpha);
	inline int getRank(int idx);
	void countingSort(int k, int sz);
	
	void clear();
};

#endif
