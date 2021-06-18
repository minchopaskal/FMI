#include <cstdio>
#include <ctime>

#include <rmq.h>

using baron::RMQ;

int main(int argc, char **argv) {

	bool useNaive = false;
	if (argc > 1) {
		--argc;
		while (argc-- > 0) {
			if (!strncmp(argv[argc], "use-naive", strlen(argv[argc]))) {
				useNaive = true;
				break;
			}
		}
	}

	//RMQ::Array a{ 1, 5, 4, -1, 3, 2, 0, 6, 3, 4 };

	RMQ::Array a;
	a.resize(100'000'000);
	srand((unsigned)time(0));

	for (int i = 0; i < 10'000'000; ++i) {
		a[i] = rand();
	}

	for (int i = 4; i <= 7; ++i)
		printf("a[%d] = %d\n", i, a[i]);

	RMQ rmq{ a };
	time_t s, e;

	time(&s);
	rmq.makeIndex();
	time(&e);

	printf("time: %.6f\n", difftime(e, s));

	int i = 4, j = 7;
	printf("RMQ(%d, %d) = %d", i, j, rmq.query(i, j));

	int g = 0;
	scanf_s("%d", &g);

	return 0;
}