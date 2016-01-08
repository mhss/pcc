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
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <bitset>
//#include <complex>
#include <limits>
#include <functional>
#include <numeric>
#include <fcntl.h>
#include <sys/stat.h>

#include "file_manager.h"
#include "stream.h"
#include "coding_integers.h"
#include "alphabet_index.h"
#include "LZ77.h"
#include "LZ78.h"
#include "suffix_tree.h"
#include "serial.h"

using namespace std;

const int MAX_PATT_LEN = 1000;
char *index_name;

// Opções
bool multiple_patterns = false;
bool count_only = false;
bool alphabetStatic = true;
bool isLZ77 = false;

//concatena ext no nome do arquivo
//se possível, substitui a extensão dele
void load_name(char *original, const char *ext) {
	int len = strlen(original);
	index_name = (char*) malloc((len + 5) * sizeof(char));
	strcpy(index_name, original);
	
	int end = len-1;
	while(end > 0 && index_name[end] != '.') end--;
	
	if(end == 0)
		end = len;
	
	strcpy(index_name + end, ext);
}

void load_index_name(char *original) {
	load_name(original, ".idx");
}

void load_compress_name(char *original) {
	load_name(original, ".com");
}

void readOptions(int argc, char **argv, int params = 1) {
	for(int i = 2; i < (argc - params); i++) {
		if (!strcmp(argv[i], "-p") ||
			!strcmp(argv[i], "--pattern")) 
			multiple_patterns = true;
		
		else if (!strcmp(argv[i], "-c") ||
				!strcmp(argv[i], "--count")) 
			count_only = true;

		else if (!strcmp(argv[i], "-7") ||
			!strcmp(argv[i], "--LZ77")) 
			isLZ77 = true;
			
		else if (!strcmp(argv[i], "-8") ||
				!strcmp(argv[i], "--LZ78")) 
			isLZ77 = false;
		else {
		
			puts("Opção não reconhecida");
			throw 1;
		}
	}
}

void encoding(Stream &stream, Alphabet &alpha, File &file) {
	if(isLZ77) {
		LZ77 lz77 = LZ77();
		lz77.encoding(stream, alpha, file);
	} else {
		LZ78 lz78 = LZ78();
		lz78.encoding(stream, alpha, file);
	}
}

void decoding(File &file, Alphabet &alpha, Stream &stream, int textSize) {
	if(isLZ77) {
		LZ77 lz77 = LZ77();
		lz77.decoding(file, alpha, stream, textSize);
	} else {
		LZ78 lz78 = LZ78();
		lz78.decoding(file, alpha, stream, textSize);
	}
}


#define OFFSET 128
#define CHARACTER_SIZE (1 << 9)
bool character[CHARACTER_SIZE];

int do_index(int argc, char **argv) {
	if (argc < 3)
		return 1;
	
	// Options
	readOptions(argc, argv, 1);
	
	// Reading text file & building alphabet
	File file = File(argv[argc-1], "r");
	if(file.file == NULL)
		return 1;
	int fileSize = file.totalSize;
	
	if (fileSize > MAXLEN) {
		printf("arquivo de texto muito longo: limite é %d caracteres\n", MAXLEN);
		return 1;
	}
	
	// No one character was seen ... until now!
	memset(character, false, sizeof(character));
	
	str = (char*) malloc((fileSize + 1) * sizeof(char));
	str[fileSize] = '\0';
	
	lines = 1;
	for(int i = 0; i < fileSize; ++i) {
		char c = file.read_char();
		str[i] = c;
		character[c + OFFSET] = true;
		
		if (c == '\n')
			lines++;
	}
	
	file.close();
	
	
	// Building alphabet
	Alphabet alpha = Alphabet(alphabetStatic);
	for(int i = 0; i < CHARACTER_SIZE; ++i)
		if(character[i])
			alpha.push((char) i - OFFSET);
	
	// Adding the null-character
	alpha.push('\0');
	
	// Building the suffix tree
	build_suffix_tree(fileSize);
	
	// Creating filename to result file
	load_index_name(argv[argc-1]);
	
	file = File(index_name, "w");
	if(file.file == NULL)
		return 1;
	
	// Header
	file.write_bit(alphabetStatic); // Static alphabet
	file.write_bit(isLZ77); // LZ 77
	
	// Alphabet
	alpha.writeTo(file);
	
	// Suffix Tree
	serialize_st(file, alpha);
	clear_st();
	
	// Text
	serialize_int(file, fileSize); // Tamanho do texto
	serialize_int(file, lines); // número de linhas do texto
	
	// Codificando texto original
	Stream stream = Stream(str);
	encoding(stream, alpha, file);
	
	file.close();
	free(index_name);
	
	return 0;
}

void do_search(char *pattern) {
	//acha o nó mais baixo na árvore de sufixo
	//que tenha o padrão como prefixo do rótulo
	int h;
	Node v = find_node(pattern, h);
	
	if (v != NULL)
		mark_occurrences(v, h);
}

int do_search(int argc, char **argv) {
	if (argc < 4)
		return 1;
	
	// lê as opções de linha de comando
	readOptions(argc, argv, 2);
	
	// Reading the file
	load_index_name(argv[argc-2]);
	
	File file = File(index_name, "r");
	if(file.file == NULL)
		return 1;
	
	// Headers
	alphabetStatic = file.read_bit();
	isLZ77 = file.read_bit();
	
	// Decoding alphabet
	Alphabet alpha = Alphabet(alphabetStatic);
	alpha.decoding(file);
	
	// Building the suffix tree
	deserialize_st(file, alpha);
	
	// Text
	s_len = deserialize_int(file); // Tamanho do texto
	str = (char*) malloc((s_len + 1) * sizeof(char));
	str[s_len] = '\0';
	
	lines = deserialize_int(file); // número de linhas do texto
	
	// Decodificando o texto original
	Stream stream = Stream(str);
	decoding(file, alpha, stream, s_len);
	stream.close();
	//printf("'%s'.", str);
	
	file.close();
	
	// Criando separação das linhas do texto
	count_lines();
	
	//limpa os resultados
	occ.clear();
	
	//realiza a busca no(s) arquivo(s)
	char* pattern = argv[argc-1];
	
	if (multiple_patterns) {
		//abre o arquivo com os padrões
		file = File(pattern, "r");
		if (file.file == NULL)
			return 1;
		
		char* buffer = (char*) malloc((MAX_PATT_LEN + 1)*sizeof(char));
		
		while (file.gets(buffer, MAX_PATT_LEN + 1) != NULL) {
			//tira o \n do texto, se houver, e ignora linhas vazias
			int len = strlen(buffer);
			
			if (len > 0 && buffer[len-1] == '\n') 
				buffer[--len] = '\0';
			
			if (len > 0) 
				do_search(buffer);
		}
		
		free(buffer);
		file.close();
	} else
		do_search(pattern);
	
	//mostra os resultados
	if (count_only) 
		printf("%d\n", (int) occ.size());
	else {
		for(std::set<int>::iterator it = occ.begin(); it != occ.end(); it++) {
			int i = *it;
			//acha as posições de início e fim da linha
			int beg = (i == 0)? 0: line_end[i-1]+1;
			int end = line_end[i];
			
			//imprime a linha
			char tmp = str[end];
			str[end] = '\0';
			//printf("%3d: ", i+1);
			printf("%s\n", str+beg);
			str[end] = tmp;
		}
	}
	
	return 0;
}


// Comprime o arquivo texto
int do_compress(int argc, char **argv) {
	if (argc < 3)
		return 1;

	// Options
	readOptions(argc, argv, 1);
	int idxFile = argc - 1;
	
	// Reading text file & building alphabet
	File file = File(argv[idxFile], "r");
	if(file.file == NULL)
		return 1;
	
	// No one character was seen ... until now!
	memset(character, false, sizeof(character));
	for(int i = 0; ! file.terminated; ++i) {
		char c = file.read_char();
		character[c + OFFSET] = true;
	}
	
	int fileSize = file.totalSize;
	file.close();
	
	// Building alphabet
	Alphabet alpha = Alphabet(alphabetStatic);
	for(int i = 0; i < CHARACTER_SIZE; ++i)
		if(character[i])
			alpha.push((char) i - OFFSET);
	
	// Adding the null-character
	alpha.push('\0');
	
	// Creating filename to result file
	load_compress_name(argv[idxFile]);
	
	file = File(argv[idxFile], "r");
	File result = File(index_name, "w");
	if(file.file == NULL || result.file == NULL)
		return 1;
	
	// Header
	result.write_bit(alphabetStatic); // Static alphabet
	result.write_bit(isLZ77); // LZ 77
	
	// Escrever o nome do arquivo original
	for(int i = 0; argv[idxFile][i]; ++i)
		result.write_char(argv[idxFile][i]);
	result.write_char('\0');
	
	// Alphabet
	alpha.writeTo(result);
	
	// Text
	serialize_int(result, fileSize); // Tamanho do texto
	
	// Codificando texto original
	Stream stream = Stream(file);
	encoding(stream, alpha, result);
	
	file.close();
	result.close();
	free(index_name);
	
	return 0;
}

// Descomprime o arquivo texto
int do_decompress(int argc, char **argv) {
	if (argc < 3)
		return 1;
	
	//readOptions(argc, argv, 1);
	int idxFile = argc - 1;
	
	// Reading the file
	File file = File(argv[idxFile], "r");
	if(file.file == NULL)
		return 1;
	
	// Headers
	alphabetStatic = file.read_bit();
	isLZ77 = file.read_bit();
	
	// Filename
	index_name = (char*) malloc(1024 * sizeof(char));
	int p = 0;
	while((index_name[p] = file.read_char()))
		++p;
	
	index_name[p] = '\0';
	
	// Arquivo original
	File target = File(index_name, "w");
	
	// Decoding alphabet
	Alphabet alpha = Alphabet(alphabetStatic);
	alpha.decoding(file);
	
	// Text
	s_len = deserialize_int(file); // Tamanho do texto
	Stream stream = Stream(target);
	decoding(file, alpha, stream, s_len);
	
	file.close();
	target.close();
	free(index_name);
	
	return 0;
}

#include <vector>

int main(int argc, char **argv) {
	if (argc < 2)
		return 1;
	
	//mostra a ajuda da ferramenta
	if (!strcmp(argv[1], "-h") ||
		!strcmp(argv[1], "--help")) {
		puts("Indexar arquivo: ./ipmt index [OPTION] FILE");
		puts("-7, --LZ77: comprime o arquivo de texto utilizando o algoritmo LZ77");
		puts("-8, --LZ78: comprime o arquivo de texto utilizando o algoritmo LZ78 (opção padrão)");
		puts("");
		puts("Buscar em índice: ./ipmt search [OPTION] FILE PATTERN");
		puts("-p, --pattern: busca todos os padrões do arquivo PATTERN, um por cada linha");
		puts("-c, --count: mostra a soma das quantidades de ocorrências de cada padrão");
		puts("");
		puts("Comprimir arquivo: ./ipmt compress [OPTION] FILE");
		puts("-7, --LZ77: comprime o arquivo de texto utilizando o algoritmo LZ77");
		puts("-8, --LZ78: comprime o arquivo de texto utilizando o algoritmo LZ78 (opção padrão)");
		puts("");
		puts("Descomprimir arquivo: ./ipmt decompress FILE");
		puts("");
		puts("Abrir ajuda: ./ipmt -h");
		puts("Ou           ./ipmt --help");
		
		return 0;
		
	} else if (!strcmp(argv[1], "index"))
		return do_index(argc, argv);
		
	else if (!strcmp(argv[1], "search"))
		return do_search(argc, argv);
		
	else if (!strcmp(argv[1], "compress"))
		return do_compress(argc, argv);
		
	else if (!strcmp(argv[1], "decompress"))
		return do_decompress(argc, argv);
		
	else if (!strcmp(argv[1], "show")) {
		File file = File(argv[2], "r");
		//file.read_bit(), file.read_bit();
		while(! file.terminated) {
			printf("%d", file.read_bit());
		} puts("");

	} else 
		return 1;
}

