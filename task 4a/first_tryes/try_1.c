#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "try_1.h"


int main(){
  Table table;
  int rc;
  while (rc = getCommand(dialog, msg_len)){
    dialog[rc].func(&table);
  }
  freeTable(&table);
  printf("Good Bye\n");
  return EXIT_SUCCESS;
}


Item *getElement(){
  Item *item = (Item*)malloc(sizeof(Item));
  getKey(&item->key1,"Enter key1:\n");
  getKey(&item->key2,"Enter key2:\n");
  getInfo(&item->info,"Enter Item info:\n");
  item->ind1 = 0;
  item->ind2 = 0;
  item->vers = 0;
  item->state = NEW;
  return item;
}

int addElement(Table *t, Item *a){

  return 0;
}

int findElement(Table *t, keys *k, keyType type){
  return 0;
}

int delElement(Table *t, keys *k, keyType type, vers v){
  return 0;
}

int printTable(Table *t){
  return 0;
}

int hashOne(keys key1){
  return 0 ;

}

int hashTwo(keys key2){
  return 0 ;
}

int freeTable(Table *t){
  
  return 0 ;
}




// dialog section
// int getElement();
// int addElement(Table *t, Item *a);
// int findElement(Table *t, keys *k, keyType type);
// int delElement(Table *t, keys *k, keyType type, vers v);
// int printTable(Table *t);

int dialogAdd(Table *t){
  item *a = getElement();
  addElement(t, a);
  printf("%s\n", "added.");
  return 0;
}
int dialogFind(Table *t){
  printf("FOUNDED\n" );
  return 0;
}
int dialogPrint(Table *t){
  printf("Printed\n" );
  return 0;
}
int dialogDel(Table *t){
  printf("Deleted\n" );
  return 0;
}

int dialogExit(Table *t){
  printf("Exited\n" );
  return 0;
}

int getCommand(base_msg *dia, const int N){
  int rc;
  for (int i=0; i < N; i++){
    printf("%s\n", dia[i].msg);
  }
  do{
    if (!getInt(&rc, "Enter command id:\n")) return 0;

  } while (rc < 0 || rc >= N);
  printf("\n" );

  return rc;
}



// base section
int getInt(int *a, const char* text){
  const char *er = "";;
  int n;
  printf("%s",text);
  do{
    printf("%s", er);
    n = scanf("%d", a);
    if (n < 0){
      return 0;
    }
    if (n == 0){
      er = "Incorrect integer type. Please, try again.\n";
      scanf("%*[^\n]");
      scanf("%*c");
      continue;
    }
  }while (n == 0);
  return 1;
}

int getKey(keys *a, const char* text){
  const char *er = "";;
  int n;
  printf("%s",text);
  do{
    printf("%s", er);
    n = scanf("%d", a);
    if (n < 0){
      return 0;
    }
    if (n == 0){
      er = "Incorrect integer type. Please, try again.\n";
      scanf("%*[^\n]");
      scanf("%*c");
      continue;
    }
  } while (n == 0);
  return 1;
}

int getInfo(char **s, const char *text){
  char *a = (char*)malloc(sizeof(char)), buf[81];
  a = '\0';
  int n, len = 0;

  printf("%s", text);

  do{
    n = scanf("%[^\n]", buf);
    if (n < 0){
      free(a);
      a = NULL;
      return 0;
    }
    if (n == 0){
      scanf("%*c");
    } else {
      len += strlen(buf);
      a = (char*)realloc(a, (len+1)*sizeof(char));
      strcat(a, buf);
    }
  } while (n > 0);
  *s = a;
  return 1;
}




























//
