# http://mrbook.org/blog/tutorials/make/

CC=g++
CFLAGS= -Wall -Wshadow -O3

all: main

main: main.o file_manager.o stream.o coding_integers.o alphabet_index.o LZ77.o LZ78.o suffix_tree.o serial.o
	$(CC) main.o file_manager.o stream.o coding_integers.o alphabet_index.o LZ77.o LZ78.o suffix_tree.o serial.o -o data/ipmt

# Files

main.o: main.cpp
	$(CC) -c $(CFLAGS) main.cpp

file-manager.o: file_manager.cpp file_manager.h
	$(CC) -c $(CFLAGS) file_manager.cpp

stream.o: stream.cpp stream.h file_manager.h
	$(CC) -c $(CFLAGS) stream.cpp

coding-integers.o: coding_integers.cpp coding_integers.h
	$(CC) -c $(CFLAGS) coding_integers.cpp

alphabet-index.o: alphabet_index.cpp alphabet_index.h file_manager.cpp serial.cpp
	$(CC) -c $(CFLAGS) alphabet_index.cpp

LZ77.o: LZ77.cpp LZ77.h alphabet_index.h file_manager.cpp stream.cpp serial.cpp
	$(CC) -c $(CFLAGS) LZ77.cpp

LZ78.o: LZ78.cpp LZ78.h alphabet_index.h file_manager.cpp stream.cpp serial.cpp
	$(CC) -c $(CFLAGS) LZ78.cpp

suffix_tree.o: suffix_tree.cpp suffix_tree.h
	$(CC) -c $(CFLAGS) suffix_tree.cpp

serial.o: serial.cpp serial.h file_manager.cpp suffix_tree.cpp
	$(CC) -c $(CFLAGS) serial.cpp


clean:
	rm *.o

