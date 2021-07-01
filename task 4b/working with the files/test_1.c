#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_1.h"


int main(){
  int rc;
  Table table;

  dialog_load(&table);


  while(print_menu(), !getCommand(&rc, menu_size)){
    if (menu[rc].func(&table))
    break;
  }

  save(&table);
  dispose(&table);
  return EXIT_SUCCESS;
}


int hash(int key){
  // returns the hash to the key
  return (key%TABLE_SIZE);
}

int nullTable(Table *table){
  // nulls Table->table
  for (int i=0; i<TABLE_SIZE; ++i){
    table->table[i] = NULL;
  }
  return 0;
}


int load(Table *table){
  // loads a table from a file or creates new files of table
  short rc = 0;
  char *tb = (char*)malloc(strlen(table->file_name) + strlen(TABLE_DATA_EXT) + 1);
  strcpy(tb, table->file_name);
  strcat(tb, TABLE_DATA_EXT);
  if ((table->file_data = fopen(tb, "r+b")) == NULL)
    rc = 1;

  char *ts = (char*)malloc(strlen(table->file_name) + strlen(TABLE_SCEP_EXT) + 1);
  strcpy(ts, table->file_name);
  strcat(ts, TABLE_SCEP_EXT);
  if ((table->file_scep = fopen(ts, "r+b")) == NULL)
    rc = 1;

  free(tb);
  free(ts);
  tb = NULL; ts = NULL;

  return rc;
}

int save(Table *table){

  return 0;
}

int find(Table *table, int key){
  return 0;
}

int insert(Table *table, Item *item){
  // add new item to the table
  int h = hash(item->key);
  Item *cur = table->table[h];
  item->next = cur;
  table->table[h] = item;
  fseek(table->file_data, 0, END);
  item->pos = ftail(table->file_data);
  fwrite()
  return 0;
}

int delete(Table *table, int key){
  return 0;
}

int create(Table *table){
  // creates new table files
  short rc = 0;
  char *tb = (char*)malloc(strlen(table->file_name) + strlen(TABLE_DATA_EXT) + 1);
  strcpy(tb, table->file_name);
  strcat(tb, TABLE_DATA_EXT);
  if ((table->file_data = fopen(tb, "w+b")) == NULL)
    rc = 1;

  char *ts = (char*)malloc(strlen(table->file_name) + strlen(TABLE_SCEP_EXT) + 1);
  strcpy(ts, table->file_name);
  strcat(ts, TABLE_SCEP_EXT);
  if ((table->file_scep = fopen(ts, "w+b")) == NULL)
    rc = 1;

  free(tb);
  free(ts);
  tb = NULL; ts = NULL;
  return rc;
}


int printTable(Table *table){
  // print a table to the console
  Item *cur;
  for (int i=0; i<TABLE_SIZE; i++){
    if ((cur = table->table[i]) != NULL){
      printf("================================");
      while (cur->next != NULL){
        printf("info: %s\n", cur->info);
        printf("key: %d\n", cur->key);
        printf("pos: %d\n", cur->pos);
        printf("len: %d\n", cur->len);
        cur = cur->next;
      }
    }
  }
  return 0;
}

int dispose(Table *table){
  // dispose the table
  Item *cur, *prev;
  for (int i=0; i<TABLE_SIZE; i++){
    if ((cur = table->table[i]) != NULL){
      while (cur->next != NULL){
        prev = cur;
        cur = cur->next;
        free(prev);
      }
      free(cur);
    }
    prev = NULL; cur= NULL;
    table->table[i] = NULL;
  }
  return 0;
}


int dialog_exit(Table *table){
  // Return an exit code for the main while
  return 1;
}

int dialog_load(Table *table){
  // a dialog funciton
  // loads a table from a file or creates new files of table
  nullTable(&table);
  getStr(&table->file_name, "Enter a file name -->: ");

  if (load(table))
    create(table);

  return 0;
}

int dialog_save(Table *table){
  return 0;
}

int dialog_find(Table *table){
  return 0;
}

int dialog_insert(Table *table){
  // dialog function
  // add new item to the table
  Item *item;
  create_item(item);

  insert(table, item);

  return 0;
}

int dialog_delete(Table *table){
  return 0;
}

int print_menu(){
  // dialog function
  // Prints menu
  for (int i=0; i<menu_size; ++i){
    printf("%s\n", menu[i].info);
  }
  printf("\n");
  return 0;
}

int create_item(Item *item){
  // creates items' info and key
  getStr(item->info, "Enter item's info --> ");
  getInt(item->key, "Enter item's key -->");
  item->len = strlen(item->info) + 1;
  return 0;


}



int getInt(int *a, char *msg){
  short n;
  printf("%s", msg);
  do {
    n = scanf("%d", a);
    if (n < 0)
      return 1;
    if (n > 0){
      scanf("%*c");
    } else {
      scanf("%*[^\n]");
      printf("Not a number. Try again.\n" );
    }
  } while(n == 0);
  return 0;
}

int getCommand(int *a, int max){
  const char *ermsg = "";
  do{
    printf("%s", ermsg);
    ermsg = "Not a command. Try again\n";
    if(getInt(a, "Enter a command -->: "))
      return 1;
  } while (*a < 0 || *a >= max);
  return 0;
}

int getStr(char **str, char *msg){
  short n;
  char buf[11], *tmp = (char *)malloc(sizeof(char));
  *tmp ='\0';
  int len = 0;
  printf("%s", msg);
  do {
    n = scanf("%10[^\n]", buf);
    if (n < 0){
      free(tmp);
      tmp = NULL;
      return 1;
    }
    if (n > 0){
      len += strlen(buf);
      tmp = (char*)realloc(tmp, sizeof(char)*(len+1));
      strcat(tmp,buf);
    } else {
      scanf("%*c");
    }
  } while (n > 0);
  *str = tmp;
  return 0;
}





















//
