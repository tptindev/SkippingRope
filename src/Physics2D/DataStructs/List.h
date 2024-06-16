#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef void (*TaskFnc)(void*);

struct Node
{
    void* data;
    struct Node *next;
    struct Node *prev;
};

struct List
{
    struct Node* head;
    struct Node* tail;
    size_t size;
};

struct List* CreateList();
void ListPushBack(struct List* list, void* data);
void ListPushFront(struct List* list, void* data);
void* ListElementAt(struct List* list, size_t index);
void ListTraverse(struct List* list, TaskFnc cb);
void ListClear(struct List* list);
inline size_t ListTotalSize(struct List* list) { return list->size; }

#endif // LIST_H
