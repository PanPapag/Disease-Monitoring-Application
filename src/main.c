#include <stdio.h>

#include "../includes/list.h"

int cmp(void *a, void *b) {
	int v = *((int *) a);
	int w = *((int *) b);
	return v - w;
}

void print(FILE *out, void *v) {
	fprintf(out, "%d\n", *((int *)v));
}

int main(void) {

  int temp;
	// Give it the type that it will hold, how to compare data and
	// how to print data.
	list_ptr l = list_create(int, cmp, print);
	// Do a bunch of random insertions.
	temp = 5;
	list_sorted_insert(&l, &temp);
	temp = 3;
	list_sorted_insert(&l, &temp);
	temp = 7;
	list_sorted_insert(&l, &temp);
	temp = 10;
	list_sorted_insert(&l, &temp);
	temp = 100;
	list_push_back(&l, &temp);
	temp = 200;
	list_push_front(&l, &temp);
	//Print the thing
	list_print(l, stdout);
	printf("\n");
	list_print_reverse(l, stdout);

	size_t res = list_size(l);

	printf("SIZE: %d\n",res);

	list_clear(l);

  return 0;
}
