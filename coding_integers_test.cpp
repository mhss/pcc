#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cassert>

#include <string>

using namespace std;

// Integer to String
string inttostr(int x) {
	if(x == 0)
		return "0";
	
	string r = "";
	while(x > 0) {
		if(x & 1) r = "1" + r;
		else r = "0" + r;
		x >>= 1;
	}

	return r;
}

// Elias Omega Encoding: from Wikipedia (https://en.wikipedia.org/wiki/Elias_omega_coding)
string _elias_omega_encoding(int x) {
//	printf("< x %d\n", x);
	string r = inttostr(x);
	if(x <= 1) return r;
	return _elias_omega_encoding(r.length() - 1) + r;
}

string elias_omega_encoding(int x) { // +1
	return _elias_omega_encoding(x + 1) + "0";
}

int elias_omega_decoding(string s) {
	int w = 1;
	int i = 0;
	while(w == 1 || s[i] != '0') {
		int r = 0;
		for(int j = 0; j < w; ++j)
			r = (r << 1) | (s[i + j] - '0');
		
		i += w;
		w = r + 1;
	}
	
	w -= 1; // Fixing the increment
	w -= 1; // (-1)
	
	return w;
}

// Paguso Encoding: from aulas
string _paguso_encoding(string x) {
	if(x.length() <= 1) return x;
	return _paguso_encoding(inttostr(x.length() - 2)) + x;
}
string paguso_encoding(int x) {
	return _paguso_encoding("1" + inttostr(x)) + "0";
}

int paguso_decoding(string s) {
	int w = 1, oldW;
	int i = 0;
	while(w == 1 || s[i] != '0') {
		int r = 0;
		for(int j = 0; j < w; ++j)
			r = (r << 1) | (s[i + j] - '0');
		
		i += w;
		oldW = w;
		w = r + 2;
	}
	
	w -= 2; // Fixing the increment
	w &= (1 << (oldW - 1)) - 1;
	
	return w;
}

// Elias Gamma Encoding: from Wikipedia (https://en.wikipedia.org/wiki/Elias_gamma_coding)
string elias_gamma_encoding(int x) { // +1
	x += 1;
	string r = "";
	int z = 1;
	while(x >= (1 << z))
		r += '0', ++z;
	return r + inttostr(x);
}

int elias_gamma_decoding(string s) { // -1
	int i = 0;
	while(s[i] == '0')
		++i;
	
	int size = 1 + i;
	int r = 0;
	for(int j = 0; j < size; ++j)
		r = (r << 1) | (s[i + j] - '0');
	
	return r - 1;
}

// Fibonacci
const int FIB_N = 45;
int fib[FIB_N];
void calc_fib() {
	int a = 1, b = 1;
	for(int i = 0; i < FIB_N; ++i) {
		fib[i] = b;
//		printf("%d\n", b);
		a += b;
		swap(a, b);
	}
}

// Fibonacci Coding: from Wikipedia (https://en.wikipedia.org/wiki/Fibonacci_coding)
string fibonacci_encoding(int x) { // +1
	x += 1;
	string r = "1";
	int i = FIB_N - 1;
	while(fib[i] > x) --i;
//	printf(" i [%d] = %d\n", i, fib[i]);
	while(i >= 0) {
		if(x >= fib[i]) {
			x -= fib[i];
			r = "1" + r;
		} else r = "0" + r;
		
		--i;
	}
	
	return r;
}

int fibonacci_decoding(string s) { // -1
	int r = 0;
	for(int i = 0; i < (int) s.length() - 1; ++i)
		if(s[i] == '1') r += fib[i];
	
	return r - 1;
}


void compareEncodings() {
	//for(int i = 3000000; i < 4000000; ++i) {
	for(int i = 0; i < 2000; ++i) {
		string eo = elias_omega_encoding(i);
		string p = paguso_encoding(i);
		string eg = elias_gamma_encoding(i);
		string f = fibonacci_encoding(i);
		
		//printf("%d\n", i);
		//assert(p.length() >= f.length());
		//printf("%5d%15s%35s (%2d)%35s (%2d)%35s (%2d)\t%lf\n", i + 1, inttostr(i + 1).c_str(),
		printf("%35s (%2d)", eo.c_str(), (int) eo.length());
		printf("%35s (%2d)", p.c_str(), (int)  p.length());
		printf("%35s (%2d)", eg.c_str(), (int) eg.length());
		printf("%35s (%2d)", f.c_str(), (int)  f.length());
		//printf("\t%lf\n", (i ? log(i) : 0) / log(2));
		puts("");
	}
	
/*/
	paguso_encoding x fibonacci_encoding: |fibonacci| <= |paguso| até n = 3524576.
3524574
3524575
3524576
3524577
main: coding_integers.cpp:99: int main(): Assertion `p.length() >= f.length()' failed.
Aborted (core dumped)
ibfs@ibfs-virtualBox:~/Desktop/Processamento de Cadeias/ipmt$ 
//*/
}



void checkEliasOmega() {
	for(int i = 0; i < 1000000; ++i) {
		string s = elias_omega_encoding(i);
		int j = elias_omega_decoding(s);
//		printf(" < i %d, s '%s', j %d\n", i, s.c_str(), j);
		assert(i == j);
	}
}

void checkPaguso() {
	for(int i = 0; i < 1000000; ++i) {
		string s = paguso_encoding(i);
		int j = paguso_decoding(s);
//		printf(" < i %d, s '%s', j %d\n", i, s.c_str(), j);
		assert(i == j);
	}
}

void checkEliasGamma() {
	for(int i = 0; i < 1000000; ++i) {
		string s = elias_gamma_encoding(i);
		int j = elias_gamma_decoding(s);
		printf(" < i %d, s '%s', j %d\n", i, s.c_str(), j);
		assert(i == j);
	}
}

void checkFibonacci() {
	for(int i = 0; i < 1000000; ++i) {
		string s = fibonacci_encoding(i);
		int j = fibonacci_decoding(s);
		assert(i == j);
	}
}

void statistics(const char* filename, string function(int), int N) {
	FILE *file = fopen(filename, "w");
	
	for(int i = 0; i < N; ++i) {
		string s = function(i);
		fprintf(file, "%d\n", (int) s.length());
	}
	
	fclose(file);
}

void statistics() {
	int N = 10000000;
	statistics("coding_elias_omega.txt", elias_omega_encoding, N);
	statistics("coding_paguso.txt", paguso_encoding, N);
	statistics("coding_elias_gamma.txt", elias_gamma_encoding, N);
	statistics("coding_fibonacci.txt", fibonacci_encoding, N);
}

int main() {
	calc_fib();
	
	//checkEliasOmega();
	//checkPaguso();
	//checkEliasGamma();
	//checkFibonacci();
	//compareEncodings();
	//statistics();
	
	return 0;
}

