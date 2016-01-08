#ifndef _STREAM_
#define _STREAM_

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

#include "file_manager.h"

struct Stream {
	bool is_string;
	
	// String-mode
	char *str;
	int idx;
	
	// File-mode
	File file;
	
	Stream(const char *st);
	Stream(char *st);
	Stream(File file);
	
	bool hasNext();
	char next();
	
	void write(char c);
	
	void close();
};

void testStream();

#endif


