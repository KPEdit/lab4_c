#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10
#define STEP 1

typedef unsigned int keys;
typedef unsigned int index;
typedef enum {NEW,OCU,DEL} ItemState;


typedef struct {
  keys key;
  index ind;
  int info;
  ItemState state;
} Item;

typedef struct {
  Item space[TABLE_SIZE];
  int len;
} TableSum;


int printMenu();
int getInt(int *a);
int addElement(TableSum *table1);
int FindElement(TableSum *table1);
int delElement(TableSum *table1);
int printTable(TableSum *table1);
int setToNormal(TableSum *table1);
int printElement(TableSum *table1, index i);
int hashOne(keys a);



int main(){
  TableSum table1;
  int n;
  setToNormal(&table1);

  while (printMenu(),(getInt(&n)) > 0){

    if (n > 4 || n < 1) continue;
    if (n == 1) addElement(&table1);
    if (n == 2) FindElement(&table1);
    if (n == 3) delElement(&table1);
    if (n == 4) printTable(&table1);
  }
  printf("%s\n", "BYE!");
  return 0;
}


int getInt(int *a){
  int n;

  do{
    n = scanf("%d", a);
    if (n < 0) return 0;
    if (n == 0){
      scanf("%*[^\n]" );
    }
  } while(n == 0);
  return 1;
}

int printMenu(){
  printf("%s\n", "1. Add element");
  printf("%s\n", "2. Find Element");
  printf("%s\n", "3. Del Element");
  printf("%s\n", "4. Print Table");
  return 0;
}

int getKey(keys *a){
  int n;
  do{
    n = scanf("%d", a);
    if (n < 0) return 0;
    if (n == 0){
      scanf("%*[^\n]" );
    }
  } while(n == 0);
  return 1;
}

int addElement(TableSum *table1){
  Item item;
  item.state = NEW;
  printf("%s\n", "Item info:");
  getInt(&item.info);
  printf("%s\n", "Item key:");
  getKey(&item.key);

  index h = hashOne(item.key);
  while (h < TABLE_SIZE){
    // printf("%s\n", "ep");
    if (table1->space[h].key == item.key){
      printf("%s\n", "Element with such key has existed!");
      return -1;
    }
    if (table1->space[h++].state == NEW){
      item.state = OCU;
      item.ind = h-1;
      table1->space[item.ind] = item;
      break;
    }
  }

  if ( h + 1 >= TABLE_SIZE)
    printf("%s\n", "Table is full!");
  // printf("number of requests: %d\n", h - hashOne(item.key));
  printf("%s\n", "BUILT\n");
  return h;
}

int FindElement(TableSum *table1){
  short was = 0;
  keys k;
  printf("%s\n", "Enter element key:");
  getKey(&k);
  index i = hashOne(k);

  while (i < TABLE_SIZE){
    if (table1->space[i].key == k && table1->space[i].state == OCU){
      was = !was;
      printElement(table1, i);
      break;
    }
    i++;
  }
  if (!was) printf("%s\n", "No elemets with such key.");
  return 0;
}

int delElement(TableSum *table1){
  short was = 0;
  keys k;
  printf("%s\n", "Enter element key:");
  getKey(&k);
  index i = hashOne(k);

  while (i < TABLE_SIZE){
    if (table1->space[i].key == k){
      if (table1->space[i].state == NEW || table1->space[i].state == DEL) {
        printf("%s\n", "Can't delete no exist elemets");
        return 1;
      }
      was = !was;
      table1->space[i].state = DEL;
      break;
    }
    i++;
  }
  if (!was) printf("%s\n", "No such element.");
  return 0;
}

int printTable(TableSum *table1){
  printf("%s\n", "Printing table items. . .");
  for (int i=0; i<TABLE_SIZE;i++){
    if(table1->space[i].state == OCU)
      printElement(table1, i);
  }

  return 0;
}

int hashOne(keys a){
  return a % 10;
}

int setToNormal(TableSum *table1){
  for (int i=0; i<TABLE_SIZE; i++){
    table1->space[i].state = NEW;
  }
  return 0;
}

int printElement(TableSum *table1, index i){
  printf("%s\n", "\n===YOUR ELEMENT===");
  printf("key: %d\n", table1->space[i].key);
  printf("index: %d\n", table1->space[i].ind);
  printf("info: %d\n", table1->space[i].info);
  const char *s = "New";
  if (table1->space[i].state == OCU) s = "Exist";
  if (table1->space[i].state == DEL) s = "Deleted";
  printf("state: %s\n", s);
  return 0;
}

// keys key;
// index ind;
// int info;
// ItemState state;























//
