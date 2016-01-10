#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 4) {
		puts("uso: ./gen str n file");
		puts("gera 100 strings aleatorias de tamanho n");
		puts("com caracteres de str");
		puts("e escreve elas em file");
		
		return 1;
	}
	
	int m = strlen(argv[1]);
	int n = atoi(argv[2]);
	freopen(argv[3], "w", stdout);
	
	srand(time(NULL));
	for(int i = 0; i < 100; i++) {
		for(int j = 0; j < n; j++) 
			putchar(argv[1][rand()%m]);
		
		puts("");
	}
	
	return 0;
}

