#include "suffix_array_manber.h"

Manber::Manber(const char *st, int m, Alphabet &alpha) {
	n = m;
	
	// number os different ranks
	int r = max(alpha.size, n); // Position 0 is reserved to 'end of string'
	
	sa    = (int*) malloc( n      * sizeof(int));
	rank  = (int*) malloc( n      * sizeof(int));
	temp  = (int*) malloc( n      * sizeof(int));
	count = (int*) malloc((r + 1) * sizeof(int));
	
	// Must be static
	assert(alpha.isStatic());
	
 	// Start
	for(int i = 0; i < n; ++i) {
		sa[i] = i;
		rank[i] = alpha.getIndex(st[i]) + 1;
	}

	/*
	printf("k = -1\n");
	for(int i = 0; i < n; ++i)
		printf(" %2d: (%2d, %2d)\n", sa[i], getRank(sa[i]), getRank(sa[i]));
	puts("");
	//*/
	
	// Suffix size = 1 ..
	int k = 1;
	while(k < n) {
		countingSort(k, r);
		countingSort(0, r);
		
		// Calculate new ranks
		r = 1;
		temp[sa[0]] = r;
		for(int i = 1; i < n; ++i) {
			if(getRank(sa[i - 1]) != getRank(sa[i]) || getRank(sa[i - 1] + k) != getRank(sa[i] + k))
				++r;
			
//			printf(" [%2d, %2d] (%2d, %2d) :: %d\n", sa[i], sa[i] + k, getRank(sa[i]), getRank(sa[i] + k), r);
			temp[sa[i]] = r;
		}
		
//		puts("");
		
		// Update array rank[]
		for(int i = 0; i < n; ++i)
			rank[i] = temp[i];
		
		/*/ TODO Debug
		printf("k = %d\n", k);
		for(int i = 0; i < n; ++i)
			printf(" %2d: %2d (%2d, %2d)\n", sa[i], rank[i], getRank(sa[i]), getRank(sa[i] + (k << 0)));
		puts("");
		//*/
		
		// It's over :)
		if(r == n + 1)
			break;
		
//		if(k >= 1) break;
		k <<= 1;
	}
	
	// TODO Debug
	for(int i = 0; i < n; ++i)
		printf(" %2d: %s\n", sa[i], st + sa[i]);

}

inline int Manber::getRank(int idx) {
	return (idx < n) ? rank[idx] : 0;
}

void Manber::countingSort(int k, int sz) {
	for(int i = 0; i <= sz; ++i)
		count[i] = 0;
	
	// Calculate the frequency
	for(int i = 0; i < n; ++i)
		count[ getRank(sa[i] + k) ]++;
	
	// Cumulative counter
	int sum = 0;
	for(int i = 0; i <= sz; ++i) {
		int j = count[i];
		count[i] = sum;
		sum += j;
	}
	
	// Using temporary array as temporary suffix
	for(int i = 0; i < n; ++i) {
		int idx = sa[i] + k;
		int r = getRank(idx);
		
		temp[ count[r] ] = sa[i];
		count[r]++;
	}
	
	// Update suffix array
	for(int i = 0; i < n; ++i)
		sa[i] = temp[i];
}

void Manber::clear() {
	free(sa);
	free(rank);
	free(temp);
	free(count);
}


