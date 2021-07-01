#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "try_3.h"


int main(){
  Table table;
  nullTable(&table);

  return EXIT_SUCCESS;
}

// DIALOG SECTION



// TABLE SECTION
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
























//BASE FUNCTION
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
