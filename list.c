#include <malloc.h>
#include "list.h"

list* ListCreate(void)
{
   list* head = malloc(sizeof(list));
   head->size = 0;
   head->first = NULL;
   head->last = NULL;
}

int ListAddNode(list* l, void* data)
{
   //fprintf(stderr, "ListAddNode\n");
   // create the node
   node* temp = malloc(sizeof(node));
   if (!temp)
      return 1;
   temp->next = NULL;
   temp->data = data;

   // now find the insertion point
   if (l->first == NULL)
   {
      //fprintf(stderr, "inserting first node\n");
      l->first = temp;
   }
   else
   {
      node* ip = l->first;
      while (ip->next)
         ip = ip->next;
      ip->next = temp;
   }

   l->last = temp;
   l->size++;
   return 0;
}

int ListRemoveNode(list* l, void* data)
{
   // find the node to delete
   node* ip = l->first;

   // is it the first node?
   if (ip->data == data)
   {
      l->first = ip->next;
      free(ip);
      return 0;
   }

   node* prev = ip;
   ip = ip->next;
   while (ip)
   {
      if (ip->data == data)
      {
         prev->next = ip->next;
         free(ip);
         return 0;
      }
      ip = ip->next;
   }
   return 1;
}

void* ListGetData(node* n)
{
   return n->data;
}

