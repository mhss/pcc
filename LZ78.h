#ifndef _LZ78_
#define _LZ78_

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

#include <vector>

#include "alphabet_index.h"
#include "file_manager.h"
#include "stream.h"
#include "serial.h"

#define MP make_pair
#define F first
#define S second

using namespace std;

#define EPS 1e-7
#define dbg if(0)

struct LzNode {
	int id;
	std::map<char, LzNode* > child;
	
	LzNode(int x = 0);
	void clear();
};

struct LZ78 {
	int nextId;
	LzNode *root;
	std::vector< std::pair<int,char> > lista;
	
	LZ78();
	void push_recursive(Stream &stream, int pos);
	void encoding(Stream &stream, Alphabet &alpha, File &file);
	void decoding(File &file, Alphabet &alpha, Stream &stream, int textSize);
	void close();
};

void testLz78();

#endif

