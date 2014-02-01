#ifndef _LIST__H
#define _LIST__H

struct node;

typedef struct node
{
   void* data;
   struct node* next;
} node;

typedef struct list
{
   int size;
   struct node* first;
   struct node* last;
} list;

typedef node* iterator;

list* ListCreate(void);
void ListDestroy(list* l);
int ListAddNode(list* l, void* data);
int ListRemoveNode(list* l, void* data);
node* ListGetPointerFromData(void* data);
void* ListGetData(node* n);

#endif // _LIST__H

