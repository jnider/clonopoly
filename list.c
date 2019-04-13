#include <malloc.h>
#include "list.h"

//#define LIST_DEBUG

#ifdef LIST_DEBUG
static void dump(list* l)
{
   // first, remove any existing nodes
   if (l->first != NULL)
   {
      node* ip = l->first;
      int count = 0;
      fprintf(stderr, "list %p: %i\n", l, l->size);
      while (ip)
      {
         fprintf(stderr, "%i: %p\n",count, ip);
         count++;
         ip = ip->next;
      }
   }
}
#endif // LIST_DEBUG

list* ListCreate(void)
{
   list* head = (list*)malloc(sizeof(list));
   if (!head)
      return NULL;

   head->size = 0;
   head->first = NULL;
   head->last = NULL;
   return head;
}

void ListDestroy(list* l)
{
#ifdef LIST_DEBUG
   fprintf(stderr, "ListDestroy\n");
#endif // LIST_DEBUG
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
   node* temp = (node*)malloc(sizeof(node));
   if (!temp)
      return 1;

   temp->next = NULL;
   temp->data = data;

   // now find the end of the list
   if (l->first == NULL)
   {
      l->first = temp;
#ifdef LIST_DEBUG
      fprintf(stderr, "new first: %p\n", l->first);
#endif // LIST_DEBUG
   }
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
   int count = 0;
#ifdef LIST_DEBUG
   fprintf(stderr, "ListRemoveNode l=%p size=%i\n", l, l->size);
#endif // LIST_DEBUG
   // find the node to delete
   node* ip = l->first;

   // is the list empty?
   if (l->size == 0)
   {
#ifdef LIST_DEBUG
      fprintf(stderr, "empty list!\n");
#endif // LIST_DEBUG
      return 1;
   }
#ifdef LIST_DEBUG
   dump(l);
#endif // LIST_DEBUG

   // is it the first node?
   if (ip->data == data)
   {
#ifdef LIST_DEBUG
      fprintf(stderr, "first node: %p\n", ip);
#endif // LIST_DEBUG
      l->first = ip->next;
#ifdef LIST_DEBUG
      fprintf(stderr, "new first: %p\n", l->first);
      dump(l);
#endif // LIST_DEBUG
      free(ip);
      l->size--;
      return 0;
   }

   node* prev = ip;
   ip = ip->next;
   count++;
   while (ip)
   {
      count++;
      if (ip->data == data)
      {
#ifdef LIST_DEBUG
         fprintf(stderr, "other node: %p\n", ip);
#endif // LIST_DEBUG
         prev->next = ip->next;
         l->size--;
         free(ip);
#ifdef LIST_DEBUG
         dump(l);
         fprintf(stderr, "node %i deleted\n", count-1);
#endif // LIST_DEBUG
         return 0;
      }
      prev = ip;
      ip = ip->next;
   }
   return 1;
}

void* ListGetData(node* n)
{
#ifdef LIST_DEBUG
   fprintf(stderr, "ListGetData %p\n", n);
#endif // LIST_DEBUG
   return n->data;
}

void* ListGetFirst(list* l, iterator* i)
{
   if (!l)
      return 0;

   i = (iterator*)malloc(sizeof(iterator));
   *i = l->first;

   return ListGetData(*i);
}

void* ListGetNext(iterator* i)
{
   *i = (*i)->next;
   if (*i)
      return ListGetData(*i);
   return 0;
}

