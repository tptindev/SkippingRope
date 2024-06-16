#include "List.h"

struct List *CreateList()
{
    struct List* list = (struct List*)malloc(sizeof(struct List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void ListClear(struct List *list)
{
    struct Node* head = list->head;
    while (head != NULL) {
        struct Node* next = head->next;
        free(head);
        head = next;
    }
    list->size = 0;
}

void ListPushBack(struct List *list, void *data)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = list->tail;
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
    }

    if (list->tail != NULL)
    {
        list->tail->next = newNode;
    }

    list->tail = newNode;
    list->size++;
}

void ListPushFront(struct List *list, void *data)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = list->head;

    if (list->head != NULL)
    {
        list->head->prev = newNode;
    }

    list->head = newNode;
    list->size++;
}

void *ListElementAt(struct List *list, size_t index)
{
    struct Node* node = list->head;
    if (index >= list->size)
    {
        printf("Index %llud out of range, auto return last element if index out of range\n", index);
        return list->tail->data;
    }
    else
    {
        size_t begin = 0;
        size_t end = list->size / 2;
        if (index > list->size / 2)
        {
            begin = list->size / 2;
            end = list->size;
        }
        for (; begin < (end && node != NULL); begin++) {
            node = node->next;
        }
        if (node == NULL) return NULL;
    }

    return node->data;
}

void ListTraverse(struct List *list, TaskFnc cb)
{
    struct Node* node = list->head;
    while (node != NULL) {
        struct Node* next = node->next;
        cb(node->data);
        node = next;
    }
}
