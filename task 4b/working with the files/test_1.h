#ifndef __main__
#define __main__

#define TABLE_SIZE 10
#define TABLE_DATA_EXT ".tdat"
#define TABLE_SCEP_EXT ".tsce"

typedef struct Item{
  int key;
  int pos;
  int len;
  struct Item *next;
} Item;

typedef struct {
  Item *table[TABLE_SIZE];
  FILE *file_data, *file_scep;
  char *file_name;
}  Table;

int hash(int);
int nullTable(Table *);

int load(Table *);
int save(Table *);
int find(Table *, int );
int insert(Table *, Item *);
int delete(Table *, int );
int create(Table *);

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
int getCommand(int *, int);
int getStr(char **, char *);

typedef struct{
  int (*func)(Table*);
  char *info;
} Menu;

const int menu_size = 7;

Menu menu[] = {
  {dialog_exit, "0. Exit"},
  {dialog_load, "1. Load table"},
  {dialog_save, "2. Save table"},
  {dialog_find, "3. Find by key"},
  {dialog_insert, "4. Insert item"},
  {dialog_delete, "5. Delete by key"},
  {printTable, "6. Print a table"},

};

#endif






























//
