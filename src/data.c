/**
 * data.c - the purpose of this file is to provide the main CDDB functionality.
 * 	    These functions will load data into a tree and allow IO related
 * 	    functions.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "data.h"

/* header files to check against */
#define APP_NAME   "xmlread"
#define VERSION_NO "1.0.0"

static FILE *data_fp; /* this is used to write while walking the tree */

/**
 * This simply allocates a new data structure for the
 * data we get from the dat file.  We have to do this because
 * the tree uses a void * that points to each individual data member.
 *
 * arguments: Data data - data structure to copy into the pointer
 * returns: pointer to the new data structure
 */
Data *data_create_new(Data data) {
	
	Data *new_data = NULL;
	new_data = (Data *)malloc(sizeof(Data));
	
	if (new_data == NULL) {
		printf("CDDB: Could not allocate enough memory to load data\n");
		exit(-1);
	} else {
		/* Create new entries, we should do some validation */
		strcpy(new_data->title,  data.title);
		strcpy(new_data->artist, data.artist);		
		new_data->country = data.country;
		strcpy(new_data->company, data.company);		
		new_data->price   = data.price;
		new_data->year    = data.year;
		return(new_data);
	}
}

/**
 * Check the header to see if this file is somewhat valid.  
 *
 * arguments: FILE *fp - pointer to the file that is opened
 * assumption: file is already open
 *
 * returns: true or false if the file is valid or not respectively
 */
static int data_checksum(FILE *fp) {
	Header header;
	rewind(fp);

	if (fread(&header, sizeof(Header), 1, fp) != 1) {
		return(0);
	}

	if (strcmp(header.app_name, APP_NAME) != 0) {
		return(0);
	}

	if (strcmp(header.version, VERSION_NO) != 0) {
		return(0);
	}

	return(1);
}


/**
 * Take all the information from filename and insert it into the tree
 * return a pointer to a tree.
 * 
 * arguments: char *filename - the name of the file to open
 * returns: Tree * - pointer to the newly created tree
 */
Tree *data_init(char *filename) {
	FILE *fp;
	Data temp;         /* temporary data to pass */
	Data *data = NULL; /* pointer to the malloc'd data */
	int n      = 0;

	Tree *tree = NULL;
	Node *node = NULL;
	
	fp = fopen(filename, "rb");
		
	if (fp == NULL) {
		printf("CDDB: Error reading '%s', make sure the file exists\n");
		exit(-1);
	}

	if (!data_checksum(fp)) {
		printf("CDDB: Invalid file format\n");
		exit(-1);
	}
	
	while(!feof(fp)) {
		n = fread(&temp, sizeof(Data), 1, fp);
		if (n > 0) { /* we have a record */
			data = data_create_new(temp);
			tree_node_add((Tree *)&tree, data->title, data);	
		}
	}
	
	fclose(fp);

	return (tree);
}


/**
 * This does the actual writing to the database. All we have to do
 * is write the Data into the file
 * 
 * arguments: char *key  - unused; used by tree_walk() function pointer call
 * 	      Data *data - data structure to write
 * 	      
 * assumptions: FILE *data_fp 
 */
static void data_write_record(char *key, Data *data) {
	
	if ((fwrite(&(*data), sizeof(Data), 1, data_fp)) != 1) {
		printf("\tCDDB: Could not write record into the database!\n");
		fclose(data_fp);
		return;
	}
	
}

/**
 * Wrapper for the data_write_record function of which we call as
 * a function pointer
 * 
 * arguments: char *filename - the file to write to
 * 	      Tree *tree - the tree to write from
 * 
 */
void data_write(char *filename, Tree *tree) {
	/* write the header */
	Header header;
	strcpy(header.app_name, APP_NAME);
	strcpy(header.version,  VERSION_NO);

	data_fp = fopen(filename, "wb");
	
	rewind(data_fp);
	if ((fwrite(&header, sizeof(Header), 1, data_fp)) != 1) {
		printf("\tCDDB: There was an error writing to the file '%s'\n", filename);
	 	fclose(data_fp);
		return;
	}
	
	tree_walk((Node *)tree, (void *)data_write_record, POSTORDER);	
	fclose(data_fp);
}

/**
 * This simply validates the input from fgets and chops off the '\n'. 
 * It will place a '\0' at the first '\n'.  The size is restricted
 * by size.
 *
 * arguments: char input[] - input buffer to validate and change
 *            int size     - the max amount of characters - '\n'
 * 
 * returns: true or false depending on if the validation was ok.  
 *          NOTE: input[] is changed!
 * 
 * assumptions: fgets(), and the user inputs 1 line only 
 */
int data_validate_record(char input[], int size) {
	int i     = 0;
	int len   = strlen(input);
	char *ref = NULL;

	if (len < 2 || len > size + 1) { 
		return (0);
	}
	
	ref = strchr(input, '\n');
	if (ref != NULL) {
		*ref = '\0';
	}
	
	return (1);
}

