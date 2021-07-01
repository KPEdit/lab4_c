#ifndef __TABLE__
#define __TABLE__

#define TABLE_SIZE 10
#define M 80
#define STEP 1

typedef unsigned int keys;
typedef unsigned int index;
typedef unsigned int version;
// typedef unsigned int position;
typedef enum {FIRST, SECOND} keyType;
typedef enum {DEL, NEW, EQU} status;

struct Item{
  keys key1;
  keys key2;
  index ind1;
  index ind2;
  char *info;
  version vers;
  version vers2;
  status state;
  struct Item *next;
  // position pos;
};
typedef struct Item Item;

typedef struct{
  Item *space1[TABLE_SIZE];
  Item *space2[TABLE_SIZE];
} Table;

index hashOne(keys , int);
index hashTwo(keys , int);
int findVers(Table *,keys, keyType);
int findVersOne(Table *,keys);
int findVersTwo(Table *,keys);

int printList(Item *);
int getSecondKey(Table *, keys, keys*);
Item *findItem(Table *, keys);
int Find(Table *, Item *, keys, keyType);
int Del(Table *, keys, keyType, version);
int DelAll(Table *, keys, keyType);
int Insert(Table *, Item *);
int PrintItemByIndex(Table *, index);
int PrintTable(Table *);

int DelAllInSpaceOne(Table *, keys);
int DelAllInSpaceTwo(Table *, keys);
int DelInSpaceOne(Table *, keys, version);
int DelInSpaceTwo(Table *, keys, version);
int FindInSpaceOne(Table *, keys, Item *);
int FindInSpaceTwo(Table *, keys, Item *);
int InsertToSpaceOne(Table *, Item *);
int InsertToSpaceTwo(Table *, Item *);

int nullTable(Table *);
int dispose(Table *);

#endif

#ifndef __DIALOG__
#define __DIALOG__

typedef struct {
  const char *msg;
  int (*func)(Table *);
} message;

int dialogExit(Table *);
int dialogFind(Table *);
int dialogDel(Table *);
int dialogInsert(Table *);
int dialogPrintItem(Table *);
int dialogPrintTable(Table *);
int printMenu(const message *, const int);


message dialogList[] = {
  {"0. Exit", dialogExit},
  {"1. Add Element", dialogInsert},
  {"2. Find Element", dialogFind},
  {"3. Del Element", dialogDel},
  {"4. Print Table", dialogPrintTable},
};

#endif

#ifndef __FUNCTONS__
#define __FUNCTONS__

int getCommand(unsigned short *, const unsigned short );
int getString(char **, const char *);
int getKey(keys *, const char *);
int getVer(version *, const char *);
keyType getKeyType();



#endif

























//
