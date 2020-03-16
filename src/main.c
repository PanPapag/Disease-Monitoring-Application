#include <stdio.h>

#include "../includes/list.h"
#include "../includes/avl.h"
#include "../includes/hash_table.h"

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
	/*
	printf("------------------------------------------------------------\n");
	list_ptr la = list_create(int*, cmp, print);
	int *skata = malloc(10 * sizeof(int));
	printf("SKATA SIZE: %d\n",sizeof(skata));
	printf("LETS SEE WHAT WE STORE\n");
	for (int i = 0; i < 10; ++i) {
		skata[i] = i;
		printf("%d\n",skata[i]);
	}
	printf("ADDRESS OUT: %p\n", &skata);
	list_push_front(&la, &skata);
	list_node_ptr list_node = list_pop_front(&la);
	int *gamw = *((int *) list_node->data_);
	printf("ADDRESS OUT AGAIN: %p\n", &list_node->data_);
	printf("LETS SEE WHAT WE GET\n");
	for (int i = 0; i < 10; ++i) {
		printf("%d\n",gamw[i]);
	}
	printf("------------------------------------------------------------\n");
	*/

	hash_table_ptr ht = hash_table_create(char*, int, 10, 20, NULL, cmp, print, print);

	hash_table_clear(ht);

  return 0;
}
