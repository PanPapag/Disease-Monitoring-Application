#include <stdio.h>

#include "../includes/list.h"
#include "../includes/avl.h"


int cmp(void *a, void *b) {
	int v = *((int *) a);
	int w = *((int *) b);
	return v - w;
}

void print(FILE *out, void *v) {
	fprintf(out, "%d\n", *((int *)v));
}

int main(void) {

	/* ----------------------------- LIST TESTING ----------------------------- */

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
	printf("----------PRINT--------\n");
	list_print(l, stdout);
	printf("----------PRINT REVERSE --------\n");
	list_print_reverse(l, stdout);

	size_t res = list_size(l);

	printf("SIZE: %d\n",res);

	temp = 10;
	list_node_ptr ka = list_find(l, &temp);
	printf("FIND -> 10: %d\n", *((int *)ka->data_));

	list_node_ptr f = list_front(l);
	printf("FRONT: %d\n", *((int *)f->data_));
	list_node_ptr b = list_back(l);
	printf("BACK: %d\n", *((int *)b->data_));


	list_remove(&l, f);

	printf("----------PRINT--------\n");
	list_print(l, stdout);

	temp = 333;
	list_push_front(&l, &temp);

	printf("----------PRINT--------\n");
	list_print(l, stdout);

	list_remove(&l, b);

	temp = 666;
	list_push_back(&l, &temp);

	printf("----------PRINT--------\n");
	list_print(l, stdout);

	list_node_ptr back = list_pop_back(&l);
	printf("----------PRINT--------\n");
	list_print(l, stdout);

	list_node_ptr front = list_pop_front(&l);
	printf("----------PRINT--------\n");
	list_print(l, stdout);

	free(front);
	free(back);

	list_clear(l);

	/* ----------------------------- AVL TESTING ----------------------------- */
	avl_ptr avl = avl_create(int, cmp, print);

	temp = 10;
	avl_insert(&avl, &temp);
	temp = 30;
	avl_insert(&avl, &temp);
	temp = 4;
	avl_insert(&avl, &temp);
	temp = 20;
	avl_insert(&avl, &temp);
	temp = 7;
	avl_insert(&avl, &temp);
	printf("----------PRINT AVL --------\n");
	avl_print_inorder(avl, stdout);
	temp = 8;
	avl_node_ptr res_avl = avl_find(avl, &temp);
	if (res_avl == NULL)
		printf("NULL CORRECT!\n");

	temp = 10;
	res_avl = avl_find(avl, &temp);
	printf("FOUND: %d -> 10 correct\n",*((int *)res_avl->data_));

	avl_clear(avl);

	/* ----------------------------- HT TESTING ----------------------------- */


  return 0;
}
