#include <malloc.h>
#include "list.h"

list* ListCreate(void)
{
   list* head = malloc(sizeof(list));
   if (!head)
      return NULL;

   head->size = 0;
   head->first = NULL;
   head->last = NULL;
   return head;
}

void ListDestroy(list* l)
{
   // first, remove any existing nodes
   if (l->first != NULL)
   {
      node* temp;
      node* ip = l->first;
      while (ip)
      {
         temp = ip;
         ip = ip->next;
         free(temp);
      }
   }

   free(l);
}

int ListAddNode(list* l, void* data)
{
   // create the node
   node* temp = malloc(sizeof(node));
   if (!temp)
      return 1;

   temp->next = NULL;
   temp->data = data;

   // now find the end of the list
   if (l->first == NULL)
      l->first = temp;
   else
   {
      node* ip = l->first;
      while (ip->next)
      {
         ip = ip->next;
      }
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
      l->size--;
      return 0;
   }

   node* prev = ip;
   ip = ip->next;
   while (ip)
   {
      if (ip->data == data)
      {
         prev->next = ip->next;
         l->size--;
         free(ip);
         return 0;
      }
      ip = ip->next;
   }
   return 1;
}

void* ListGetData(node* n)
{
   //fprintf(stderr, "ListGetData %i\n", n);
   return n->data;
}

