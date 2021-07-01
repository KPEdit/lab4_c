#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "try_1.h"


int main(){
  Table table;
  unsigned short rc;
  const message *menu = &dialogList[0];
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

int printMenu(const message *dl, const int N){
  printf("\n=====MENU=====\n");
  for (int i=0; i<N;i++)
    printf("%3d) %s\n", i, dl[i].msg);
  printf("==============\n");
  return 0;
}

int dialogDel(Table *table){
  return 0;
}

int dialogInsert(Table *table){
  return 0;
}

int dialogPrintTable(Table *table){
  printf("Your table:\n" );
  // PrintTable(table);
  return 0;
}

int dialogFind(Table *table){
  Item *listHead = (Item*)malloc(sizeof(Item));
  keys key;
  keyType kt = getKeyType();
  getKey(&key, "Enter your key:\n");
  if (Find(table, listHead, key, kt)){
    printf("%s\n", "No such Element.");
    return 1;
  }
  printList(listHead);
  disposeList(listHead);
  return 0;
}

index hashOne(keys key, int i){
  // check work of this function
  return ((key+ i)%TABLE_SIZE);
}
index hashTwo(keys key, int i){
  return ((((key) % M)+ i)%TABLE_SIZE);
}

int PrintItemByIndex(Table *table, index ind){
  printf("key 1: %d\n", table->space1[ind]->key1);
  printf("key 2: %d\n", table->space1[ind]->key2);
  printf("info: %s\n", table->space1[ind]->info);
  printf("index 1: %d\n", table->space1[ind]->ind1);
  printf("index 2: %d\n", table->space1[ind]->ind2);
  printf("version 1: %d\n", table->space1[ind]->vers);
  printf("version 2: %d\n", table->space1[ind]->vers2);
  return 0;
}
//
int nullTable(Table *t){
  return 0;
}
int dispose(Table *t){
  return 0;
}
int printList(Item *t){
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

int getVer(version *a,const char *text){
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

















//
