#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "try_1.h"


int main(){
  Table table;
  unsigned short rc;
  const message *menu = &dialogList[0];
  nullTable(&table);
  while (printMenu(menu, 5), !getCommand(&rc, 5)){
    if(menu[rc].func(&table) < 0) {
      break;
    }
  };
  printf("%s\n", "Bye!");
  return EXIT_SUCCESS;
}


// DIALOG SECTION
int dialogExit(Table *table){
  return -1;
}

int dialogFind(Table *table){
  Item *listHead = (Item*)malloc(sizeof(Item));
  keys key;
  keyType kt = getKeyType();
  getKey(&key, "Enter your key:\n");
  if (Find(table, &listHead, key, kt)){
    printf("%s\n", "No such Element.");
    return 1;
  }
  printList(listHead);
  disposeList(listHead);
  return 0;
}
int dialogDel(Table *table){
  keys key;
  unsigned short all_flag;
  keyType kt = getKeyType();
  getKey(&key, "Enter your key:\n");
  printf("Would you like to delete all version of table element? (1 - Yes, 0 - No)\n");
  getCommand(&all_flag, 2);
  return 0;
}

int dialogInsert(Table *table){
  Item *item = (Item*)malloc(sizeof(Item));
  if (getKey(&item->key1, "Enter key1:\n")) return 1;
  if (getKey(&item->key2, "Enter key2:\n")) return 1;
  if (getString(&item->info, "Enter info:\n")) return 1;
  int v = findVersTwo(table, item->key2);
  if (v < 0) v = 0;
  item->vers = v+1;
  item->state = EQU;
  if (Insert(table, item)) {
    printf("%s\n", "TABLE IS FULL!!!");
  }
  return 0;
}

int dialogPrintTable(Table *table){
  printf("Your table:\n" );
  PrintTable(table);
  return 0;
}

int printMenu(const message *dl, const int N){
  printf("\n=====MENU=====\n");
  for (int i=0; i<N;i++)
    printf("%3d) %s\n", i, dl[i].msg);
  printf("==============\n");
  return 0;
}


// FUNCTION SECTION
int nullTable(Table *table){
  for (int i=0; i<TABLE_SIZE;i++){
    table->space1[i] = NULL;
    table->space2[i] = NULL;
  }
  return 0;
}

int dispose(Table *table){
  for (int i=0;i<TABLE_SIZE;i++){
    if (table->space1[i] != NULL){
      free(table->space1[i]->info);
      table->space1[i]->info = NULL;
      free(table->space1[i]);
      table->space1[i] = NULL;
    }
  }
  return 0;
}

int disposeList(Item *item){
  Item *prev = item, *next = item->next;
  while (next != NULL){
    free(prev);
    prev = next;
    next = next->next;
  }
  free(prev);
  return 0;
}

index hashOne(keys key, int i){
  // check work of this function
  return ((((key) % M)+ i)%TABLE_SIZE);
}

int findVers(Table *table, keys key, keyType kType){
  // return -1 if table doesn't have Item with key
  // otherwise return index
  if (kType == FIRST) return findVersOne(table, key);
  return findVersTwo(table, key);
}

int findVersOne(Table *table,keys key){
  keys key2;
  if (getSecondKey(table, key, &key2)) return -1;
  return findVersTwo(table, key2);
}

int findVersTwo(Table *table,keys key){
  int i = 0;
  index ind = hashTwo(key, i);
  if (table->space2[ind] == NULL) return -1;
  while (table->space2[ind]->key2 != key){
    i += 1;
    ind = hashTwo(key, i);
  }
  return table->space2[ind]->vers;
}

index hashTwo(keys key, int i){
  return ((((key) % M)+ i)%TABLE_SIZE);
}

int getSecondKey(Table *table, keys key, keys *tp){
  int i = 0;
  index ind;
  while (ind = hashOne(key, i), i < TABLE_SIZE){
    if (table->space1[ind] == NULL) return 1;
    if (table->space1[ind]->key1 == key){
      *tp = table->space1[ind]->key2;
      return 0;
    }
    i++;
  }
  return 1;
}

int printList(Item *list){
  Item *l = list;
  while(l != NULL){
    printf("key 1: %d\n", l->key1);
    printf("key 2: %d\n", l->key2);
    printf("info: %s\n", l->info);
    printf("index 1: %d\n", l->ind1);
    printf("index 2: %d\n", l->ind2);
    printf("version: %d\n", l->vers);
    l = l->next;
  }
  return 0;
}

Item *findItem(Table *table, keys key){
  int i = 0;
  index ind = hashTwo(key, i);
  while (table->space2[ind]->key2 != key){
    i += 1;
    ind = hashTwo(key, i);
  }
  return table->space2[ind];
}

int Find(Table *table, Item **list, keys key, keyType kType){
  // doesn't work
  keys key2;
  if (getSecondKey(table, key, &key2)) return 1;
  Item *head = (Item*)malloc(sizeof(Item));
  Item *tail = head, *headp = head;
  *head = *findItem(table, key2);

  while (tail != NULL){
    if (tail->key2 == key2){
      Item *new_item = (Item*)malloc(sizeof(Item));
      *new_item = *tail;
      headp->next = new_item;
      headp = headp->next;
    }
    tail = tail->next;
  }
  headp->next = NULL;
  *list = head;
  return 0;
}

int Del(Table *table, keys key, keyType kType, unsigned short all_flag){
  return 0;
}

int Insert(Table *table, Item *item){

  if (InsertToSpaceOne(table, item)) return 1;
  InsertToSpaceTwo(table, item);
  return 0;
}

int PrintItemByIndex(Table *table, index ind){
  printf("key 1: %d\n", table->space1[ind]->key1);
  printf("key 2: %d\n", table->space1[ind]->key2);
  printf("info: %s\n", table->space1[ind]->info);
  printf("index 1: %d\n", table->space1[ind]->ind1);
  printf("index 2: %d\n", table->space1[ind]->ind2);
  printf("version: %d\n", table->space1[ind]->vers);
  return 0;
}

int PrintTable(Table *table){
  for (int i=0;i<TABLE_SIZE;i++){
    if(table->space1[i] != NULL && table->space1[i]->state == EQU){
      PrintItemByIndex(table, i);
      printf("\n");
    }
  }
  return 0;
}


int FindInSpaceOne(Table *table, keys key, Item *list){
  // reorg to list NOT TO VECTOR
  int times = 0;
  index len = findVersOne(table, key);
  Item *listPointer = (Item*)calloc(len, sizeof(Item)); // this

  index ind = hashOne(key, STEP*times);
  if (table->space1[ind] == NULL){
    free(listPointer);
    listPointer = NULL;
    return 1;
  }
  while (times < len){
    if ((table->space1[ind]->key1 == key && table->space1[ind]->state == EQU)){
      listPointer[times] = *table->space1[ind];
    }
    times +=1;
    ind = hashOne(key, STEP*times);
  }
  return 0;
}

int FindInSpaceTwo(Table *table, keys key, Item *list){
  // REOGT TO LIST NOT TO VECTOR
  index len = findVersTwo(table, key);
  return 0;
}

int InsertToSpaceOne(Table *table, Item *item){
  index ktemp;
  keys key = item->key1;
  short work = 1;
  int i = 0;
  while (work){
    ktemp = hashOne(key, i);
    if (ktemp+1 >= TABLE_SIZE) return 1;
    if (table->space1[ktemp] == NULL || table->space1[ktemp]->state == DEL){
      table->space1[ktemp] = item;
      item->ind1 = ktemp;
      work = !work;
    }
    i += STEP;
  }
  return 0;
}

int InsertToSpaceTwo(Table *table, Item *item){

  keys key = item->key2;
  index ktemp = hashTwo(key,0);
  if (table->space2[ktemp] == NULL){
    item->ind2 = ktemp;
    item->next = NULL;
    table->space2[ktemp] = item;

    return 0;
  }
  Item *ip = table->space2[ktemp];
  item->next = ip;
  table->space2[ktemp] = item;
  return 0;
}


// BASE FUNCTIONS
int getKey(keys *a, const char *text){
  short n;
  const char* help_text = "";
  printf("%s", text);
  do{
    printf("%s", help_text);
    n = scanf("%du", a);
    if (n < 0) return 1;
    if (n == 0){
      scanf("%*[^\n]");
      help_text = "Incorrect type.\n";
    } else {
      scanf("%*c" );
    }
  } while(n == 0);
  return 0;
}

int getString(char **s, const char *text){
  short n;
  int len = 0;
  char buf[81], *tmp_s = (char*)malloc(sizeof(char));
  *tmp_s = '\0';
  printf("%s\n", text);

  do {
    n = scanf("%80[^\n]", buf);
    if (n < 0){
      free(tmp_s);
      tmp_s = NULL;
      return 1;
    }
    if (n == 0){
      scanf("%*c");
    } else {
      len += strlen(buf);
      tmp_s = (char*)realloc(tmp_s, (len+1)*sizeof(char));
      strcat(tmp_s, buf);
    }
  } while (n > 0);
  *s = tmp_s;
  return 0;
}

int getCommand(unsigned short *a, const unsigned short N){
  short n;
  const char* help_text = "";
  do{
    printf("%s", help_text);
    n = scanf("%hu", a);
    if (n < 0) return 1;
    if (n == 0 || (*a < 0 || *a >= N)){
      scanf("%*[^\n]");
      // scanf("%*c");
      help_text = "No such command. Try another one.\n";
    }
  } while(n == 0 || (*a < 0 || *a >= N));
  return 0;
}

keyType getKeyType(){
  unsigned short rc;
  short n;
  const int N = 2;
  const char* help_text = "Enter keyType:\n1. First key\n2. Second key\n";
  do{
    printf("%s", help_text);
    n = scanf("%hu", &rc);
    if (n < 0) return 1;
    if (n == 0 || (rc < 0 || rc > N)){
      scanf("%*[^\n]");
      // scanf("%*c");
      help_text = "No such keyType. Try another one.\n";
    }
  } while(n == 0 || (rc < 1 || rc > N));
  return (rc == 1 ? FIRST:SECOND);
}


















//
