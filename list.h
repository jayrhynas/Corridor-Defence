/* 
 * File:    list.h
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Declares doubly-linked lists for the various objects.
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <assert.h>

typedef enum {
    ENEMY,
    DEFENSE,
    BULLET,
    BUTTON
} IType;


typedef struct node {
    IType type;
    void *item;
    struct node *next;
    struct node *prev;
} Node;

typedef struct {
    IType type;
    int size;
    struct node* first;
    struct node* last;
} List;

List* listCreate (IType type);
void listAppend (List *list, void *item);
void listRemove (List *list, Node *node);
void listClear (List *list);
void listDelete (List *list);
void listForEach (List *list, void(*func)(void*));

void listPrint(List *list);

#endif /* _LIST_H_ */