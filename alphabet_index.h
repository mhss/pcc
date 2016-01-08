#ifndef _ALPHABET_INDEX_
#define _ALPHABET_INDEX_

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

#define ALPHA_LENGTH (1 << 9)
struct Alphabet {
	char *lista;
	int *index;
	int size;
	
	Alphabet(bool dynamic = false);
	
	inline bool isStatic();
	
	// Não chamar este método com caractere repetido
	void push(char c);
	
	// Dado um índice, retorna qual o idx-ésimo caractere do alfabeto
	char getChar(int idx);
	
	// Dado um caractere, retorna qual o índice do caractere no alfabeto
	int getIndex(char c);
	
	// Escreve no arquivo o alfabeto codificado
	void writeTo(File &file);
	// Lê do arquivo decodificado o alfabeto
	void decoding(File &file);
	//void read(File &file);
	
	// Limpando estrutura
	void clear();
};

// Incluir serial.h após criar a struct Alphabet
#include "serial.h"

void testAlphabet();

#endif

