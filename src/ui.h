/* this is to facilitate the menu choices */
typedef enum {
	ARTIST,
	COUNTRY, 
	COMPANY,
	PRICE,
	YEAR
} Edit;

void ui_destroy(Tree *tree);
void ui_init(Tree *tree, char *argv);


