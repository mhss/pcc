#include "stream.h"

using namespace std;

Stream::Stream(const char *st) {
	idx = 0;
	str = (char*) malloc((strlen(st) + 1) * sizeof(char));
	strcpy(str, st);
	
	is_string = true;
}

Stream::Stream(char *st) {
	idx = 0;
	str = st;
	
	is_string = true;
}

Stream::Stream(File f) {
	file = f;
	is_string = false;
}

bool Stream::hasNext() {
	if(is_string) {
		return str[idx];
	} else {
		return ! file.terminated;
	}
}

char Stream::next() {
	if(is_string) {
		return str[idx++];
	} else {
		return file.read_char();
	}
}

void Stream::write(char c) {
	if(is_string) {
		str[idx++] = c;
	} else {
		file.write_char(c);
	}
}

void Stream::close() {
	if(is_string)
		str[idx] = '\0';
	else
		file.close();
}

void testStream() {
	Stream s = Stream("abc");
	while(s.hasNext())
		printf("%c\n", s.next());
	s.close();
	
	s = Stream(File("data/Output.out", "r"));
	while(s.hasNext())
		printf("%c\n", s.next());
	s.close();
}

