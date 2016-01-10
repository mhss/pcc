#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 4) {
		puts("uso: ./fmb in out x");
		puts("cria um arquivo out com os x primeiros MBs de in");
		return 1;
	}
	
	FILE* in = fopen(argv[1], "r");
	FILE* out = fopen(argv[2], "w");
	
	if (in == NULL || out == NULL)
		return 1;
	
	int n = atoi(argv[3]) << 20;
	
	for(int i = 0; i < n; i++)
		fputc(fgetc(in), out);
	
	fclose(in);
	fclose(out);
	
	return 0;
}

