
/* country enumeration */
typedef enum {
	CANADA,
	USA,
	UNITED_KINGDOM,
	EUROPE,
} COUNTRY_TYPE;

/* data member for CD_RECORD */
typedef struct {
	char         title[40];
	char         artist[40];
	COUNTRY_TYPE country;
	char         company[30];
	float        price;
	int          year;
} Data;

/* checksum structure */
typedef struct {
	char app_name[16];
	char version[10];
} Header;

Data *data_create_new(Data data);
Tree *data_init(char *filename);
void data_write(char *filename, Tree *tree);
int data_validate_record(char input[], int size);
