#ifndef __TABLE__
#define __TABLE__

#define TABLE_SIZE 10
#define M 51

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

int nullTable(Table *);
int checkKeys(Item *, Item *);
int _insertToSpaceOne(Table *, Item *);
int _insertToSpaceTwo(Table *, Item *);
int insertToTable(Table *, Item *);
int dispose(Table *);

#endif

#ifndef __DIALOG__
#define __DIALOG__

typedef struct {
  const int (*func)(Table *);
  const char *text;
} dialogMsg;

int Exit(Table *);
int findElement(Table *);
int addElement(Table *);
int delElement(Table *);
int printElement(Table *);
int printMenu(const dialogMsg *, const unsigned short);
int getCommand(unsigned short *, const unsigned short);
Item *generateItem();

const unsigned short menu_size = 5;
dialogMsg menu[] = {
  {Exit, "Exit"},
  {findElement, "Find Element"},
  {addElement, "Add Element"},
  {delElement, "Delete Element"},
  {printElement, "Print Element"},
};

#endif

#ifndef __BASE__
#define __BASE__

int getInt(int *, const char *);
int getKey(keys *, const char *);
int getString(char **, const char *);

#endif
