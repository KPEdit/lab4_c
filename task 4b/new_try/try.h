#ifndef __main__
#define __main__

#define TABLE_SIZE 11
#define STEP 1
#define TABLE_DATA_EXT ".tdat"
#define TABLE_SCEP_EXT ".tsce"
#define TABLE_SUME_EXT ".tsum"

typedef enum {FREE, DEL, BUSY} state;

typedef struct Item{
  int key1;
  int key2;
  int ind1;
  int ind2;
  int ver1;
  int ver2;
  int dpos;
  int dlen;
  state busy;
  struct Item *next;
} Item;

typedef struct {
  Item *space1[TABLE_SIZE];
  Item *space2[TABLE_SIZE];
  FILE *file_data, *file_sume, *file_scep;
  char *file_name;
}  Table;

int hash(int);
int nullTable(Table *);

int load(Table *);
int load_space_one(Table *);
int load_space_two(Table *);
int save(Table *);
int close(Table *);
int save_one(Table *);
int save_two(Table *);
int find(Table *, int , int);
int find_one(Table *, int , Item**);
int find_two(Table *, int , Item**);
int insert(Table *, Item *, char *);
int insert_one(Table *, Item *);
int insert_two(Table *, Item *);
int delete_all(Table *, int , int);
int delete_all_one(Table *, int );
int delete_all_two(Table *, int );
int delete_one(Table *, int , int, int);
int delete_one_one(Table *, int , int);
int delete_one_two(Table *, int , int);

int create(Table *);
int compress(Table *);
int update_data(Table *);
int compress_new(Table *);
int compress_new_data(Table *);
int compress_space1(Table *);
int compress_space2(Table *);

int print_list(Item*, FILE *);
int free_list(Item*);
int print_item(Item *, FILE *);
int printTable(Table *);
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

const int menu_size = 10;

Menu menu[] = {
  {dialog_exit, "0. Exit"},
  {dialog_load, "1. Load table"},
  {dialog_save, "2. Save table"},
  {dialog_find, "3. Find by key"},
  {dialog_insert, "4. Insert item"},
  {dialog_delete, "5. Delete by key"},
  {printTable, "6. Print the table"},
  {compress, "7. Compress the table"},
  {compress_new, "8. Compress the table via new file"},
  {close, "9. Close files"}
};

#endif






























//
