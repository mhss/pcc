#include "LZ78.h"


LzNode::LzNode(int x) {
	id = x;
	child.clear();
}

void LzNode::clear() {
	for(__typeof((child).begin()) a = child.begin(); a != child.end(); ++a)
		a->second->clear();
	
	for(__typeof((child).begin()) a = child.begin(); a != child.end(); ++a)
		free(a->second);
	
	child.clear();
}

// Struct LZ77
LZ78::LZ78() {
	nextId = 0;
	
	// Raiz
	root = new LzNode(nextId++);
}

#define ROOT 0
void LZ78::encoding(Stream &stream, Alphabet &alpha, File &file) {
	LzNode *curr = root;
	char c;
	
	// Para cada caractere de entrada
	while(stream.hasNext()) {
		c = stream.next();
		
		// Se já existe um nó com o caractere c'
		if(curr->child.count(c)) {
			curr = curr->child[c];
		
		// Não existe a substring procurada, então ..
		} else {
//			printf(" (%2d, %c)", curr->id, c);
			serialize_int(file, curr->id);
			serialize_int(file, alpha.getIndex(c));
			
			// Criar um novo nó
			LzNode *new_node = new LzNode(nextId++);
			curr->child.insert(std::make_pair(c, new_node));
			
			curr = root;
		}
	}
	
	// Terminal
//	printf(" (%2d, 0)\n", curr->id);
	serialize_int(file, curr->id);
	serialize_int(file, alpha.getIndex('\0'));
}

void LZ78::push_recursive(Stream &stream, int pos) {
	// Se não é a raiz ..
	int previous = lista[pos].first;
	if(previous > 0)
		push_recursive(stream, previous - 1);
	
	// Adiciona o caractere atual
	stream.write(lista[pos].second);
}

void LZ78::decoding(File &file, Alphabet &alpha, Stream &stream, int textSize) {
	lista.clear();
	lista.reserve(101 + textSize / 5);
	
	char c;
	while(! file.terminated) {
		int pos = deserialize_int(file);
		int idx = deserialize_int(file);
		
		// Se há algo a ser adicionado (ou seja, se não é a raiz)
		if(pos)
			push_recursive(stream, pos - 1);
		
		c = alpha.getChar(idx);
		
		// Terminal
		if(c == '\0') break;
		
		// Atualiza a lista
		lista.push_back(make_pair(pos, c));
		
		// Escreve o caractere
		stream.write(c);
	}
}

#undef ROOT

void LZ78::close() {
	root->clear();
	free(root);
}

// Tests
void testLz78() {
	// Lz78 Test
	puts("LZ 78");
}

