/**
 * cddb.c
 *
 * CDDB is a program that reads in a binary file and then provides a user
 * with a choice of operations that can be done on the records.
 * 
 * Author: Shayne Kasai
 */
#include <stdio.h>
#include "tree.h"
#include "data.h"
#include "ui.h"

#define USAGE "Usage: cddb filename.dat"

int main(int argc, char *argv[]) {

	Tree *tree = NULL;
	
	if (argc < 2) {
		printf("%s\n", USAGE);	
		exit(-1);
	}
	
	tree = (Tree *)data_init(argv[1]);
	
	/* Load up the UI */
	ui_init(tree, argv[1]); 
	ui_destroy(tree);
	return(0);
}
