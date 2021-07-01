#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "try.h"

/*
hash1, hash2
in Table space1[TMP_TABLE_SIZE]
in Table space2[OTHER_TMP_TABLE_SIZE]
in data.file [TMP_TABLE_SIZE][OTHER_TMP_TABLE_SIZE][ITEMS FROM SPACE1][ITEMS FROM SPACE2]
*/


int main(){
  int rc;
  Table table;

  dialog_load(&table);

  // print_item(table.space2[12], table.file_data);
  // print_item(table.space2[12]->next, table.file_data);

  while(print_menu(), !getCommand(&rc, menu_size, "Enter a command -->: ")){
    if (menu[rc].func(&table))
    break;
  }

  save(&table);
  close(&table);
  dispose(&table);
  printf("%s\n", "Bye!");
  return EXIT_SUCCESS;
}


int hash1(Table *table, int key){
  return (key % table->sz1);
}

int hash2(Table *table, int key){
  return (key % table->sz2);
}


int nullTable(Table *table){
  // null the table
  for (int i=0; i<table->sz1; ++i){
    table->space1[i] = NULL;
  }
  for (int i=0; i<table->sz2; ++i){
    table->space2[i] = NULL;
  }

  return 0;
}


int load(Table *table){
  // loads table from a files
  short rc = 0;
  int fn = strlen(table->file_name);

  char *td = (char*)malloc(fn + strlen(TABLE_DATA_EXT) + 1);
  strcpy(td, table->file_name);
  strcat(td, TABLE_DATA_EXT);
  if ((table->file_data = fopen(td, "a+b")) == NULL)
    rc = 1;

  char *tc = (char*)malloc(fn + strlen(TABLE_SCEP_EXT) + 1);
  strcpy(tc, table->file_name);
  strcat(tc, TABLE_SCEP_EXT);
  if ((table->file_scep = fopen(tc, "r+b")) == NULL)
    rc = 1;

  free(td); free(tc);
  td = NULL; tc = NULL;;

  if (rc)
    return rc;

  // adding data from files to the table
  fseek(table->file_scep, 0, SEEK_SET);
  fread(&table->cur, sizeof(int), 1, table->file_scep);
  fread(&table->sz1, sizeof(int), 1, table->file_scep);
  fread(&table->sz2, sizeof(int), 1, table->file_scep);

  table->space1 = (Item1**)calloc(sizeof(Item1*), table->sz1);
  table->space2 = (Item2**)calloc(sizeof(Item2*), table->sz2);

  nullTable(table);

  load_space_one(table);
  load_space_two(table);
  return 0;
}

int load_space_one(Table *table){
  // load space1
  Item1 cur, *new;
  fseek(table->file_scep, sizeof(int)*3, SEEK_SET);
  for (int i=0; i<table->cur; i++){
    fread(&cur, sizeof(Item1), 1, table->file_scep);
    new = (Item1*)malloc(sizeof(Item1));
    *new = cur;
    table->space1[cur.ind] = new;
  }
  return 0;
}

int load_space_two(Table *table){
  // loads items from file to table->space2
  Item2 ritem, *new, *prev = NULL;
  fseek(table->file_scep, sizeof(int)*3+sizeof(Item1)*table->cur, SEEK_SET);
  while (fread(&ritem, sizeof(int)*5,1,table->file_scep)){
    new = (Item2*)malloc(sizeof(Item2));
    *new = ritem;
    new->next = NULL;
    if (prev != NULL && new->ind == prev->ind){
        prev->next = new;
    } else {
      table->space2[new->ind] = new;
    }
    prev = new;
  }
  return 0;
}

int save(Table *table){
  // saves data to files

  char *tc = (char*)malloc(strlen(table->file_name) + strlen(TABLE_SCEP_EXT) + 1);
  strcpy(tc, table->file_name);
  strcat(tc, TABLE_SCEP_EXT);
  freopen(tc, "w+b", table->file_scep);
  free(tc); tc = NULL;

  fseek(table->file_scep, 0, SEEK_SET);
  fwrite(&table->cur, sizeof(int), 1,table->file_scep);
  fwrite(&table->sz1, sizeof(int), 1,table->file_scep);
  fwrite(&table->sz2, sizeof(int), 1,table->file_scep);

  save_one(table);
  printf("%s\n", "SAVE SPACE1");
  save_two(table);
  printf("%s\n", "SAVE SPACE2");

  return 0;
}

int close(Table *table){
  // closes files
  fclose(table->file_data);
  fclose(table->file_scep);
  return 0;
}

int save_one(Table *table){
  // save items1 to file file_scep

  for (int i=0; i<table->sz1; i++){
    if (table->space1[i] != NULL){
      fseek(table->file_scep, 0, SEEK_END);
      fwrite(table->space1[i], sizeof(Item1), 1, table->file_scep);
    }
  }
  return 0;
}

int save_two(Table *table){
  // save items2 to file file_scep
  Item2 *cur;
  for (int i=0;i<table->sz2; i++){
    if (table->space2[i] != NULL){
      cur = table->space2[i];
      while (cur != NULL){
        fseek(table->file_scep, 0, SEEK_END);
        fwrite(cur, sizeof(int)*5, 1, table->file_scep);
        cur = cur->next;
      }
    }
  }

  return 0;
}


int find(Table *table, int key, int keyType){
  // finds all versions of item in both spaces
  short rc;
  Item *item_list;
  if (keyType)
    rc = find_two(table, key, &item_list);
  else
    rc = find_one(table, key, &item_list);

  if (rc)
    printf("No such item with key = %d in space%d\n", key, keyType+1);
  else{
    printf("%s\n", "Your List");
    print_list(item_list, table->file_data);
    free_list(item_list);
  }
  return rc;
}

int find_one(Table *table, int key, Item **item_list){
  // finds all versions of item in space1 by key
  short rc = 1;
  Item *item, *prev = NULL;
  for (int i=hash1(table, key); i<table->sz1; i+=STEP){
    if (table->space1[i] != NULL){
      if (table->space1[i]->busy == BUSY && table->space1[i]->key == key){
        rc = 0;
        item = (Item*)malloc(sizeof(Item));
        *item->item1 = *table->space1[i];
        *item->item2 = *find_two_by_data(table, item->item1->dpos, item->item1->dlen);
        item->next = prev;
        prev = item;
      }
    }
  }
  *item_list = prev;
  return rc;
}

int find_two(Table *table, int key, Item **item_list){
  // finds all versions of item in space2 by key
  short rc = 1;
  Item *item, *prev = NULL;
  Item2 *cur = table->space2[hash2(table,key)];
  while (cur != NULL){
    if (cur->key == key){
      rc = 0;
      item = (Item*)malloc(sizeof(Item));
      *item->item2 = *cur;
      item->item2->next = NULL;
      *item->item1 = *find_one_by_data(table, item->item2->dpos, item->item2->dlen);
      item->next = prev;
      prev = item;
    }
    cur = cur->next;
  }
  *item_list = prev;
  return rc;
}


Item1 *find_one_by_data(Table *table, int dpos, int dlen){
  //finds
  for (int i=0; i<table->sz1; i++){
    if (table->space1[i] != NULL && table->space1[i]->dpos == dpos && table->space1[i]->dlen == dlen){
      return table->space1[i];
    }
  }
  return NULL;
}

Item2 *find_two_by_data(Table *table, int dpos, int dlen){
  //finds
  Item2 *cur;
  for (int i=0; i<table->sz2; i++){
    cur = table->space2[i];
    while (cur != NULL){
      if (cur->dpos == dpos && cur->dlen == dlen){
        return cur;
      }
      cur = cur->next;
    }
  }
  return NULL;
}


int insert(Table *table, Item *item, char *info){
  // adds new data to the table and writes item info to file
  if (insert_one(table, item->item1)){
    printf("%s\n", "TABLE IS FULL");
    return 1;
  }
  insert_two(table, item->item2);

  fseek(table->file_data, 0, SEEK_END);
  item->item1->dlen = strlen(info)+1;
  item->item1->dpos = ftell(table->file_data);
  item->item2->dlen = strlen(info)+1;
  item->item2->dpos = ftell(table->file_data);
  fwrite(info, sizeof(char), item->item1->dlen, table->file_data);

  free(info);
  info = NULL;
  table->cur++;
  printf("%s\n", "INSERTED");
  return 0;
}

int insert_one(Table *table, Item1 *item){
  // add new item to space1 of table
  int h = hash1(table, item->key)-STEP;
  int ver = 0;
  int rc = 1;
  do{
    h += STEP;
    if (table->space1[h] == NULL) {
      item->ind = h;
      item->ver = ver+1;
      table->space1[h] = item;
      // printf("ind1 = %d\n", table->space1[h]->ind);
      rc = 0;
    } else {
      // table->space1[h]->busy == BUSY &&
      if (item->key == table->space1[h]->key)
      ver = table->space1[h]->ver;
    }
  }while (table->space1[h] != NULL && h<table->sz1 && rc);

  return rc;
}

int insert_two(Table *table, Item2 *item){
  // add new item to space2 of table
  int h = hash2(table, item->key);
  int ver = 0;
  item->ind = h;
  Item2 *cur = table->space2[h];

  while (cur != NULL){
    if (cur->key == item->key){
      ver = cur->ver;
      break;
    }
    cur = cur->next;
  }

  item->ver = ver+1;
  item->next = table->space2[h];
  table->space2[h] = item;
  // printf("ver2 = %d\n", table->space2[h]->ver);
  if (table->space2[h]->next != NULL)
  printf("%d", table->space2[h]->next->key);
  return 0;
}

int delete_all(Table *table, int key, int keyType){
  // delete all item by key in spaces
  short rc = 0;
  if (keyType)
    rc = delete_all_two(table, key);
  else
    rc = delete_all_one(table, key);

  if (rc){
    printf("No such items with key = %d in space%d\n", key, keyType+1);
  } else {
    printf("All items were deleted\n");
  }
  return rc;
}

int delete_all_one(Table *table, int key){
  // delete all item by key in space 1
  short rc = 1;
  Item2 *item = NULL;
  for (int h = hash1(table,key); h < table->sz1; h+=STEP){
    if (table->space1[h] == NULL){
      break;
    }
    if (table->space1[h]->busy == BUSY && table->space1[h]->key == key){
      rc = 0;
      printf("%s\n", "CHANGED");
      table->space1[h]->busy = DEL;
      if (item == NULL){
        item = find_two_by_data(table, table->space1[h]->dpos, table->space1[h]->dlen);
      }
    }
  }
  if (item != NULL)
    delete_all_two(table, item->key);
  return rc;
}

int delete_all_two(Table *table, int key){
  // delete all items by key in space 2

  short rc = 1;
  Item2 *prev = table->space2[hash2(table, key)];
  Item2 *cur = NULL;
  Item1 *item;
  if (prev == NULL)
    return rc;
  while (prev != NULL && prev->key == key){
    table->space2[hash2(table, key)] = prev->next;
    item = find_one_by_data(table, prev->dpos, prev->dlen);
    item->busy = DEL;
    rc = 0;
    free(prev);
    prev = NULL;
    prev = table->space2[hash2(table, key)];
  }
  printf("GE");
  if (prev != NULL)
    cur = prev->next;

  while (cur != NULL){
    if (cur->key == key){
      rc = 0;
      if ((item = find_one_by_data(table, cur->dpos, cur->dlen)) != NULL){
        item->busy = DEL;
      }
      prev->next = cur->next;
      free(cur);
      cur = NULL;
      cur = prev->next;
    } else {
      prev = prev->next;
      cur = prev->next;
    }
  }
  return rc;
}

int delete_one(Table *table, int key, int keyType, int vers){
  // delete one item by key and version in spaces
  short rc = 0;
  if (keyType)
    rc = delete_one_two(table, key, vers);
  else
    rc = delete_one_one(table, key, vers);

  if (rc){
    printf("No such items with key = %d and version = %d in space%d\n", key, vers, keyType+1);
  } else {
    printf("Item was deleted\n");
  }
  return rc;
}

int delete_one_one(Table *table, int key, int vers){
  // delete one item by key and version in space1
  short rc = 1;
  Item2 *item = NULL;
  for (int h = hash1(table, key); h < table->sz1; h+=STEP){
    if (table->space1[h] == NULL){
      break;
    }
    if (table->space1[h]->key == key && table->space1[h]->ver == vers && table->space1[h]->busy == BUSY){
      if (item == NULL)
        item = find_two_by_data(table, table->space1[h]->dpos, table->space1[h]->dlen);
      table->space1[h]->busy = DEL;
      rc = 0;
    }
  }

  if (item != NULL)
    delete_one_two(table, item->key, item->ver);
  return rc;
}

int delete_one_two(Table *table, int key, int vers){
  // delete one item by key and version in space2
  Item1 *item = NULL;
  short rc = 1;
  Item2 *prev = table->space2[hash2(table, key)];
  Item2 *cur;
  if (prev == NULL)
    return rc;
  if (prev != NULL && prev->key == key && prev->ver == vers){
    table->space2[hash2(table, key)] = prev->next;
    item = find_one_by_data(table, prev->dpos, prev->dlen);
    rc = 0;
    free(prev);
    prev = NULL;
  }else
    cur = prev->next;
  while (cur != NULL && rc){
    if (cur->key == key && cur->ver == vers){
      rc = 0;
      prev->next = cur->next;
      item = find_one_by_data(table, cur->dpos, cur->dlen);
      free(cur);
      cur = NULL;
    } else {
      prev = prev->next;
      cur = prev->next;
    }
  }
  if (item != NULL)
    delete_one_one(table, item->key, item->ver);
  return rc;
}


int create(Table *table){
  // creates table files

  short rc = 0;
  int fn = strlen(table->file_name);

  char *td = (char*)malloc(fn + strlen(TABLE_DATA_EXT) + 1);
  strcpy(td, table->file_name);
  strcat(td, TABLE_DATA_EXT);
  if ((table->file_data = fopen(td, "w+b")) == NULL)
    rc = 1;

  char *tc = (char*)malloc(fn + strlen(TABLE_SCEP_EXT) + 1);
  strcpy(tc, table->file_name);
  strcat(tc, TABLE_SCEP_EXT);
  if ((table->file_scep = fopen(tc, "w+b")) == NULL)
    rc = 1;

  table->space1 = (Item1**)calloc(sizeof(Item1*), table->sz1);
  table->space2 = (Item2**)calloc(sizeof(Item2*), table->sz2);

  free(td); free(tc);
  td = NULL; tc = NULL;

  return rc;
}

int compress_new(Table *table){
  // compress table files with help of new files
  compress_space1(table);
  compress_new_data(table);
  // compress_space2(table);

  return 0;
}

int compress_new_data(Table *table){
  // compress data
  char *new_file_name = (char*)malloc(strlen(TABLE_DATA_EXT)+1+strlen(table->file_name)+3);
  strcpy(new_file_name,table->file_name);
  strcat(new_file_name, "new");
  strcat(new_file_name, TABLE_DATA_EXT);
  FILE *new_data_file = fopen(new_file_name, "w+b");
  char *info;

  Item2 *item2;
  for (int i=0; i<table->sz1; i++){
    if (table->space1[i] != NULL && table->space1[i]->busy == BUSY){
      fseek(table->file_data, table->space1[i]->dpos, SEEK_SET);
      info = (char*)malloc(table->space1[i]->dlen);
      fread(info, sizeof(char), table->space1[i]->dlen, table->file_data);
      fseek(new_data_file, 0, SEEK_END);
      item2 = find_two_by_data(table, table->space1[i]->dpos, table->space1[i]->dlen);
      table->space1[i]->dpos = ftell(new_data_file);
      item2->dpos = table->space1[i]->dpos;
      fwrite(info, sizeof(char), table->space1[i]->dlen, new_data_file);
      free(info);
    }
  }

  char *old_file_name = (char*)malloc(strlen(TABLE_DATA_EXT)+1+strlen(table->file_name));
  strcpy(old_file_name,table->file_name);
  strcat(old_file_name, TABLE_DATA_EXT);

  fclose(table->file_data);
  free(table->file_data);
  fclose(new_data_file);
  remove(old_file_name);

  rename(new_file_name,old_file_name);

  new_data_file = fopen(old_file_name, "r+b");
  table->file_data = new_data_file;
  remove(new_file_name);
  free(new_file_name);
  free(old_file_name);

  return 0;
}

int compress_space1(Table *table){
  // compress space1
  for (int i=0; i<table->sz1; i++){
    if (table->space1[i] != NULL && table->space1[i]->busy == DEL){

      free(table->space1[i]);
      table->cur--;
      table->space1[i] = NULL;
    }
  }

    for (int i=0; i<table->sz1; i++){
      if (table->space1[i] != NULL && table->space1[i]->busy == BUSY){
        for (int j=hash1(table, table->space1[i]->key); j < table->sz1; j+=STEP){
          // printf("%d %d\n", i, j);
          if (table->space1[j] == NULL){
            table->space1[i]->ind = j;
            table->space1[j] = table->space1[i];
            // printf("1)%d %d\n", j, table->space1[j]->ind);
            // printf("%s\n", "NEXT");
            table->space1[i] = NULL;
            break;
          }
        }
      }

    }
    printf("%s\n", "FINISHED");
    return 0;
}
/*
int compress_space2(Table *table){ // DONT NEED
  //compress space2
    Item *cur, *prev = NULL, *tmp;
    for (int i=0; i<table->sz2; i++){
      if (table->space2[i] != NULL){
        prev = table->space2[i];
        cur = table->space2[i]->next;
        while(cur != NULL){
          if (cur->busy == DEL){
            prev->next = cur->next;
            tmp = cur;
            cur = cur->next;
            table->space1[tmp->ind1] = NULL;
            free(tmp);
          } else {
            cur = cur->next;
            prev = prev->next;
          }

        }
        if (table->space2[i]->busy == DEL){
          tmp = table->space2[i];
          table->space2[i] = tmp->next;
          table->space1[tmp->ind1] = NULL;

          free(tmp);
          }
      }
      tmp = NULL; prev = NULL; cur = NULL;
    }
    return 0;
}
*/
int print_list(Item *il, FILE *fd){
  // prints list of items
  Item *cur = il;
  while (cur != NULL){
    print_item(cur, fd);
    cur = cur->next;
  }
  return 0;
}

int free_list(Item *il){
  // free mem from items in list
  Item *cur = il->next, *prev = il;
  while (cur != NULL){
    free(prev->item1);
    free(prev->item2);
    free(prev);
    prev = cur;
    cur = cur->next;
  }
  free(prev->item1);
  free(prev->item2);
  free(prev);
  prev = NULL;
  return 0;
}


int print_item(Item *item, FILE *fd){
  //prints item
  printf("key1: %5d                ", item->item1->key);
  printf("key2: %5d\n", item->item2->key);
  printf("ind1: %5d                ", item->item1->ind);
  printf("ind2: %5d\n", item->item2->ind);
  printf("ver1: %5d                ", item->item1->ver);
  printf("ver2: %5d\n", item->item2->ver);
  if (item->item1->busy == BUSY) printf("State: BUSY\n");
  else{if (item->item1->busy == DEL) printf("State: DEL\n");
  else printf("State: FREE\n");
  }
  char *info = (char*)malloc(item->item1->dlen);
  fseek(fd, item->item1->dpos, SEEK_SET);
  fread(info, sizeof(char), item->item1->dlen, fd);
  printf("info: \"%s\"\n\n", info);
  free(info);
  info = NULL;
  return 0;
}

int printTable(Table *table){
  // prints each element of a table
  Item item;
  for (int i=0; i<table->sz1; i++){
    if (table->space1[i] != NULL && table->space1[i]->busy == BUSY){
      item.item1 = table->space1[i];
      item.item2 = find_two_by_data(table, table->space1[i]->dpos, table->space1[i]->dlen);
      print_item(&item, table->file_data);
    }
  }
  return 0;
}

int dispose(Table *table){
  // dispose the table
  for (int i=0; i<table->sz1; i++){
    if (table->space1[i] != NULL){
      table->space1[i] = NULL;
      free(table->space1[i]);
      table->space1[i] = NULL;
    }
  }

  Item2 *cur, *tmp;
  for (int i=0; i<table->sz2; i++){
    cur = table->space2[i];
    if (cur != NULL){
      tmp = cur;
      cur = cur->next;
      free(tmp);
      tmp = NULL;
    }
  }

  free(table->space1);
  free(table->space2);
  free(table->file_data);
  free(table->file_scep);
  free(table->file_name);
  return 0;
}


int dialog_exit(Table *table){
  // dialog function
  // Return an exit code for the main while
  return 1;
}

int dialog_load(Table *table){
  // dialog function
  // loads table
  getStr(&table->file_name, "Enter name of file --> ");

  if (load(table)){
    getInt(&table->sz1, "Enter size of 1 space --> ");
    getInt(&table->sz2, "Enter size of 2 space --> ");
    table->cur = 0;
    create(table);
    nullTable(table);
  }
  printf("%s\n", "LOADDED");

  return 0;
}

int dialog_save(Table *table){
  // dialog function
  // saves data to files
  save(table);
  printf("%s\n", "FILES WERE SAVED");
  return 0;
}

int dialog_find(Table *table){
  // dialog function
  // finds all verions of item by key
  int keyType, key;
  getCommand(&keyType, 2, "Enter type of key (0 - space1, 1 - space2)--> ");
  getInt(&key, "Enter your key --> ");
  find(table, key, keyType);
  return 0;
}

int dialog_insert(Table *table){
  // dialog function
  // add new item to the table
  Item item;
  item.item1 = (Item1*)calloc(sizeof(Item1), 1);
  item.item2 = (Item2*)calloc(sizeof(Item2), 1);
  getInt(&item.item1->key, "Enter key1 --> ");
  getInt(&item.item2->key, "Enter key2 --> ");
  char *info;
  getStr(&info, "Enter item info --> ");
  item.item1->busy = BUSY;
  insert(table, &item, info);

  return 0;
}

int dialog_delete(Table *table){
  // dialog function
  // delete element in table by key
  int keyType, key, a_flag,ver;
  getCommand(&keyType, 2, "Enter type of key (0 - space1, 1 - space2)--> ");
  getCommand(&a_flag, 2, "Delete all keys' versions (0 - No, 1 - Yes)--> ");
  getInt(&key, "Enter your key --> ");
  if (a_flag){
    delete_all(table, key, keyType);
  }else{
    getInt(&ver, "Enter your key's version --> ");
    delete_one(table, key, keyType, ver);
  }
  return 0;
}


int print_menu(){
  // dialog function
  // Prints menu
  printf("\n");
  for (int i=0; i<menu_size; ++i){
    printf("%s\n", menu[i].info);
  }
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

int getCommand(int *a, int max, char *msg){
  const char *ermsg = "";
  do{
    printf("%s", ermsg);
    ermsg = "Not a command. Try again\n";
    if(getInt(a, msg))
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
