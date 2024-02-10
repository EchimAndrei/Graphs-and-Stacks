/* ECHIM Eugen Andrei - Grupa 311CD */

#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>

// ------------- Declaram structurile Stivei si ListNode ----------------

// Structura pentr Nodurile Benzii
typedef struct ListNode {
  char elem;  // declaram elementul
  struct ListNode *next;
  struct ListNode *prev;
} ListNode;

// Structura pentru Nodurile stivei de UNDO si REDO
typedef struct StackNode {
  struct ListNode *poz;  // Aici vom retine pozitia degetului
  struct StackNode *next;
} StackNode;

// Structura pentru stivele de UNDO si REDO
typedef struct Stack {
  StackNode *head;
} Stack;

// Functia de creare a unei stive
Stack *createStack(void) {
  Stack *stack =
      (Stack *)malloc(sizeof(StackNode));  // Alocam memorie pentru stiva
  stack->head = NULL;
  return stack;
}

// Functia de creare a unui nod din stiva
StackNode *createStackNode() {
  StackNode *node = (StackNode *)malloc(
      sizeof(struct StackNode));  // Alocam memorie pentru nod
  node->next = NULL;
  return node;
}

// Functia care verifica daca stiva e goala
int isStackEmpty(Stack *stack) {
  if (stack->head == NULL)  // daca head-ul este NULL atunci stiva e goala
    return 1;               // Stiva e goala
  else
    return 0;  // Stiva nu e goala
}

// Functia care introduce o pozitie in stiva
void push(Stack *stack, ListNode *poz) {
  StackNode *node =
      malloc(sizeof(struct StackNode));  // alocam memorie pentru noul nod
  node->poz = poz;

  // Daca stiva e goala cream un nod
  if (isStackEmpty(stack)) {
    stack->head = node;
    node->next = NULL;
    return;
  }
  // Daca stiva nu e goala atunci mutam head-ul in noul nod creat
  node->next = stack->head;
  stack->head = node;
  return;
}

// Functia care returneaza pozitia de deasupra stivei
ListNode *top(Stack *stack) {
  if (isStackEmpty(stack)) exit(1);  // Daca stiva e goala nu face nimic
  return stack->head->poz;           // returnam pozitia
}

// Functia care scoate din stiva o pozitie
void pop(Stack *stack) {
  struct StackNode *tmp =
      stack->head;  // Declaram un nod care va tine minte head-ul stivei
  stack->head = stack->head->next;  // Mutam head-ul
  free(tmp);                        // Dam free la nodul auxiliar
  return;
}

// Functia de distrugere a stivei
void destroyStack(Stack *stack) {
  // Cat timp nu am ajuns la head-ul stivei
  while (stack->head != NULL) {
    // Luam fiecare nod si ii vom da free
    struct StackNode *node = stack->head;
    stack->head = stack->head->next;  // trecem la urmatorul nod
    free(node);                       // Dam free la nod
  }
  free(stack);  // Dam free la stiva
  return;
}

#endif
