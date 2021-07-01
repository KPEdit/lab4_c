#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "try.h"

/*
stuct Item 1, 2
hash1, hash2
in Table space1[TMP_TABLE_SIZE]
in Table space2[OTHER_TMP_TABLE_SIZE]
in data.file [TMP_TABLE_SIZE][OTHER_TMP_TABLE_SIZE][ITEMS FROM SPACE1][ITEMS FROM SPACE2]
*/


int main(){
  int rc;
  Table table;

  nullTable(&table);
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


int hash(int key){
  // returns the hash to the key
  return (key%TABLE_SIZE);
}

int nullTable(Table *table){
  // null the table
  for (int i=0; i<TABLE_SIZE; ++i){
    table->space1[i] = NULL;
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
  load_space_two(table);
  printf("%s\n", "LOADDED");
  return 0;
}


int load_space_two(Table *table){
  // loads items from file to table->space2
  Item ritem, *new, *prev = NULL;
  while (fread(&ritem, sizeof(state)+sizeof(int)*8,1,table->file_scep)){
    new = (Item*)malloc(sizeof(Item));
    *new = ritem;
    new->next = NULL;
    table->space1[ritem.ind1] = new;
    if (prev != NULL){
      if (prev->ind2 == new->ind2){
        prev->next = new;
      }else{
        table->space2[new->ind2] = new;
      }
    }else{
      table->space2[new->ind2] = new;
    }
    prev = new;
  }
  return 0;
}

int save(Table *table){
  // saves data to files
  save_two(table);
  return 0;
}

int close(Table *table){
  // closes files
  fclose(table->file_data);
  fclose(table->file_scep);
  return 0;
}

int save_two(Table *table){
  // save data to file file_scep
  char *tc = (char*)malloc(strlen(table->file_name) + strlen(TABLE_SCEP_EXT) + 1);
  strcpy(tc, table->file_name);
  strcat(tc, TABLE_SCEP_EXT);
  freopen(tc, "w+b", table->file_scep);
  free(tc); tc = NULL;
  Item *cur;
  for (int i=0;i<TABLE_SIZE; i++){
    if (table->space2[i] != NULL){
      cur = table->space2[i];
      while (cur != NULL){
        fseek(table->file_scep, 0, SEEK_END);
        fwrite(cur, sizeof(state)+sizeof(int)*8, 1, table->file_scep);
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
  for (int i=hash(key); i<TABLE_SIZE; i+=STEP){
    if (table->space1[i] != NULL){
      if (table->space1[i]->busy == BUSY && table->space1[i]->key1 == key){
        rc = 0;
        item = (Item*)malloc(sizeof(Item));
        *item = *table->space1[i];
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
  Item *item, *cur = table->space2[hash(key)], *prev = NULL;
  while (cur != NULL){
    if (cur->busy == BUSY && cur->key2 == key){
      rc = 0;
      item = (Item*)malloc(sizeof(Item));
      *item = *cur;
      item->next = prev;
      prev = item;
    }
    cur = cur->next;
  }
  *item_list = prev;
  return rc;
}

int insert(Table *table, Item *item, char *info){
  // adds new data to the table and writes item info to file
  if (insert_one(table, item)){
    printf("%s\n", "TABLE IS FULL");
    return 1;
  }
  insert_two(table, item);

  fseek(table->file_data, 0, SEEK_END);
  item->dlen = strlen(info)+1;
  item->dpos = ftell(table->file_data);
  fwrite(info, sizeof(char), item->dlen, table->file_data);

  free(info);
  info = NULL;
  printf("%s\n", "INSERT");
  return 0;
}

int insert_one(Table *table, Item *item){
  // add new item to space1 of table
  int h = hash(item->key1)-STEP;
  int ver = 0;
  int rc = 1;
  do{
    h += STEP;
    if (table->space1[h] == NULL) {
      item->ind1 = h;
      item->ver1 = ver+1;
      table->space1[h] = item;
      rc = 0;
      break;
    } else {
      if (item->key1 == table->space1[h]->key1)
      ver = table->space1[h]->ver1;
    }
  }while (table->space1[h] != NULL && h<TABLE_SIZE && rc);

  return rc;
}

int insert_two(Table *table, Item *item){
  // add new item to space2 of table
  int h = hash(item->key2);
  int ver = 0;
  item->ind2 = h;
  Item *cur = table->space2[h];

  while (cur != NULL){
    if (cur->key2 == item->key2){
      ver = cur->ver2;
      break;
    }
    cur = cur->next;
  }

  item->ver2 = ver+1;
  item->next = table->space2[h];
  table->space2[h] = item;

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
  for (int h = hash(key); h < TABLE_SIZE; h+=STEP){
    if (table->space1[h] == NULL){
      break;
    }
    if (table->space1[h]->key1 == key){
      table->space1[h]->busy = DEL;
      rc = 0;
    }
  }

  return rc;
}

int delete_all_two(Table *table, int key){
  // delete all items by key in space 2

  short rc = 1;
  Item *cur = table->space2[hash(key)];
  while (cur != NULL){
    if (cur->key2 == key){
      // printf("%s\n", "da");
      rc = 0;
      cur->busy = DEL;
    }
    cur = cur->next;
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
  for (int h = hash(key); h < TABLE_SIZE; h+=STEP){
    if (table->space1[h] == NULL){
      break;
    }
    if (table->space1[h]->key1 == key && table->space1[h]->ver1 == vers && table->space1[h]->busy == BUSY){
      table->space1[h]->busy = DEL;
      rc = 0;
    }
  }

  return rc;
}

int delete_one_two(Table *table, int key, int vers){
  // delete one item by key and version in space2

  short rc = 1;
  Item *cur = table->space2[hash(key)];
  while (cur != NULL && rc){
    if (cur->key2 == key && cur->ver2 == vers && cur->busy == BUSY){
      rc = 0;
      cur->busy = DEL;
    }
    cur = cur->next;
  }
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

  free(td); free(tc);
  td = NULL; tc = NULL;

  return rc;
}

int compress(Table *table){
  // compress the table
  // compress_space1(table);
  compress_space2(table);
  update_data(table);
  printf("%s\n", "COMPRESSED!");
  return 0;
}

int update_data(Table *table){
  // update file_data
  fseek(table->file_data, 0, SEEK_END);
  int start = ftell(table->file_data);
  int end;
  char *info;

  for (int i=0; i<TABLE_SIZE; i++){
    if (table->space1[i] != NULL){
      fseek(table->file_data, 0, SEEK_END);
      end = ftell(table->file_data);
      info = (char*)malloc(table->space1[i]->dlen);
      fseek(table->file_data, table->space1[i]->dpos, SEEK_SET);
      fread(info,sizeof(char), table->space1[i]->dlen, table->file_data);
      table->space1[i]->dpos = end - start;
      fseek(table->file_data, 0, SEEK_END);
      fwrite(info, sizeof(char), table->space1[i]->dlen, table->file_data);
      free(info);
      info = NULL;
    }
  }

  fseek(table->file_data, 0, SEEK_END);
  end = ftell(table->file_data);

  info = (char*)malloc(end-start);


  fseek(table->file_data, start, SEEK_SET);
  fread(info, sizeof(char), end-start, table->file_data);

  int fn = strlen(table->file_name);
  char *td = (char*)malloc(fn + strlen(TABLE_DATA_EXT) + 1);
  strcpy(td, table->file_name);
  strcat(td, TABLE_DATA_EXT);
  freopen(td, "w+b", table->file_data);

  fseek(table->file_data, 0, SEEK_SET);
  fwrite(info, sizeof(char), end-start, table->file_data);
  free(info);
  info = NULL;
  return 0;
}

int compress_new(Table *table){
  // compress table files with help of new files
  compress_new_data(table);
  compress_space2(table);
  // compress_space1(table);

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
  for (int i=0; i<TABLE_SIZE; i++){
    if (table->space1[i] != NULL && table->space1[i]->busy == BUSY){
      fseek(table->file_data, table->space1[i]->dpos, SEEK_SET);
      info = (char*)malloc(table->space1[i]->dlen);
      fread(info, sizeof(char), table->space1[i]->dlen, table->file_data);
      fseek(new_data_file, 0, SEEK_END);
      table->space1[i]->dpos = ftell(new_data_file);
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
    for (int i=0; i<TABLE_SIZE; i++){
      if (table->space1[i] != NULL){
        if (table->space1[i]->busy == BUSY){
          for (int j=hash(table->space1[i]->key1); j < TABLE_SIZE; j+=STEP){
            if (table->space1[j] == NULL){
              table->space1[j] = table->space1[i];
              table->space1[i] = NULL;
            } else {
              if (table->space1[j]->busy == DEL){
                free(table->space1[j]);
                table->space1[j] = table->space1[i];
                table->space1[i] = NULL;
              }
            }
          }
        }
      }
    }
    return 0;
}

int compress_space2(Table *table){
  //compress space2
    Item *cur, *prev = NULL, *tmp;
    for (int i=0; i<TABLE_SIZE; i++){
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
    free(prev);
    prev = cur;
    cur = cur->next;
  }
  free(prev);
  prev = NULL;
  return 0;
}


int print_item(Item *item, FILE *fd){
  //prints item
  printf("key1: %5d                ", item->key1);
  printf("key2: %5d\n", item->key2);
  printf("ind1: %5d                ", item->ind1);
  printf("ind2: %5d\n", item->ind2);
  printf("ver1: %5d                ", item->ver1);
  printf("ver2: %5d\n", item->ver2);
  // printf("len: %5d                ", item->dlen);
  // printf("pos: %5d\n", item->dpos);
  if (item->busy == BUSY) printf("State: BUSY\n");
  else{if (item->busy == DEL) printf("State: DEL\n");
  else printf("State: FREE\n");
  }
  char *info = (char*)malloc(item->dlen);
  fseek(fd, item->dpos, SEEK_SET);
  fread(info, sizeof(char), item->dlen, fd);
  printf("info: \"%s\"\n\n", info);
  free(info);
  info = NULL;
  return 0;
}

int printTable(Table *table){
  // prints each element of a table
  for (int i=0; i<TABLE_SIZE; i++){
    if (table->space1[i] != NULL){
      print_item(table->space1[i], table->file_data);
    }
  }
  return 0;
}

int dispose(Table *table){
  // dispose the table
  for (int i=0; i<TABLE_SIZE; i++){
    if (table->space1[i] != NULL){
      table->space1[i]->next = NULL;
      free(table->space1[i]);
      table->space1[i] = NULL;
    }
  }
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

  if (load(table))
    create(table);

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
  Item *item = (Item*)calloc(sizeof(Item), 1);
  getInt(&item->key1, "Enter key1 --> ");
  getInt(&item->key2, "Enter key2 --> ");
  char *info;
  getStr(&info, "Enter item info --> ");
  item->busy = BUSY;
  insert(table, item, info);

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
