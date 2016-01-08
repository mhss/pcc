#include "file_manager.h"

using namespace std;

File::File(const char *filename, const char *mode) {
	file = fopen(filename, mode);
	
	buffer = (char*) malloc(FILE_BUFFER_LENGTH * sizeof(char));
	
	// Read-mode
	if(strstr(mode, "r") != NULL) {
		loadBuffer();
		terminated = offset == size;
		
		struct stat buf;
		fstat(fileno(file), &buf);
		totalSize = buf.st_size;
	
	// Write-mode
	} else {
		offset = 0;
		position = BYTE_LENGTH - 1;
		buffer[offset] = 0;
		totalSize = -1;
	}
}

void File::loadBuffer() {
	offset = 0;
	position = BYTE_LENGTH - 1;
	size = fread(buffer, 1, FILE_BUFFER_LENGTH, file);	
}

char File::read_char() {
	// Já acabou o arquivo
	if(terminated)
		return -1;
	
	// Há um byte disponível
	char r = '\0';
	if(position == BYTE_LENGTH - 1) {
		r = buffer[offset++];
	
		// Atualizando offset
		if(offset == size) {
			loadBuffer();
		
			// Se, após carregar, não há mais nada no buffer, End of File
			if(offset == size)
				terminated = true;
		}
	} else { // Carregar bit por bit
		r = 0;
		for(int i = BYTE_LENGTH - 1; i >= 0; --i) {
			if(terminated) {
				r = -1;
				break;
			}
			
			r = (r << 1) | read_bit();
		}
	}
	
	return r;
}

char File::read_bit() { // 0, 1, or -1
	// Já acabou o arquivo
	if(terminated)
		return -1;
	
	// Bit a ser retornado
	char r = (buffer[offset] >> position--) & 1;
	
	// Ajusta offset & position
	if(position < 0) {
		position = BYTE_LENGTH - 1;
		offset += 1;
	}
	
	// Se é necessário carregar dados do arquivo no buffer
	if(offset == size) {
		loadBuffer();
		
		// Se, após carregar, não há mais nada no buffer, End of File
		if(offset == size)
			terminated = true;
	}
	
	return r;
}

void File::write_char(char c) {
	// Ajusta offset & position
	if(position < 0) {
		offset += 1;
		position = BYTE_LENGTH - 1;
		buffer[offset] = 0;
	}
	
	// Se há conteúdo demais no buffer', escreva no arquivo e reinicialize o buffer
	if(offset >= FILE_BUFFER_LENGTH)
		flush();
	
	// Se position' está no início de uma posição
	if(position == BYTE_LENGTH - 1) {
		buffer[offset] = c; // Escreve diretamente o char
		offset++;
		
	} else { // Caso contrário, será necessário escrever bit por bit
		for(int i = BYTE_LENGTH - 1; i >= 0; --i)
			write_bit((c >> i) & 1);
	}
	
}

void File::write_bit(bool bit) {
//	printf("  File::write_bit(bit = %d)  ~ { pos: %2d, offset: %3d }\n", bit, position, offset);
	// Ajusta offset & position
	if(position < 0) {
		offset += 1;
		position = BYTE_LENGTH - 1;
		buffer[offset] = 0;
	}
	
	if(offset >= FILE_BUFFER_LENGTH)
		flush();
	
	buffer[offset] = (buffer[offset] & ~(1 << position)) | (bit << position);
	--position;
}

void File::set_pos(int x) {
	flush();
	
	// Posiciona o "cursor" na posição desejada
	fseek(file, x, SEEK_SET);
}

char* File::gets(char *st, int maxLen) {
	if(terminated)
		return st = NULL;	
	
	char c = '!';
	while(! terminated) {
		c = read_char();
		if(c != '\n' && c != '\r')
			break;
	}
	
	int z = 0;
	while(! terminated && c != '\n' && c != '\r') {
		st[z++] = c;
		c = read_char();
		
		if(z > maxLen) {
			printf("Erro: Tamanho do padrão maior que o permitido!\n");
			break;
		}
	}
	
	st[z] = '\0';
	return st;
}

void File::flush() {
	fwrite(buffer, 1, offset + (position < (BYTE_LENGTH - 1)), file);
	
	offset = 0;
	position = BYTE_LENGTH - 1;
	buffer[offset] = 0;
}

void File::close() {
	flush();
	free(buffer);
	fclose(file);
}

void testFileBit() {
	File file;
	
	file = File("data/Output.out", "w");
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(1);
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(1);
	file.write_bit(1);
	
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(1);
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(1);
	file.write_bit(0);
	file.write_bit(0);
	
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(0);
	file.write_bit(1);
	file.write_bit(0);
	file.write_bit(1);
	file.write_bit(0);
	
	file.close();
	
	file = File("data/Output.out", "r");
	
	// Conteúdo do arquivo
	char str[] = "#$\n";
	int j = 0;
	
	int k;
	while(! file.terminated) {
		int byte = 0;
		for(int i = 0; i < 8; ++i)
			byte = (byte << 1) | file.read_bit();
//		printf(" << %c %c\n", str[j], byte);
		assert(str[j++] == byte);
	}
}

void testFileChar() {
	File file;
	
	file = File("data/Output.out", "w");
	file.write_char('#');
	file.write_char('$');
	file.write_char('\n');
	file.close();
	
	file = File("data/Output.out", "r");
	
	// Conteúdo do arquivo
	char str[] = "#$\n";
	int j = 0;
	
	int k;
	while(! file.terminated) {
		k = file.read_char();
//		printf(" << %c(%3d) %c(%3d)\n", str[j], str[j], k, k);
		assert(str[j++] == k);
	}
}

