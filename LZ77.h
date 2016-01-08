#ifndef _LZ77_
#define _LZ77_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cctype>
#include <cmath>
#include <cassert>
#include <ctime>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>

#include "alphabet_index.h"
#include "coding_integers.h"
#include "file_manager.h"
#include "stream.h"
#include "serial.h"

#define MP make_pair
#define F first
#define S second

using namespace std;

#define EPS 1e-7
#define dbg if(0)

#define LZ77_DICTIONARY_BUFFER_LENGTH (1 << 10) // TODO
#define LZ77_PREVIEW_BUFFER_LENGTH (1 << 10) // TODO

struct LzIterator {
	int pos;
	bool start;
	LzIterator(int a = 0, bool b = false);
};

struct LzStack {
	char *array;
	int size;
	int begin, end;
	bool empty;
	
	// Iterator
	LzStack(int sz = 1);
	
	void push(char c);
	void clear();
	int getSize();
	
	LzIterator getIterator();
	LzIterator getIterator(int pos);
	bool hasNext(LzIterator it);
	char next(LzIterator &it);
	
	void close();
};

void showStack(LzStack stk);


struct LzQueue {
	char *array;
	int size;
	int begin, end;
	bool empty;
	
	LzQueue(int sz = 1);
	
	void push(char c);
	void pop(int x = 1);
	void clear();
	bool full();
	
	LzIterator getIterator();
	bool hasNext(LzIterator it);
	char next(LzIterator &it);
	
	void close();
};

void showQueue(LzQueue que);


struct LZ77 {
	LzStack dictionary; // dictionary buffer
	LzQueue preview; // preview buffer
	
	// Aho-Corasick
	int previewSize;
	char *previewTable; // String to Aho
	int *fail; // Fail Function
	
	LZ77();
	inline void buildFailFunction();
	void encoding(Stream &stream, Alphabet &alpha, File &file);
	void decoding(File &file, Alphabet &alpha, Stream &stream, int textSize);
	void close();
};

void testStack();
void testQueue();
void testLz77();

#endif

