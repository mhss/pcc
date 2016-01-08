#include "coding_integers.h"

// Fibonacci Coding: from Wikipedia (https://en.wikipedia.org/wiki/Fibonacci_coding)
string fibonacci_encoding_str(int x) { // +1
	x += 1;
	string r = "1";
	int i = FIB_N - 1;
	while(fib[i] > x) --i;
	
	while(i >= 0) {
		if(x >= fib[i]) {
			x -= fib[i];
			r = "1" + r;
		} else r = "0" + r;
		
		--i;
	}
	
	return r;
}

int fibonacci_decoding_str(string s) { // -1
	int r = 0;
	for(int i = 0; i < (int) s.length() - 1; ++i)
		if(s[i] == '1')
			r += fib[i];
	
	return r - 1;
}

long long fibonacci_encoding(int x) { // +1
	x += 1;
	long long r = 1;
	int i = FIB_N - 1;
	while(fib[i] > x) --i;
	
	while(i >= 0) {
		r <<= 1;
		if(x >= fib[i]) {
			x -= fib[i];
			r |= 1;
		}
		
		--i;
	}
	
	return r;
}

int fibonacci_decoding(long long x) { // -1
	int r = 0;
	for(int i = 0, one = 0; x; ++i) {
		if((x & 1) && one) break;
		one = x & 1;
		if(one)
			r += fib[i];
		
		x >>= 1;
	}
	
	return r - 1;
}

void checkFibonacci() {
	for(int i = 0; i < 1000000; ++i) {
		string s = fibonacci_encoding_str(i);
		int j = fibonacci_decoding_str(s);
		assert(i == j);
	}
}

