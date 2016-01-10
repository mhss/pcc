#include "suffix_array_manber.h"

// m = size of text
// alpha = alphabet
// build = is it to build suffix array, or to search ?
Manber::Manber(int m, int alphaSize, const char *st, bool build) {
	n = m;
	
	sa = (int*) malloc(n * sizeof(int));
	text = st;
	
	if(build) {
		// number os different ranks
		int r = max(alphaSize, n); // Position 0 is reserved to 'end of string'
		
		rank  = (int*) malloc( n      * sizeof(int));
		temp  = (int*) malloc( n      * sizeof(int));
		count = (int*) malloc((r + 1) * sizeof(int));
	}
}

void Manber::build_suffix_array(Alphabet &alpha) {
	// number os different ranks
	int r = max(alpha.size, n); // Position 0 is reserved to 'end of string'
	
	// Must be static
	assert(alpha.isStatic());
	
 	// Start
	for(int i = 0; i < n; ++i) {
		sa[i] = i;
		rank[i] = alpha.getIndex(text[i]) + 1;
	}
	
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
			
			temp[sa[i]] = r;
		}
		
		// Update array rank[]
		for(int i = 0; i < n; ++i)
			rank[i] = temp[i];
		
		// It's over :)
		if(r == n + 1)
			break;
		
		k <<= 1;
	}
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

inline int Manber::lcp(const char *a, const char *b, int start) {
	int i;
	for(i = start; a[i] && b[i] && a[i] == b[i]; ++i);
	return i;
}

int Manber::binary_search_left(const char *word) {
	int lcpl = lcp(word, text + sa[0]); // LCP Left
	
	// Full matching with suffix_array[0]
	if(! word[lcpl])
		return 0;
	
	// The first suffix is "greater than" word
	if(word[lcpl] < text[sa[0] + lcpl])
		return n;
	
	int lcpr = lcp(word, text + sa[n - 1]); // LCP Right
	
	// Worst suffix is "less than" word
	if(word[lcpr] && text[sa[n - 1] + lcpr] < word[lcpr])
		return n;
	
	// l = l-th suffix is "less or equals than" word
	// r = r-th suffix is "greater than" word
	int l = 0, r = n - 1;
	while(r - l > 1) {
		int mid = (l + r) >> 1;
		int lcpm = lcp(word, text + sa[mid], min(lcpl, lcpr)); // LCP Mid
		
		if(word[lcpm] <= text[sa[mid] + lcpm])
			r = mid, lcpr = lcpm;
		else
			l = mid + 1, lcpl = lcp(word, text + sa[mid + 1], min(lcpm, lcpr));
	}
	
	// is l-th good ?
	if(! word[lcpl]) return l;

	// is r-th good ?
	if(! word[lcpr]) return r;
	
	// There isn't good suffix to word
	return n;
}

int Manber::binary_search_right(const char *word) {
	int lcpr = lcp(word, text + sa[n - 1]); // LCP Right
	
	// Full matching with suffix_array[n - 1]
	if(! word[lcpr])
		return n - 1;
	
	// Worst suffix is "less than" word
	if(word[lcpr] && text[sa[n - 1] + lcpr] < word[lcpr])
		return -1;
	
	int lcpl = lcp(word, text + sa[0]); // LCP Left
	
	// The first suffix is "greater than" word
	if(word[lcpl] && word[lcpl] < text[sa[0] + lcpl])
		return -1;
	
	// l = l-th suffix is "less than" word
	// r = r-th suffix is "greater or equals than" word
	int l = 0, r = n - 1;
	while(r - l > 1) {
		int mid = (l + r) >> 1;
		int lcpm = lcp(word, text + sa[mid], min(lcpl, lcpr)); // LCP Mid
		
		if(word[lcpm] && word[lcpm] < text[sa[mid] + lcpm])
			r = mid - 1, lcpr = lcp(word, text + sa[mid - 1], min(lcpl, lcpm));
		else
			l = mid, lcpl = lcpm;
	}
	
	// is r-th good ?
	if(! word[lcpr]) return r;
	
	// is l-th good ?
	if(! word[lcpl]) return l;
	
	// There isn't good suffix to word
	return -1;
}

vector<int> Manber::search(const char *word) {
	vector<int> result;
	int l = binary_search_left(word);
	int r = binary_search_right(word);
	
	if(l <= r) {
		result.resize(r - l + 1);
		for(int i = l; i <= r; ++i)
			result[i - l] = sa[i];
	}
	
	// Utilizando estrutura declarada no suffix_tree para guardar as ocorrências
	return result;
}

void Manber::do_search(const char *word) {
	int l = binary_search_left(word);
	int r = binary_search_right(word);
	
	// Routine suffix tree-like to find lines
	for(int i = l; i <= r; ++i) {
		int line = std::upper_bound(line_end, line_end + lines, sa[i]) - line_end;
		if (line < lines)
			occ.insert(line);
	}
}

void Manber::clear() {
	free(sa);
	if(rank) free(rank);
	if(temp) free(temp);
	if(count) free(count);
}


