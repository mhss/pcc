#include <cstdio>

using namespace std;

//g++ somador.cpp -o somador
int main() {
	int cnt = 0;
	double sum = 0;
	double a, b;
	
	while (scanf("%lf%lf", &a, &b) == 2) {
		sum += a+b;
		cnt++;
	}
	
	if (cnt)
		printf("%lf\n", sum/double(cnt));
}

