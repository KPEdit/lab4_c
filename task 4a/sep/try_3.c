#include <stdio.h>
#define TABLE_SIZE 100

typedef unsigned int keys;
typedef unsigned int version;
typedef enum {FREE,EXIST,DEL} ItemState;
typedef enum {FIRST,SECOND} keyType;


typedef struct Item {
  keys key1;
  keys key2;
  char *info;
  struct Item **ind1;
  struct Item **ind2;
  struct Item *next;
  ItemState state;
  version ver;
} Item;

typedef union{
  Item *space[2][TABLE_SIZE];
  struct{
    Item* space1[TABLE_SIZE];
    Item* space2[TABLE_SIZE];
  };
} Table;

int nullTable(Table *t){
  for (int i=0;i<TABLE_SIZE;i++){
    for (int j=0;j<2;j++){
      t->space[j][i] = NULL;
    }
  }
  return 0;
}

int main(){
  Table table;
  nullTable(&table);

  return 0;
}
