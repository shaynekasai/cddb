/**
 * tree.c
 *
 * This is (eventually) going to be a library for an unbalanced binary tree.
 * These functions provide access in the Node structure provided in tree.h
 *
 */
#include <stdio.h>
#include <string.h>
#include "tree.h"

Tree_Function walk_function; /* function from user */

/**
 * Allocate memory when creating a new node.  
 * 
 * returns: pointer to the new Node
 */
static Node *tree_node_new() {
	Node *node;
	node = (Node *)malloc(sizeof(Node));
	
	if (node == NULL) {
		printf("TREE: out of memory\n");
		exit(-1);
	} else {
		node->key   = NULL;
		node->data  = NULL;
		node->left  = NULL;
		node->right = NULL;		
	}
	return (node);
		
}


/**
 * Search the tree for node->key.  If the key matches the node
 * key then return a pointer to the Node.  This should make it
 * easier to modify data members directly.
 *
 * arguments: Tree *node - the tree to search
 *            char *key  - key to search for
 *            
 * returns: pointer to the node
 */
Node *tree_node_search(Tree *node, char *key) {
	int result;
	
	
	if ((*node) == NULL) {
		return;
	}

	
	result = strcmp((*node)->key, key);
	
	
	if (result < 0) {
		#if DEBUG
			printf("TREE: right %s\n", (*node)->key);
		#endif
		return(tree_node_search((Tree *)&((*node)->right), key));
	} else if ( result > 0) {
		#if DEBUG
			printf("TREE: left %s\n", (*node)->key);
		#endif	
		return(tree_node_search((Tree *)&((*node)->left), key));
	} else {
		#if DEBUG
			printf("TREE: match %s\n", (*node)->key);
		#endif
		return ((*node));
	}
	
}

/**
 * Calls tree_node_new for a new node.  If the node is created then
 * simply connect it to left or right depending on the value.
 * tree_node_add(Tree, "test", data);
 *
 * arguments: Tree *tree - the tree to add the node to
 *            char *key  - the key has to be unique, otherwise it won't insert
 *                         the node.
 *            void *data - pointer to any type
 * 
 * assumption: if key matches anything in the tree, then don't do anything                  
 */
void tree_node_add(Tree *tree, char *key, void *data) {
	int result;
	
	/* we're at the bottom of the tree */
	if ((*tree) == NULL) {
		(*tree) = tree_node_new();
		(*tree)->key  = key;
		(*tree)->data = data;
		return;
	}
	
	result = strcmp((*tree)->key, key);
	
	if (result == 0) {
		return; /* duplicate key */
	}

	if(result < 0) { /* insert right */
		tree_node_add((Tree *)&((*tree)->right), key, data); 
	} else {	 /* insert left */
		tree_node_add((Tree *)&((*tree)->left), key, data);
	}
	
	
}

/**
 * This function will walk the tree in post-order fashion, and
 * it will execute walk_function which is a pointer to a user
 * function.
 *
 * arguments: Tree tree - the tree to walk 
 *
 * assumption: walk_function points to a valid pointer function!
 */
void walk_POSTORDER(Tree tree) {
	if (tree != NULL) {
		walk_POSTORDER((Tree)tree->left);
		walk_POSTORDER((Tree)tree->right);
		walk_function(tree->key, tree->data);
		#if DEBUG
			printf("POSTORDER: %s\n", tree->key);
		#endif
	}
}

/**
 * This function will walk the tree in ordered fashion, and 
 * it will execute walk_function which is a pointer to a user
 * function.
 * 
 * arguments: Tree tree - tree to walk
 * 
 * assumption: walk_function points to a valid pointer function!
 */
void walk_INORDER(Tree tree) {
	if (tree != NULL) {
		walk_INORDER((Tree)tree->left);
		walk_function(tree->key, tree->data);
		#if DEBUG
			printf("INORDER: %s\n", tree->key);
		#endif
		walk_INORDER((Tree)tree->right);
	}
}



/**
 * This is the main wrapper for walk_X functions.  This function should
 * be used instead of calling walk_X directly.
 * 
 * arguments: Tree tree - tree to walk
 *            Tree_Function user_function - user defined function to perform on the
 *            				    walk
 *            ORDER order - order of walking: POSTORDER, INORDER			    
 * 
 */
void tree_walk(Tree tree, Tree_Function user_function, ORDER order) {
	
	walk_function = user_function;
	
	switch(order) {
		case POSTORDER:
			walk_POSTORDER(tree);
			break;
		case INORDER:
			walk_INORDER(tree);
			break;
	}
}



/**
 * It's a good idea to free up the tree after we are done with it.
 * This will only destroy the NODES
 * 
 * Tree tree - pointer to the tree to destroy
 */
void tree_destroy(Tree tree) {
	Node *temp;
	
	if (tree != NULL) {
		temp = (Tree)tree->right;
		
		tree_destroy((Tree)tree->left);
		free(tree->data); /* not sure if this is right */
		free(*tree);
		#if DEBUG
			printf("Deleted: %p\n", *tree);
		#endif
		tree_destroy(temp);

	}
	
}

