#ifndef _FILE_MANAGER_
#define _FILE_MANAGER_

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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

#define EPS 1e-7
#define dbg if(0)

#define BYTE_LENGTH 8
#define FILE_BUFFER_LENGTH (1 << 16)

struct File {
	FILE *file;
	char *buffer;
	int offset, position, size;
	int totalSize;
	bool terminated;
	
	File(const char *filename = "", const char *mode = "");
	
	void loadBuffer();
	
	char read_char();
	char read_bit();
	void write_char(char c);
	void write_bit(bool bit);
	void set_pos(int x);
	
	char* gets(char *st, int maxLen);
	
	void flush();
	void close();
};

void testFileBit();
void testFileChar();

#endif


