#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <set>

using namespace std;

const int SIZE = 36 << 20;
char str[SIZE];

int K = 20;
set<int> pos;

int main(int argc, char **argv) {
	if (argc != 4) {
		puts("uso: ./gen_filter file_input n file_output");
		puts("gera K strings do arquivo file_input de tamanho n");
		puts("e escreve elas em file");
		
		return 1;
	}
	
	int tam = atoi(argv[2]);
	freopen(argv[1], "r", stdin);
	
	if(tam == 0) {
		freopen(argv[3], "w", stdout);
		puts("");
		return 0;
	}
	
	srand(time(NULL));
	int p = 0;
	char c;
	while((scanf("%c", &c) > 0) & p < SIZE)
		str[p++] = c;
	str[p] = '\0';
	
	pos.clear();
	int cnt = 0;
	const int LIM = 10000;
	while((int) pos.size() < K) {
		int z = rand() % (p - tam);
		bool ok = true;
		if(cnt > LIM) break;
		++cnt;
		for(int i = 0; i < tam; ++i)
			if(str[z + i] == '\n' ||
				str[z + i] == '\t' ||
				str[z + i] == '\r' ||
				str[z + i] == ' ')
					ok = false;
		
		if(! ok) continue;
		
		cnt = 0;
		pos.insert(z);
	}
	
	printf("   words %d\n", (int) pos.size());
	freopen(argv[3], "w", stdout);
	while(! pos.empty()) {
		int z = *pos.begin();
		pos.erase(pos.begin());
		
		c = str[z + tam];
		str[z + tam] = '\0';
		printf("%s\n", str + z);
		str[z + tam] = c;
	}
	
	return 0;
}

