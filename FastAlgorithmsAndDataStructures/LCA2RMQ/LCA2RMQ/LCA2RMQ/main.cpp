#include <cstdio>

#include "rmq_pm1.h"

using baron::RMQPM1;

int main(int argc, char **argv) {
	Node a{ 1 };
	Node b{ 2 };
	Node c{ 3 };
	Node d{ 4 };
	Node e{ 5 };

	a.children = { &b, &c };
	b.children = { &d, &e };

	auto rmq = RMQPM1(&a, 5);

	auto q = rmq.LCA(&d, &c);
	printf("Result(d,c) (expect a=1): %d\n", q->data);

	q = rmq.LCA(&e, &c);
	printf("Result(e,c) (expect a=1): %d\n", q->data);

	q = rmq.LCA(&b, &c);
	printf("Result(b,c) (expect a=1): %d\n", q->data);

	q = rmq.LCA(&a, &c);
	printf("Result(a,c) (expect a=1): %d\n", q->data);

	q = rmq.LCA(&e, &d);
	printf("Result(d,e) (expect b=2): %d\n", q->data);

	q = rmq.LCA(&e, &e);
	printf("Result(e,e) (expect e=5): %d\n", q->data);

	int i;
	scanf_s("%d", &i);
}