#include "alphabet_index.h"
	
const int OFFSET = 128;

Alphabet::Alphabet(bool isStatic) {
	lista = (char*) malloc(ALPHA_LENGTH * sizeof(char));
	size = 0;
	
	if(isStatic) { // Array (para alfabeto estático) mapeando os índices
		index = (int*) malloc(ALPHA_LENGTH * sizeof(int));
	}
}

bool Alphabet::isStatic() {
	return index != NULL;
}

// Não chamar este método com caractere repetido
void Alphabet::push(char c) {
	lista[size++] = c;
	
	// Se é estático, adicionar o índice do caractere no index'
	if(isStatic())
		index[c + OFFSET] = size - 1;
}

// Dado um índice, retornar qual o idx-ésimo caractere do alfabeto
char Alphabet::getChar(int idx) {
	return lista[idx];
}

int Alphabet::getIndex(char c) {
	// Se o alfabeto é estático, O(1)
	if(isStatic()) {
		return index[c + OFFSET];
	}
	
	// Caso contrário, procurar o caractere na lista
	int i;
	for(i = 0; i < size; ++i)
		if(lista[i] == c) break;
	
	if(i == size) return -1;
	
	// Move-to-front transform
	for(int j = i; j > 0; --j)
		lista[j] = lista[j - 1];
	
	lista[0] = c;
	
	return i;
}

void Alphabet::writeTo(File &file) {
	// Tamanho do alfabeto
	serialize_int(file, size - 1);
//	printf(" << write::size %d (%d %c)\n", size, lista[0], lista[0]);
	
	// Primeiro caractere
	char first = lista[0];
	for(int i = BYTE_LENGTH - 1; i >= 0; --i)
		file.write_bit((first >> i) & 1);
	
	// Para os demais caracteres, escreva o "delta"
	// Importante: não escrever o caractere '\0' que está no final da lista
	for(int i = 1; i < size - 1; ++i)
		serialize_int(file, lista[i] - lista[i - 1]);
}

void Alphabet::decoding(File &file) {
	// Tamanho do alfabeto
	int sz = deserialize_int(file);
//	printf(" decoding.size = %d\n", sz);
	
	char first = 0;
	for(int i = BYTE_LENGTH - 1; i >= 0; --i)
		first |= file.read_bit() << i;
	
//	printf("< primeiro %c (%d)\n", first, first);
	// Primeiro caractere na estrutura
	push(first);
	
	// Para os demais caracteres, escreva o "delta"
	// Exceto o último caractere, que será o terminal '\0'
	for(int i = 1; i < sz; ++i) {
		first += deserialize_int(file);
//		printf("     %c,\n", first);
		push(first);
	}
	
	// Adicionando terminal
	push('\0');
}

// Limpando estrutura
void Alphabet::clear() {
	free(lista);
	
	if(index)
		free(index);
}


void testAlphabet() {
	Alphabet alpha = Alphabet(false);
	alpha.push('a');
	alpha.push('b');
	alpha.push('c');
	
	printf("%d\n", alpha.getIndex('b'));
	printf("%d\n", alpha.getIndex('b'));
	printf("%d\n", alpha.getIndex('c'));
	printf("%d\n", alpha.getIndex('b'));
	printf("%d\n", alpha.getIndex('a'));
	printf("%d\n", alpha.getIndex('a'));
	printf("%d\n", alpha.getIndex('a'));
	printf("%d\n", alpha.getIndex('b'));
	printf("%d\n", alpha.getIndex('c'));
	printf("%d\n", alpha.getIndex('b'));
	printf("%d\n", alpha.getIndex('a'));
	printf("%d\n", alpha.getIndex('b'));
	printf("%d\n", alpha.getIndex('a'));
	
	alpha.clear();
}

