#ifndef __main__
#define __main__

#define STEP 1
#define TABLE_DATA_EXT ".tdat"
#define TABLE_SCEP_EXT ".tsce"

typedef enum {FREE, DEL, BUSY} state;

typedef struct {
  int key;
  int ver;
  int ind;
  int dpos;
  int dlen;
  state busy;
} Item1;

typedef struct Item2 {
  int key;
  int ver;
  int ind;
  int dpos;
  int dlen;
  struct Item2 *next;
} Item2;


typedef struct {
  int cur;
  int sz1;
  int sz2;
  Item1 **space1;
  Item2 **space2;
  // Item **ex_space;
  FILE *file_data, *file_scep;
  char *file_name;
}  Table;


typedef struct Item{
  Item1 *item1;
  Item2 *item2;
  struct Item *next;
} Item;
int hash1(Table *, int);
int hash2(Table *, int);
int nullTable(Table *);

int load(Table *);
int load_space_one(Table *);
int load_space_two(Table *); // check
int save(Table *);
int close(Table *);
int save_one(Table *);
int save_two(Table *);
int find(Table *, int , int);
int find_one(Table *, int , Item**);
int find_two(Table *, int , Item**); // check

Item1 *find_one_by_data(Table *, int, int);// check
Item2 *find_two_by_data(Table *, int, int);// check

int insert(Table *, Item *, char *);
int insert_one(Table *, Item1 *); // check
int insert_two(Table *, Item2 *); // check
int delete_all(Table *, int , int);
int delete_all_one(Table *, int ); // check
int delete_all_two(Table *, int ); // check
int delete_one(Table *, int , int, int);
int delete_one_one(Table *, int , int); // check
int delete_one_two(Table *, int , int); // check

int create(Table *);
int compress_new(Table *);
int compress_new_data(Table *); // CHECK
int compress_space1(Table *); // CHECK
// int compress_space2(Table *); // DONT NEED

int print_list(Item*, FILE *);
int free_list(Item*); // CHECK
int print_item(Item *, FILE *);
int printTable(Table *); // check
int dispose(Table *);

int dialog_exit(Table *);
int dialog_load(Table *);
int dialog_save(Table *);
int dialog_find(Table *);
int dialog_insert(Table *);
int dialog_delete(Table *);
int print_menu();
int create_item(Item *);

int getInt(int *, char *);
int getCommand(int *, int, char *);
int getStr(char **, char *);

typedef struct{
  int (*func)(Table*);
  char *info;
} Menu;

const int menu_size = 6;

Menu menu[] = {
  {dialog_exit, "0. Exit"},
  {dialog_find, "1. Find by key"},
  {dialog_insert, "2. Insert item"},
  {dialog_delete, "3. Delete by key"},
  {printTable, "4. Print the table"},
  {compress_new, "5. Compress the table via new file"},
  // {dialog_load, "1. Load table"},
  // {dialog_save, "2. Save table"},
  // {close, "8. Close files"}
};

#endif






























//
