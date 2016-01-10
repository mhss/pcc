#include "LZ77.h"

// LzIterator
LzIterator::LzIterator(int a, bool b) {
	pos = a, start = b;
}

// LzStack
LzStack::LzStack(int sz) {
	size = sz;
	array = (char*) malloc(size * sizeof(char));
	
	begin = 0, end = 0;
	empty = true;
}

void LzStack::push(char c) {
	// Array is full. Move end'
	if(!empty && begin == end) {
		if(++end == size)
			end = 0;
	}
	
	array[begin] = c;
	
	// Move begin'
	if(++begin == size)
		begin = 0;
	
	empty = false;
}

void LzStack::clear() {
	begin = 0, end = 0;
	empty = true;
}

int LzStack::getSize() {
	if(empty) return 0;
	if(begin == end) return size;
	return begin - end;
}

LzIterator LzStack::getIterator() {
	return LzIterator(end, true);
}

LzIterator LzStack::getIterator(int pos) {
	pos = begin - pos;
	if(pos < 0) pos += size;
	return LzIterator(pos, true);
}

bool LzStack::hasNext(LzIterator it) {
	if(empty) // estrutura vazia
		return false;
	
	// Or the iterator just started, or iterator is not begin'
	return it.start || (it.pos != begin);
}

char LzStack::next(LzIterator &it) {
	it.start = false;
	char r = array[it.pos];
	if(++it.pos == size)
		it.pos = 0;
	
	return r;
}

void LzStack::close() {
	free(array);
}

// LzStack : aux
void showStack(LzStack stk) {
	LzIterator it = stk.getIterator();
	while(stk.hasNext(it)) {
		printf(".%c ", stk.next(it));
	} puts("");
}

// LzQueue
LzQueue::LzQueue(int sz) {
	size = sz;
	array = (char*) malloc(size * sizeof(char));
	
	begin = 0, end = 0;
	empty = true;
}

void LzQueue::push(char c) {
	array[end] = c;
	
	// Move end'
	if(++end == size)
		end = 0;
	
	empty = false;
}

void LzQueue::pop(int x) {
	begin += x;
	if(begin >= size)
		begin -= size;
	
	if(begin == end)
		empty = true;
}

void LzQueue::clear() {
	begin = 0, end = 0;
	empty = true;
}

bool LzQueue::full() {
	return (!empty) && (begin == end);
}

LzIterator LzQueue::getIterator() {
	return LzIterator(begin, true);
}

bool LzQueue::hasNext(LzIterator it) {
	if(empty) // estrutura vazia
		return false;
	
	// Or the iterator just started, or iterator is not end'
	return it.start || (it.pos != end);
}

char LzQueue::next(LzIterator &it) {
	it.start = false;
	char r = array[it.pos];
	if(++it.pos == size)
		it.pos = 0;
	
	return r;
}

void LzQueue::close() {
	free(array);
}

// LzQueue : aux
void showQueue(LzQueue que) {
	LzIterator it = que.getIterator();
	while(que.hasNext(it)) {
		printf(".%c ", que.next(it));
	} puts("");
}

// Struct LZ77
LZ77::LZ77() {
	dictionary = LzStack(LZ77_DICTIONARY_BUFFER_LENGTH);
	preview = LzQueue(LZ77_PREVIEW_BUFFER_LENGTH);
	
	previewTable = (char*) malloc((LZ77_PREVIEW_BUFFER_LENGTH + 2) * sizeof(char));
	fail = (int*) malloc((LZ77_PREVIEW_BUFFER_LENGTH + 2) * sizeof(int));
}

#define ROOT 0
inline void LZ77::buildFailFunction() {
	int &i = previewSize = 1; // Copiando conteúdo de preview' para previewTable'
	LzIterator it = preview.getIterator();
	while(preview.hasNext(it)) {
		previewTable[i++] = preview.next(it);
	}
	
	// Construindo função de falha
	fail[ROOT] = -1;
	
	// Busca em largura
	for(int j = 0; j < (i - 1); ++j) {
		int f = fail[j];
		while(f != -1 && previewTable[f + 1] != previewTable[j + 1])
			f = fail[f];
		
		if(f == -1) f = ROOT;
		else f = f + 1;
		
		fail[j + 1] = f;
	}
	
	previewTable[i] = '\0';
	previewTable[0] = '.';
}

void LZ77::encoding(Stream &stream, Alphabet &alpha, File &file) {
	bool terminalActive = false;
	
	// Limpando estruturas
	dictionary.clear();
	preview.clear();
	
	// Enquanto há mais caracteres na stream (string), ou a tabela preview' possui caracteres
	while(stream.hasNext() || ! preview.empty) {
		
		// Se houver caracteres disponíveis na stream e houver espaço na tabela preview, ...
		while(stream.hasNext() && ! preview.full())
			preview.push(stream.next());
		
		// Se a stream está finalizada, o terminal não foi inserido na preview', e há espaço para caracteres
		if(! stream.hasNext() && ! terminalActive && ! preview.full()) {
			terminalActive = true;
			preview.push('\0');
		}
		
		// Construindo Aho-Corasick para tabela preview'
		buildFailFunction();
		
		// Procurando maior sufixo do dictionary'
		int k = ROOT;
		int dictionaryIdx = dictionary.getSize();
		
		// Melhor emparelhamento encontrado
		pair<int,int> bestMatch = make_pair(0, 0);
		// pair(posição no dictionary, quantidade de caracteres)
		
		LzIterator it = dictionary.getIterator();
		while(dictionary.hasNext(it)) {
			char next = dictionary.next(it);
			while(k != -1 && previewTable[k + 1] != next)
				k = fail[k];
			
			if(k == -1) k = ROOT;
			else k = k + 1;
			
			// Se um emparelhamento maior foi encontrado, ou um emparelhamento de mesmo tamanho,
			// mais o valor da posição é menor, então este emparelhamento é melhor
			if(k > 0 && k >= bestMatch.S)
				bestMatch = make_pair(dictionaryIdx + k - 1, k);
			
			// Atualizando posição do dicionário
			dictionaryIdx = dictionaryIdx - 1;
		}
		
		// Tentando estender o emparelhamento encontrado, através de um "loop"
		if(bestMatch.S > 0 && bestMatch.F == bestMatch.S && false) {
			for(int j = 1, next = bestMatch.S + 1;
				previewTable[j] == previewTable[next];
				++j, ++next, ++bestMatch.S);
		}
		
		// Utilizando o valor obtido do Aho-Corasick
		char character_to_push = '!';
		// Atualizando valor do dictionary' & preview'
		for(int j = 1; j <= bestMatch.S; ++j)
			dictionary.push(previewTable[j]);
		
		if(bestMatch.S == LZ77_PREVIEW_BUFFER_LENGTH)
			bestMatch.S--;
		
		character_to_push = previewTable[bestMatch.S + 1];
		dictionary.push(character_to_push);
		preview.pop();
		
		preview.pop(bestMatch.S);
		
		// Escreve a saída no arquivo
		serialize_int(file, bestMatch.F);
		if(bestMatch.F > 0) serialize_int(file, bestMatch.S);
		serialize_int(file, alpha.getIndex(character_to_push));
	}
}

void LZ77::decoding(File &file, Alphabet &alpha, Stream &stream, int textSize) {
	// Limpando estruturas
	dictionary.clear();
	preview.clear();
	
	char c;
	// Enquanto há conteúdo no arquivo
	int z = 0;
	while(! file.terminated) {
		
		int pos = deserialize_int(file);
		int cnt = (pos > 0) ? deserialize_int(file) : 0;
		int idx = deserialize_int(file);
		
		while(cnt--) {
			LzIterator it = dictionary.getIterator(pos);
			c = dictionary.next(it);
			stream.write(c);
			dictionary.push(c);
			++z;
		}
		
		// Add idx-character to text
		c = alpha.getChar(idx);
		
		// Terminal
		if(c == '\0') break;
		
		stream.write(c);
		dictionary.push(c);
		++z;
	}
}

#undef ROOT

void LZ77::close() {
	dictionary.close();
	preview.close();
	
	free(previewTable);
	free(fail);
}

// Tests
void testStack() {
	puts("LzStack");
	LzStack stk = LzStack(LZ77_DICTIONARY_BUFFER_LENGTH);
	stk.push('a');
	showStack(stk);
	
	stk.push('a');
	stk.push('c');
	showStack(stk);
	
	stk.push('a');
	stk.push('a');
	stk.push('c');
	stk.push('a');
	showStack(stk);
	
	stk.push('b');
	showStack(stk);
	
	stk.push('c');
	stk.push('a');
	stk.push('b');
	stk.push('a');
	showStack(stk);
	
	for(char c = 'A'; c != 'I'; ++c)
		stk.push(c), showStack(stk);
}

void testQueue() {
	puts("LzQueue");
	LzQueue que = LzQueue(LZ77_PREVIEW_BUFFER_LENGTH);
	que.push('a');
	que.push('a');
	que.push('c');
	que.push('a');
	showQueue(que);
	
	que.push('a');
	que.push('c');
	showQueue(que);
	
	que.push('a');
	que.push('b');
	showQueue(que);
	
	que.pop(2);
	que.push('c');
	que.push('a');
	showQueue(que);
	
	que.pop();
	que.push('a');
	showQueue(que);
	
	que.pop(2);
	que.push('a');
	que.push('c');
	showQueue(que);
}

void testLz77() {
	// Lz77 Test
	puts("LZ 77");
	Alphabet alpha = Alphabet();
	char alp[] = "abcdet_ACDS";
	for(int i = 0; alp[i]; ++i)
		alpha.push(alp[i]);
	
	LZ77 lz77 = LZ77();
	Stream stream = Stream("");
	File out = File("data/Output.out");
	lz77.encoding(stream = Stream("aabbbbbaaaaaaa"), alpha, out);
	lz77.encoding(stream = Stream("aacaacabcabaaacd"), alpha, out);
	lz77.encoding(stream = Stream("aacaacabcabaaac"), alpha, out);
	lz77.encoding(stream = Stream("aacaacabca"), alpha, out);
	lz77.encoding(stream = Stream("abaca"), alpha, out);
	lz77.encoding(stream = Stream("abacabadabacaba"), alpha, out);
	lz77.encoding(stream = Stream("A_ASA_DA_CASA"), alpha, out);
	lz77.encoding(stream = Stream("abacate"), alpha, out);
	
	lz77.close();
}

