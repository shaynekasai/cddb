/**
 * ui.c
 *
 * The purpose of this file is to construct the UI for the CDDB.  This
 * file also handles the binding between the UI and functionality from
 * data.h, so all data related functions should go into data.[c|h], right
 * now there's quite a bit of casting, simply because I forgot what kind
 * of struct I was using to begin with.  That's what you get for using
 * anti-histamines and avoiding the project for a week.
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "tree.h"
#include "data.h"
#include "ui.h"


/* max size of the user input */
#define MAX_SIZE 100

int  UI_RECORD_MODIFIED = 0;           /* This is used to see if any records have been edited */
int  search_counter     = 0;           /* This is to keep track of the display amount */
char *search_artist     = NULL;        /* This is used for ui_search_result */

/**
 * Construct the main UI when the user starts cddb.  This will display
 * the menu only.
 */
static void ui_main_construct() {
	puts("Welcome to the COMP 3425 CD Database\n");
	puts("\t1. Search for CD by title");
	puts("\t2. Search for all CD's by artist");
	puts("\t3. Add a new CD");
	puts("\t4. Modify an existing CD");
	puts("\t5. Print all CD records");
	puts("\t6. Write database to a file\n");
	puts("\t7. Quit\n");
	printf("choice? ");
}

/**
 * Construct the submenu after the user selects "Modify an existing CD".
 * This will display a menu with the title of the record that is to be
 * edited.  Please note that this shouldn't appear unless the record
 * was searched.
 *
 * arguments: char *title - title of the record to edit (node->title)
 */
static void ui_edit_construct(char *title) {
	
	printf("\nCD Database - Edit a Record (%s)\n\n", title);
	puts("\t1. Change the CD's artist");
	puts("\t2. Change the CD's country of origin");
	puts("\t3. Change the CD's company");
	puts("\t4. Change the CD's price");
	puts("\t5. Change the CD's year of manufacture\n");
	puts("\t6. Go back to main menu\n");
	printf("choice? ");
}




/**
 * This will print out one record from Data.  This is to be used in conjunction
 * with search by Title feature.
 *
 * arguments: Data *data: data member (from the tree) to display
 */
static void ui_print_results(Data *data) {
	
	puts("\n\t+===================================================+");
	puts(  "\t| CD Database - Results                             |");
	puts(  "\t+===================================================+");
			
	printf("\t| Title:   %-39.39s  |\n"
 	       "\t| Artist:  %-39.39s  |\n", data->title, data->artist);
	printf("\t| Country: ");
	switch(data->country) {
		case CANADA:
			printf("%-40.40s |", "Canada");
			break;
		case USA:
			printf("%-40.40s |", "USA");
			break;
		case UNITED_KINGDOM:
			printf("%-40.40s |", "UK");
			break;
		case EUROPE:
			printf("%-40.40s |", "EU");
			break;
		default:
			printf("%-40.40s |", "Undefined");
	}
	printf( "\n\t| Company: %-40.40s |\n",     data->company);
	printf(   "\t| Price:   %.2f %-35.35s|\n", data->price,   " ");
	printf(   "\t| Year:    %4.0d %-35.35s |\n",   data->year,    " ");
	puts(     "\t+===================================================+\n");
}

/**
 * Add a title from the user input, the title has to be unique and will not
 * modify unless it is.
 *
 * arguments: char user_input[] - string to change and copy
 * 	      Tree *tree        - tree to search
 * 	      
 * returns: true or false if the title was able to be copied
 */
static int ui_add_title(char user_input[], Tree *tree) {
	Node *search_node = NULL;
	
	printf("\tEnter the title of the CD [max 39 characters]: "); 
	
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 39)) {
			/* search the tree */
			search_node = (Node *)tree_node_search((Tree *)&tree, user_input);
			
			if (search_node->key != NULL) {
				puts("\tAn entry has already been made for this title\n");
				return(0);
			} else {
				return(1);
			}	
		} else {
			puts("\tThe data you entered is invalid.\n");
			return(0);
		}
	} else {
		return(0);
	}

}

/**
 * Add artist from the user input
 *
 * arguments: char user_input[] - string to change and copy
 * 	      
 * returns: true or false if the artist was able to be copied
 */
static int ui_add_artist(char user_input[]) {
	printf("\tEnter the artist's name [max 39 characters]: ");
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 39)) {
			return(1);
		} else {
			puts("\tThe data you entered is invalid.\n");
			return(0);
		}
	} else {
		return(0);
	}
	
}

/**
 * Add company from the user input
 *
 * arguments: char user_input[] - string to change and copy
 * 	      
 * returns: true or false if the company was able to be copied
 */
static int ui_add_company(char user_input[]) {
	printf("\tEnter the company's name [max 29 characters]: ");
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 29)) {
			return(1);
		} else {
			puts("\tThe data you entered is invalid.\n");
			return(0);
		}
	} else {
		return(0);
	}

}

/**
 * Add price from the user input to &price
 *
 * arguments: char user_input[] - string to change and copy
 * 	      float *price - the resulting price
 * 	      
 * returns: true or false if the price was validated and changed 
 */
static int ui_add_price(char user_input[], float *price) {
	
	printf("\tEnter the price of the CD: $");
		
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 8)) {
			*price = atof(user_input);
			return(1);
		} else {
			puts("\tInvalid amount entered.\n");
			return(0);
		}
	} else {
		return(0);
	}
	
	
	
}

/**
 * Add year from the user input to &year
 *
 * arguments: char user_input[] - string to change and copy
 * 	      int *year - the resulting year
 * 	      
 * returns: true or false if the year was validated and changed 
 * assumptions: year is between 1900 and 9999
 */
static int ui_add_year(char user_input[], int *year) {
	printf("\tEnter the year of the CD [YYYY]: ");
	
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 4)) {
			*year = atoi(user_input);
			if (*year < 1900 || *year > 9999) {
				puts("\tThe data you entered is out of range.\n");
				return(0);
			} else {
				return(1);
			}
		} else {
			puts("\tThe data you entered is invalid.\n");
			return(0);
		}
	} else {
		return(0);
	}
}

/**
 * This inputs the user for the country, and it will change &country apporpriately
 *
 * arguments: COUNTRY_TYPE *country - enum of the country:
 *
 * typedef enum {
 *       CANADA,
 *       USA,
 *       UNITED_KINGDOM,
 *       EUROPE,
 * } COUNTRY_TYPE;
 * 	      
 * returns: true or false if the price was validated and changed 
 */
static int ui_add_country(COUNTRY_TYPE *country) {
	int ch = 0;
	
	puts("\tPlease select a country");
	puts("\t\t[C]ANADA");
	puts("\t\t[U]SA");
	puts("\t\tU[N]ITED KINGDOM");
	puts("\t\t[E]UROPE");
	
	printf("\tYour choice [C,U,N,E]: ");
	
	while((ch = getchar()) != EOF) {		 
		while(getchar()	!= '\n') {
			continue;
		}


		switch(toupper(ch)) {
			case 'C':
				*country = CANADA;
				return(1);
				break;
			case 'U':
				*country = USA;
				return(1);
				break;
			case 'N':
				*country = UNITED_KINGDOM;
				return(1);
				break;
			case 'E':
				*country = EUROPE;
				return(1);
				break;
			default:
				puts("\n\tInvalid choice, try again.\n");
		}
	}

}


/**
 * This function will collect all the data from ui_add_x() and create a data structure
 * to be made inserted into the tree.  If there are any errors, ui_add_record will 
 * restart from the beginning.
 *
 * arguments: Tree *tree - The tree to add the new node
 * returns: true or false whether the record was added or not
 */
static int ui_add_record(Tree *tree) {
	char user_input[MAX_SIZE];
	int  ch     = 0;
	int  status = 0;

	int          year;
 	float        price;
	COUNTRY_TYPE country;
	
	Data data;
	Data *data_new = NULL; /* append this to the tree */
	Data *temp = NULL; /* this is only for the pretty results */
	
	puts("\n\t+============================+");
	puts(  "\t| CD Database - Add a new CD |");
	puts(  "\t+============================+\n");
	
	/* title */
	if (ui_add_title(user_input, tree)) {
		strcpy(data.title, user_input);
	} else {
		return (0);
	}

	/* artist */
	if (ui_add_artist(user_input)) {
		strcpy(data.artist, user_input);
	} else {
		return(0);
	}

	
	/* country */
	while (status != 1) {
		status = ui_add_country(&country);
		data.country = country;
	} 
	
	/* company */
	if (ui_add_company(user_input)) {
		strcpy(data.company, user_input);
	} else {
		return(0);
	}

	/* price */
	if (ui_add_price(user_input, &price)) {
		data.price = price;
	} else {
		return(0);
	}
	
	/* year */
	if (ui_add_year(user_input, &year)) {
		data.year = year;
	} else {
		return(0);
	}
	
	temp = &data;
	ui_print_results(temp);
	
	/* are you sure? [Y/N] */
	
	status = 0;
	while (status != 1) {
		printf("\tIs this information correct [y/n]? ");

		ch = getchar();		
		while(getchar()	!= '\n') {
			continue;
		}
		switch(tolower(ch)) {
			case 'y':
				puts("\tCreating new entry...");
				data_new = (Data *)data_create_new(data);
				tree_node_add((Tree *)&tree, data_new->title, data_new);
				puts("\tDone...\n");
				UI_RECORD_MODIFIED = 1; /* set modified state to modified */
				status = 1;
				break;
			case 'n':
				puts("\tChanges not entered.\n");
				status = 1;
				break;
			default:
				puts("\tInvalid choice! Try again");
		}

		
	}
	
	printf("\n\n");
}

/**
 * Basically you have to search for the node you want to edit
 * and then change the values from the ref.  This will allow you
 * to edit a PART of the record depending on the type.
 *
 * arguments: Tree *tree - the tree that contains node (not really used), for now
 * 	      Node *node - the node to edit
 * 	      Edit type  - enum of the edit type:
 * 	      
 * typedef enum {
 *       ARTIST,
 *       COUNTRY, 
 *       COMPANY,
 *       PRICE,
 *       YEAR
 * } Edit;
 *
 * returns: true or false of whether the record was able to be modified (not really used)
 */ 
static int ui_edit_record(Tree *tree, Node *node, Edit type) {
	
	char user_input[MAX_SIZE];
	Data *data = node->data;	
	int          year;
 	float        price;
	COUNTRY_TYPE country;
	
	switch (type) {
		case ARTIST:
			printf("\n\tArtist: %s\n", data->artist);
			if (ui_add_artist(user_input)) {
				strcpy(data->artist, user_input);
				UI_RECORD_MODIFIED = 1; /* set modified state to modified */
			} else {
				return(0);
			}
			break;
		case COUNTRY:
			printf("\n\tCountry: ");
			switch(data->country) {
				case CANADA:
					printf("Canada\n");
					break;
				case USA:
					printf("USA\n");
					break;
				case UNITED_KINGDOM:
					printf("UK\n");
					break;
				case EUROPE:
					printf("EU\n");
					break;
				default:
					printf("Undefined\n");
			}
			
			if (ui_add_country(&country)) {
				data->country = country;
				UI_RECORD_MODIFIED = 1; /* set modified state to modified */
			} else {
				return(0);
			}
			
			break;
		case COMPANY:
			printf("\n\tCompany: %s\n", data->company);
			if (ui_add_company(user_input)) {
				strcpy(data->company, user_input);
				UI_RECORD_MODIFIED = 1; /* set modified state to modified */
			} else {
				return(0);
			}
			break;
		case PRICE:
			printf("\tPrice: %.2f\n", data->price); 
			if (ui_add_price(user_input, &price)) {
				UI_RECORD_MODIFIED = 1; /* set modified state to modified */
				data->price = price;
			} else {
				return(0);
			}
			break;
		case YEAR:
			printf("\tYear: %d\n", data->year);
			if (ui_add_year(user_input, &year)) {
				UI_RECORD_MODIFIED = 1; /* set modified state to modified */
				data->year = year;
			} else {
				return(0);
			}
			break;

	}

	
}


/**
 * Wrapper for tree_node_search for, this will return the resulting node
 * to be displayed under Search by Title from the user's input
 *
 * arguents: Tree *tree - the tree to search
 * assumption: we only need 1 result.
 */
static Node *ui_search_node(Tree *tree) {
	char user_input[MAX_SIZE];
	Node *node = NULL;
	
	printf("\tEnter the title of the album you wish to edit [maximum 39]: ");
	
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 39)) {
			node = (Node *)tree_node_search((Tree *)&tree, user_input);
			return(node);
		} else {
			puts("\tThe data you entered is invalid.\n");
			return;
		}
	} else {
		return;
	}
	
}

/**
 * This function basically allows you to search for a node in the tree
 * and displays it in a pretty table (see ui_print_results()).
 * 
 * arguments: Tree *tree - the tree to pass to ui_search_node
 * 
 * Assumptions: The user knows the exact title
 *
 */
static void ui_search_title(Tree *tree) {
	Node *node = NULL;
	
	puts("\t+-----------------------------------+");
	puts("\t| CD Database - Search CD by title  |");
	puts("\t+-----------------------------------+\n");
	
	node = ui_search_node(tree);

	if (node->key == NULL) {
		puts("\tNo results for your query.\n");
	} else {
		ui_print_results(node->data);
	}
	
	
}

/**
 * Child function of ui_search_artist, passed as function pointer.  This 
 * will simply format the output from Data *data and output it.  This function
 * is called as a pointer through tree_walk
 * 
 * arguments: char *key - member passed from tree_walk
 * 	      Data *data - member passed from tree_walk (see struct Data)
 * 
 */
static void ui_search_result(char *key, Data *data) {
	int ch = 0;
	
	if (search_artist != NULL) { /* we are searching */
		if (strcmp(search_artist, data->artist) != 0) {
			return;
		}
	} 

	/* search_artist */
	printf("%-39s ", data->title);
	printf("%-39s ", data->artist);
	switch(data->country) {
		case CANADA:
			printf("%-10s", "Canada"); 
			break;
		case USA:
			printf("%-10s", "USA");
			break;
		case UNITED_KINGDOM:
			printf("%-10s", "UK");
			break;
		case EUROPE:
			printf("%-10s", "EU");
			break;
		default:
			printf("%-10s", "UNDEFINED");
	}
	printf(" %-29s",    data->company);
	printf("  %-7.2f",  data->price);
	printf(" %-4d\n\n", data->year);

	if (search_counter == 10) {
		puts("Press enter to continue...");
		ch = getchar();
		search_counter = 0;
	} 
	search_counter++;
}


/**
 * This prints out all rows and calls ui_search_result.  This is actually
 * a wrapper to print out the headers...
 *
 * arguments: Tree *tree - the tree to walk
 * assumptions: search_artist has to be NULL!
 */
static void ui_search_all(Tree *tree) {
	search_counter = 0;

	puts("\n\t+------------------------------------+");
	puts(  "\t| CD Database - Print all CD records |");
	puts(  "\t+------------------------------------+");
	printf("\n");
	
	printf("\n%-39s %-39s "
               "%-10s %-29s "
	       "%-7s %-5s\n"
	       "======================================================="
	       "======================================================="
	       "========================\n",
	       "TITLE", "ARTIST", "COUNTRY", "COMPANY", "PRICE", "YEAR");

	search_artist = NULL;
	tree_walk((Node *)tree, (void *)ui_search_result, INORDER);			
}

/**
 * Search by artist and display (if applicable) multiple rows of
 * the search results.  In order for this to work, search_artist can
 * not be NULL
 * 
 * arguments: Tree *tree - the tree to walk
 * 
 */
static void ui_search_artist(Tree *tree) {
	char user_input[MAX_SIZE];
	
	search_counter = 0;
	
	puts("\t+-----------------------------------+");
	puts("\t| CD Database - Search CD by artist |");
	puts("\t+-----------------------------------+\n");

	printf("\tEnter the artist of the albums you wish to search [maximum 39]: ");
	
	if (fgets(user_input, MAX_SIZE, stdin) != NULL) {
		if (data_validate_record(user_input, 39)) {
			printf("\n%-39s %-39s "
		               "%-10s %-29s "
			       "%-7s %-5s\n"
			       "======================================================="
			       "======================================================="
			       "========================\n",
			       "TITLE", "ARTIST", "COUNTRY", "COMPANY", "PRICE", "YEAR");

			search_artist = user_input;
			tree_walk((Node *)tree, (void *)ui_search_result, POSTORDER);			
		} else {
			puts("\tThe data you entered is invalid.\n");
			return;
		}
	} else {
		return;
	}

}

/**
 * This is the actual functionality for the submenu "Modify a record", depending
 * on which option the user chooses, it will call ui_edit_record with the 
 * correct enum Edit
 *
 * arguments: Tree *tree - the tree where the record is stored
 *
 * assumptions:
 * typedef enum {
 *       ARTIST,
 *       COUNTRY, 
 *       COMPANY,
 *       PRICE,
 *       YEAR
 * } Edit;
 */
static void ui_menu_edit(Tree *tree) {
	int  ch     = 0;
	char status = 0;
	
	Node *search_node = NULL; /* we have to search for the node */
	
	search_node = ui_search_node(tree);
	
	if (search_node->key == NULL) {
		puts("\tThe album was not found.\n");
		return;
	}
	
	
	ui_edit_construct(search_node->key);
	while ((ch = getchar()) != EOF) {
		while(getchar() != '\n') {
			continue;
		}

		switch(ch - '0') {
			case 1:
				ui_edit_record(tree, search_node, ARTIST);
				break;
			case 2:
				ui_edit_record(tree, search_node, COUNTRY);
				break;
			case 3:
				ui_edit_record(tree, search_node, COMPANY);
				break;
			case 4:
				ui_edit_record(tree, search_node, PRICE);
				break;
			case 5:
				ui_edit_record(tree, search_node, YEAR);
				break;
			case 6:
				return;
			default:
		}

		if (status) {
			break;
		} else {
			ui_edit_construct(search_node->key);
		}
	}
	
}

/**
 * This is called if the user decides to write the file.  Simply check
 * for y/n and write to argv[1] from main().
 *
 * arguments: char *argv - filename to write to
 * 	      Tree *tree - the tree to write from
 */
static void ui_write_file(char *argv, Tree *tree) {
	int ch     = 0;
	int status = 0;

	while (status != 1) {
		printf("\tAre you sure you want to write to '%s' [y/n]? ", argv);

		ch = getchar();		
		while(getchar()	!= '\n') {
			continue;
		}
		switch(tolower(ch)) {
			case 'y':
				puts("\tWriting to file...");
				data_write(argv, tree);
				UI_RECORD_MODIFIED = 0; /* set modified state to modified */
				puts("\tDone...\n");
				status = 1;
				break;
			case 'n':
				puts("\tChanges not saved.\n");
				status = 1;
				break;
			default:
				puts("\tInvalid choice! Try again");
		}

		
	}
	
	printf("\n\n");
}

/**
 * Wrapper for tree_destroy
 *
 * arguments: Tree *tree - the tree to destroy
 */
void ui_destroy(Tree *tree) {
	tree_destroy((Node *)tree);
}


/**
 * Checks UI_RECORD_MODIFIED to see if it set to modified state
 * (see assumptions)
 *
 * assumptions: 1 - the state is modified
 *              0 - the state is not modified
 */
static int ui_quit() {
	int status = 0;
	int ch     = 0;
	
	if (UI_RECORD_MODIFIED == 1) {
		while (status != 1) {
			printf("\tData modified, are you sure you want to quit [y/n]? ");

			ch = getchar();		
			while(getchar()	!= '\n') {
				continue;
			}
			switch(tolower(ch)) {
				case 'y':
					return(1);
				case 'n':
					return(0);
				default:
					puts("\tInvalid choice! Try again");
			}

		
		}

	} else {
		return(1);
	}
}

/**
 * Main external UI call, this will get the users input and execute the appropriate
 * function.  This is the first thing the user uses.
 *
 * arguments: Tree *tree - current tree being used
 *            char *argv - filename (to write to)
 */
void ui_init(Tree *tree, char *argv) {
	int ch     = 0;
	int status = 0;
	
	ui_main_construct();

	while ((ch = getchar()) != EOF) {
		while(getchar() != '\n') {
			continue;
		}
		switch(ch - '0') { /* not exactly portable */
			case 1:
				ui_search_title(tree);
				break;
			case 2:
				ui_search_artist(tree);
				break;
			case 3:
				while(!ui_add_record(tree));
				break;
			case 4:
				ui_menu_edit(tree);
				break;
			case 5:
				ui_search_all(tree);
				break;
			case 6:
				ui_write_file(argv, tree);
				break;
			case 7:
				if (ui_quit()) {
					return;
				} 	
				break;
			default:
		}

		ui_main_construct();
		
	}

	
	
}

