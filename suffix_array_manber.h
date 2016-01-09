#ifndef _SUFFIX_ARRAY_MANBER_
#define _SUFFIX_ARRAY_MANBER_

#include <cstdio>
#include <algorithm>
#include <vector>

struct Manber;

#include "alphabet_index.h"
#include "suffix_tree.h"

struct Manber {
	int *sa; // Suffix Array
	int *rank; // Rank
	int *temp; // Temporary array (temporary rank or temporary suffix array
	int n;
	
	int *count; // Counter to countingSort
	
	const char *text;
	
	Manber(int m, int alphaSize, const char *st, bool build);
	
	inline int getRank(int idx);
	void countingSort(int k, int sz);
	void build_suffix_array(Alphabet &alpha);
	
	//void raea(int l, int lcpl, int r, int lcpr, int mid, int lcpm);
	inline int lcp(const char *a, const char *b, int start = 0);
	int binary_search_left(const char *word);
	int binary_search_right(const char *word);
	vector<int> search(const char *word);
	void do_search(const char *word);
	
	void clear();
};

#endif
