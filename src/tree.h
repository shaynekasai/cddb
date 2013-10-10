/* one node to bind them all */
typedef struct {
	char *key;  /* key to identify the node */ 
	void *data; /* whatever */
	struct Node *left;
	struct Node *right;
} Node;

/* the actual tree */
typedef Node *Tree;

typedef enum {
	POSTORDER,
	INORDER
} ORDER;

typedef void (*Tree_Function) (char *key, void *data); /* function pointer for walk */

Node *tree_node_search(Tree *node, char *key);
void tree_node_add(Tree *tree, char *key, void *data);
void walk_POSTORDER(Tree tree);
void walk_INORDER(Tree tree);
void tree_walk(Tree tree, Tree_Function user_function, ORDER order);
void tree_destroy(Tree tree);
