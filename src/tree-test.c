#include <stdio.h>
#include "tree.h"

typedef struct {
	char name[40];
	char book[30];
} Data;

void try(char *key, Data *data) {
	printf("%s\n", key);
}

int main(int argc, char *argv[]) {
	Tree *test = NULL;
	Node *test_node = NULL;
	Data *foo;
	Data *blah;
	
	char *value = "abcdefg";
	char *value2 = "ab";
	char *value3 = "acccaaaaaaa";
	char *value4 = "abcdefgh";
	
	foo = (Data *)malloc(sizeof(Data));
	strcpy(foo->name, "value");
	
	/* tree_node_add(&test, data, value) */
	tree_node_add(&test, value, foo);
	tree_node_add(&test, value2, NULL);
	tree_node_add(&test, value3, NULL);
	tree_node_add(&test, value4, NULL);

	test_node = (Node *)tree_node_search(&test, "abcdefg");
	//printf("%s %s\n", test_node->key, test_node->data);
	blah = test_node->data;
	strcpy(blah->name, "newvalue");
	
	test_node = (Node *)tree_node_search(&test, "abcdefg");
	blah = test_node->data;
	
	printf("%s\n", blah->name);
	
	test_node = NULL;
	test_node = (Node *)tree_node_search(&test, "jjjjj");
	printf("%p\n", test_node);
	
	tree_walk(test, try, INORDER);
	
	
	return (0);
}
