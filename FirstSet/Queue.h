/* ECHIM Eugen Andrei - Grupa 311CD */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

// ------------- Declaram structurile cozii ----------------

// Declaram structura unui nod din coada
typedef struct QueueNode {
  char comanda[20];  // stringul ce va retine comenzile
  struct QueueNode *next;
} QueueNode;

// Declaram structura cozii
typedef struct Queue {
  QueueNode *front;
  QueueNode *rear;
} Queue;

// Facem functia de creare a cozii
Queue *createQueue(void) {
  Queue *queue = malloc(sizeof(Queue));  // Alocam memorie pentru coada
  queue->front = NULL;  // Initializam front-ul si rear-ul cu NULL
  queue->rear = NULL;
  return queue;
}

// Functia care verifica daca coada e goala
int isQueueEmpty(Queue *q) {
  if (q->front == NULL)
    return 1;  // Coada e goala
  else
    return 0;  // Coada nu e goala
}

// Functia care adauga o comanda in coada
void enqueue(Queue *q, char *comanda) {
  QueueNode *node =
      malloc(sizeof(QueueNode));  // alocam memorie pentru o comanda
  strcpy(node->comanda, comanda);

  // Daca avem coada goala atunci front si rear vor deveni noul nod
  if (isQueueEmpty(q)) {
    q->front = q->rear = node;
    node->next = NULL;
    return;
  }

  // Daca nu avem coada goala atunci facem legaturile cu nodul din coada
  q->rear->next = node;
  q->rear = node;
  node->next = NULL;
  return;
}

// Functia care returneaza prima comanda
char *front(Queue *q) {
  if (isQueueEmpty(q)) exit(1);  // Daca avem coada goala nu vom face nimic
  return q->front->comanda;      // returnam prima comanda
}

// Functia care elimina o comanda din coada
void dequeue(Queue *q) {
  // Daca nu avem coada goala
  if (!isQueueEmpty(q)) {
    // Daca avem un singur nod
    if (q->front == q->rear) {
      free(q->front);  // dam free nodului
      q->front = q->rear = NULL;
      return;
    }
    // Daca avem mai multe noduri
    QueueNode *tmp = q->front;  // luam un nod auxiliara
    q->front = q->front->next;  // mutam front-ul
    free(tmp);                  // dam free nodului auxiliar
    return;
  }
}

// Functia care distruge coada
void destroyQueue(Queue *q) {
  // Cat timp front este diferit de NULL
  while (q->front) {
    struct QueueNode *node = q->front;  // alocam un nod auxiliar
    q->front = q->front->next;          // trecem la urmatorul element
    free(node);                         // dam free nodului auxiliar
  }
  free(q);
  return;
}
#endif
