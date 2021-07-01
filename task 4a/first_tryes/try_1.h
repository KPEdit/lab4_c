// table section
#ifndef __TABLE__
#define __TABLE__


#define TABLE_SIZE 100

typedef int version;
typedef unsigned int keys;
typedef unsigned int index;
typedef enum {FIRST,SECOND} keyType;
typedef enum {NEW,OCP,DEL} ItemState;


typedef struct {
  keys key1;
  keys key2;
  char *info;
  index ind1;
  index ind2;
  version vers;
  ItemState state;
} Item;

typedef struct{
  Item *elem;
  struct List *next;
} List;

typedef struct {
  int  space1[TABLE_SIZE];
  List space2[TABLE_SIZE];
} Table;

Item* getElement();
int addElement(Table *t, Item *a);
int findElement(Table *t, keys *k, keyType type);
int delElement(Table *t, keys *k, keyType type, version v);
int printTable(Table *t);
int freeTable(Table *);
int hashOne(keys key1);
int hashTwo(keys key2);


#endif

// dialog section
#ifndef __DIALOG__
#define __DIALOG__

typedef struct {
  const char *msg;
  int (*func)(Table *);
} base_msg;

int dialogAdd(Table *);
int dialogFind(Table *);
int dialogPrint(Table *);
int dialogDel(Table *);
int dialogExit(Table *);

const base_msg dExit =   {"0. Exit", dialogExit};
const base_msg dAdd =   {"1. Add Item", dialogAdd};
const base_msg dFind =     {"2. Find Item", dialogFind};
const base_msg dPrint =     {"3. Print Table", dialogPrint};
const base_msg dDel =     {"4. Delete Item", dialogDel};

base_msg dialog[] = {
dExit,
dAdd,
dFind,
dPrint,
dDel
};

const int msg_len = 5;

int getCommand(base_msg *dia, const int N);

#endif

#ifndef __BASE__
#define __BASE__

int getInt(int *a, const char* text);
int getKey(keys *a, const char* text);
int getInfo(char **s,const char *);

























#endif
