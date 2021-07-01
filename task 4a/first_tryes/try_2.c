#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "try_2.h"



int main(){
  Table table;
  unsigned short rc;
  // printf("Sizeof table: %d; sizeof item: %d\n", sizeof(table), sizeof(Item*));
  nullTable(&table);
  while(printMenu(menu, menu_size), getCommand(&rc, menu_size)){
    if (menu[rc].func(&table)) break;
  }
  dispose(&table);
  printf("Bye!\n");
  return EXIT_SUCCESS;
}


//Table functions
int dispose(Table *t){

  return 0;
}

unsigned int preHashOne(keys k){
  return (k % M-1);
}

unsigned int hashOne(keys k, int i){
  return ((preHashOne(k) + i) % M);
}

unsigned int hashTwo(keys k){
  return (k % M);
}

int nullTable(Table *t){
  for (int i=0;i<TABLE_SIZE;i++){
    t->space1[i] = NULL;
    t->space2[i] = NULL;
  }
  return 0;
}

int checkKeys(Item *a, Item *b){
  return (a->key1 == b->key1 && a->key2 == b->key2);
}


// sum space
int _insertToSpaceOne(Table *t, Item *a){
  short run = 1;
  unsigned int id;
  int i = -1;
  while (i++,run) {
    id = hashOne(a->key1, i);
    if (t->space1[id] == NULL){
      t->space1[id] = a;
      a->ind1 = &t->space1[id];
      run = !run;
    } else {
      if (checkKeys(t->space1[id], a) && t->space1[id]->state == FREE) a->ver++;
    }
  }
  return 1;
}

// connected space
int _insertToSpaceTwo(Table *t, Item *a){
  unsigned int id = hashTwo(a->key2);
  Item **tmp = &t->space2[id];
  if (*tmp == NULL) {
    *tmp = a;
    a->ind2 = tmp;
    return 1;
  }
  while ((*tmp)->next != NULL) tmp = &((*tmp)->next);
  (*tmp)->next = a;
  a->ind2 = &((*tmp)->next);
  return 1;
}

int insertToTable(Table *t, Item *a){
  if (!_insertToSpaceOne(t, a) || !_insertToSpaceTwo(t,a)) return 0;
  a->state = EXIST;
  return 1;
}


// dialog functions
int generateItem(Item *item){
  if (!getKey(&item->key1, "Enter key1:\n")) return 0;
  if (!getKey(&item->key2, "Enter key2:\n")) return 0;
  if (!getString(&item->info, "Enter item informaiton:\n")) return 0;
  item->ver = 1;
  item->ind1 = NULL;
  item->ind2 = NULL;
  item->next = NULL;
  item->state = FREE;
  return 1;
}

int addElement(Table *t){
  Item *item = (Item*)malloc(sizeof(Item));
  if (!generateItem(item)) return 1;
  if (!insertToTable(t, item)) return 1;
  return 0;
}

int findElement(Table *t){
  printf("%s\n", "FIND");
  return 0;
}

int delElement(Table *t){
  printf("%s\n", "del");

  return 0;
}

int printElement(Table *t){
  printf("%s\n", "print el");

  return 0;
}

int Exit(Table *t){
  return 1;
}

int printMenu(const dialogMsg *menu, const unsigned short N){
  printf("\n=====MENU=====\n");
  for (int i=0; i<N;i++)
    printf("%3d) %s\n", i, menu[i].text);
  printf("==============\n");
  return 0;
}

int getCommand(unsigned short *a, const unsigned short N){
  short n;
  const char* help_text = "";
  do{
    printf("%s", help_text);
    n = scanf("%hu", a);
    if (n < 0) return 0;
    if (n == 0 || (*a < 0 || *a >= N)){
      scanf("%*[^\n]");
      // scanf("%*c");
      help_text = "No such command. Try another one.\n";
    }
  } while(n == 0 || (*a < 0 || *a >= N));
  return 1;
}



//Base functions
int getKey(keys *a, const char *text){
  short n;
  const char* help_text = "";
  printf("%s", text);
  do{
    printf("%s", help_text);
    n = scanf("%du", a);
    if (n < 0) return 0;
    if (n == 0){
      scanf("%*[^\n]");
      help_text = "Incorrect type.\n";
    }
  } while(n == 0);
  return 1;
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
      return 0;
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
  return 1;
}





























//
