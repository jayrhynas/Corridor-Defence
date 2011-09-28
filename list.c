/* 
 * File:    list.c
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Implements doubly-linked lists for the various objects.
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "defenses.h"
#include "enemies.h"
#include "main.h"

/* Creates an empty list and records the type as specified */
List* listCreate (IType type) {
    List* list;
    list = (List *)malloc(sizeof(List));
    list->type = type;
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

/* Adds a node to the end of the list. */
void listAppend (List *list, void *item) {
    Node *node;
    if (list == NULL || item == NULL) {
        return;
    }
    node = (Node*)malloc(sizeof(Node));
    node->item = item;
    if (list->type == ENEMY) {
        Enemy *e;
        e = item;
        e->container = node;
    } else if (list->type == DEFENSE) {
        Defense *d;
        d = item;
        d->container = node;
    } else if (list->type == BULLET) {
        Bullet *b;
        b = item;
        b->container = node;
    } else if (list->type == BUTTON) {
        Button *b;
        b = item;
        b->container = node;
    }
    node->prev = list->last;
    node->next = NULL;
    if (list->last != NULL) {
        list->last->next = node;
    }
    list->last = node;
    if (list->first == NULL) {
        list->first = list->last;
    }
    list->size++;
}

/* Removes the specified node from the given list 
   Does not check to ensure node is in list. BE CAREFUL */
void listRemove (List *list, Node *node) {
    if (list == NULL || node == NULL) {
        return;
    }
    if (node->prev == NULL && node->next == NULL) {
        list->first = NULL;
        list->last = NULL;
    } else if (node->prev == NULL) {
        list->first = node->next;
        list->first->prev = NULL;
    } else if (node->next == NULL) {
        list->last = node->prev;
        list->last->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    list->size--;
    free(node);
}

void listClear (List *list) {
    if (list == NULL) {
        return;
    }
    Node *c, *n;
    c = list->first;
    while(c != NULL) {
        n = c->next;
        free(c->item);
        free(c);
        c = n;
    }
    list->first = NULL;
    list->last = NULL;
}

void listDelete(List *list) {
    if (list == NULL) {
        return;
    }
    listClear(list);
    free(list);
}

void listForEach (List *list, void(*func)(void*)) {
    if (list == NULL) {
        return;
    }
    Node *node;
    node = list->first;
    while(node != NULL) {
        func(node->item);
        node = node->next;
    }
}

void listPrint (List *list) {
    if (list == NULL) {
        return;
    }
    Node *node;
    node = list->first;
    while(node != NULL) {
        if (list->type == ENEMY) {
            fprintf(stderr, " e "); 
        } else {
            fprintf(stderr, " d "); 
        }
        node = node->next;
    }
    fprintf(stderr, "\n"); 
}