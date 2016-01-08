#ifndef _CODING_INTEGERS_
#define _CODING_INTEGERS_

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cassert>
#include <string>

using namespace std;

// Integer to String
string inttostr(int x);

// Fibonacci
const int FIB_N = 45;
const int fib[] = {
			  1,
			  2,
			  3,
			  5,
			  8,
			 13,
			 21,
			 34,
			 55,
			 89,
			144,
			233,
			377,
			610,
			987,
		   1597,
		   2584,
		   4181,
		   6765,
		  10946,
		  17711,
		  28657,
		  46368,
		  75025,
		 121393,
		 196418,
		 317811,
		 514229,
		 832040,
		1346269,
		2178309,
		3524578,
		5702887,
		9227465,
	   14930352,
	   24157817,
	   39088169,
	   63245986,
	  102334155,
	  165580141,
	  267914296,
	  433494437,
	  701408733,
	 1134903170,
	 1836311903,
};

string fibonacci_encoding_str(int x);
int fibonacci_decoding_str(string s);

long long fibonacci_encoding(int x);
int fibonacci_decoding(long long x);

void checkFibonacci();

#endif

