#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define INFINIT 999999999
#define CUVANT 100
#define DIMENSIUNECOST 100

typedef int TCost;

typedef struct node {
  int v;
  TCost c;
  struct node *next;
} TNode, *ATNode;

typedef struct {
  int nn;
  ATNode *adl;
} TGraphL;

// ALOCAM SPATIU PENTRU LISTA
void alloc_list(TGraphL *G, int n) {
  int i;
  // Initializam G->nn cu nr de Obiective
  G->nn = n;
  // Alocam spatiu pentru lista de adiacenta si o initializam cu NULL
  G->adl = (ATNode *)malloc((n + 1) * sizeof(ATNode));
  for (i = 0; i < n; i++) G->adl[i] = NULL;
}

// ELIBERAM SPATIUL PENTRU LISTA
void free_list(TGraphL *G) {
  int i;
  // Luam un nod temporar
  ATNode it;
  for (i = 0; i < G->nn; i++) {
    // it va fi nodul curent
    it = G->adl[i];
    while (it != NULL) {
      // Luam un nod auxiliar si ii dam free
      ATNode aux = it;
      it = it->next;
      free(aux);
    }
    G->adl[i] = NULL;
  }
  // Dam free la lista de adiacenta
  free(G->adl);
  // Dam free la graph
  free(G);
}

// ADAUGAM UN ELEMENT IN LISTA
void insert_edge_list(TGraphL *G, int v1, int v2, int c) {
  // Cream un nod auxiliar si ii initializam componentele
  TNode *t;
  t = (ATNode)malloc(sizeof(TNode));
  t->v = v2;
  t->c = c;
  t->next = G->adl[v1];
  G->adl[v1] = t;
  // Facem acelasi lucru pentru urmatorul nod
  t = (ATNode)malloc(sizeof(TNode));
  t->v = v1;
  t->c = c;
  t->next = G->adl[v2];
  G->adl[v2] = t;
}

void destroyHeap(MinHeap *heap) {
  for (int i = 0; i < heap->size; i++) {
    free(heap->elem[i]);
  }
  free(heap->elem);
  free(heap->pos);
  free(heap);
}

void Prim(TGraphL G, int START, int **vizitati, int *rezultatcost) {
  // Declaram vectorul de distante si cel cu parinti
  int dist[G.nn];
  int parinti[G.nn];

  // Cream un heap si ii initializam size-ul
  MinHeap *heap = newQueue(G.nn);
  heap->size = G.nn;

  for (int i = 0; i < G.nn; i++) {
    // Initializam fiecare destinatie ca infinit
    dist[i] = INFINIT;

    // Cream pentru fiecare element un nod in heap
    heap->elem[i] = newNode(i, dist[i]);
    heap->pos[i] = i;

    // Setam parintele cu -1
    parinti[i] = -1;
  }

  // initializam distanta pentru pozitia start cu 0
  dist[START] = 0;
  heap->pos[START] = START;

  // Actualizam heap-ul
  SiftUp(heap, START, dist[START]);

  // Suma = suma costurilor
  int suma = 0;

  while (!isEmpty(heap)) {
    // Luam un MinHeapNode care va fi minimul din heap
    MinHeapNode *nodeheap = removeMin(heap);

    // Ii atribuim lui u valoarea nodului (indexul)
    int u = nodeheap->v;

    // Daca are costul infinit atunci dam free nodului si incetam while-ul
    if (nodeheap->d == INFINIT) {
      free(nodeheap);
      break;
    }

    // Adunam la suma dist[u]
    suma += dist[u];

    // Marcam nodul u ca fiind vizitat
    (*vizitati)[u] = 1;

    // Pentru fiecare nod din adl[u]
    TNode *nod = G.adl[u];
    while (nod != NULL) {
      // Verificam daca e in minheap si daca dist[index] > costul
      if (isInMinHeap(heap, nod->v) && dist[nod->v] > nod->c) {
        // Daca da actualizam datele si heap-ul
        dist[nod->v] = nod->c;
        parinti[nod->v] = u;
        SiftUp(heap, nod->v, dist[nod->v]);
      }
      // Trecem la urmatorul nod
      nod = nod->next;
    }
    // Dam free la nodeheap
    free(nodeheap);
  }

  // rezultatcost o sa fie suma
  *rezultatcost = suma;

  // distrugem heap-ul
  destroyHeap(heap);
}

// Functia de comparare pentru qsort
int cmp(const void *v1, const void *v2) { return (*(int *)v1 - *(int *)v2); }

int main() {
  // Deschidem fisierul
  FILE *file = fopen("tema3.in", "r");

  // Citim Nr de obiective si de drumuri
  int nrObiective, nrDrumuri;
  int cateobiective = 0;
  fscanf(file, "%d %d", &nrObiective, &nrDrumuri);

  TGraphL *graph = malloc(sizeof(TGraphL));
  alloc_list(graph, nrObiective);

  // Cream vectorul de obiective
  char **numeObiective = malloc(nrObiective * sizeof(char *));
  for (int i = 0; i < nrDrumuri; i++) {
    // Alocam spatiu pentru fiecare cuvant si citim obiectivele si costul
    char *obiectiv = malloc(CUVANT * sizeof(char));
    char *obiectiv2 = malloc(CUVANT * sizeof(char));
    int cost;
    fscanf(file, "%s %s %d", obiectiv, obiectiv2, &cost);

    // Verificam daca am introdus deja obiectivul in vector
    int gasitObiectiv = 0;
    int j = 0;
    for (j = 0; j < cateobiective; j++) {
      if (!strcmp(obiectiv, numeObiective[j])) {
        gasitObiectiv = 1;  // Inseamna ca am gasit obiectivul
        break;
      }
    }
    // Daca nu l-am gasit atunci il introducem acum
    if (!gasitObiectiv) {
      numeObiective[cateobiective] = malloc(CUVANT * sizeof(char));
      strcpy(numeObiective[cateobiective], obiectiv);
      cateobiective++;
    }

    // Verificam pentru al doilea obiectiv
    gasitObiectiv = 0;
    int k = 0;
    for (k = 0; k < cateobiective; k++) {
      if (!strcmp(obiectiv2, numeObiective[k])) {
        gasitObiectiv = 1;  // Inseamna ca am gasit obiectivul
        break;
      }
    }
    // Daca nu l-am gasit atunci il introducem acum
    if (!gasitObiectiv) {
      numeObiective[cateobiective] = malloc(CUVANT * sizeof(char));
      strcpy(numeObiective[cateobiective], obiectiv2);
      cateobiective++;
    }
    // Inseram in lista nodul
    insert_edge_list(graph, j, k, cost);

    // Dam free la obiective
    free(obiectiv);
    free(obiectiv2);
  }
  fclose(file);

  // Facem un vector de vizitati sa vedem ce nod vizitam
  int *viziati = calloc(graph->nn, sizeof(int));
  // Acesta este un contor ce o sa numere insulele
  int insule = 0;
  // Vectorul cu costuri
  int *vectorcost = malloc(sizeof(int) * DIMENSIUNECOST);

  for (int i = 0; i < graph->nn; i++) {
    // Resetam costul pentru fiecare subgraph
    int rezultatcost = 0;
    // Daca nodul nu a fost vizitat, apelam prim pentru el
    if (viziati[i] != 1) {
      Prim(*graph, i, &viziati, &rezultatcost);
      // Il marcam viziatat si crestem nr de insule
      viziati[i] = 1;
      // Introducem costul in vector
      vectorcost[insule] = rezultatcost;
      // Crestem nr de insule
      insule++;
    }
  }

  // Sortam crescator vectorul de costuri
  qsort(vectorcost, insule, sizeof(int), cmp);

  // Deschidem fisierul de out
  file = fopen("tema3.out", "w");

  // Afisam nr de insule
  fprintf(file, "%d\n", insule);

  for (int i = 0; i < insule; i++) {
    fprintf(file, "%d\n", vectorcost[i]);
  }

  // Dam free la vectorul de viziati
  free(vectorcost);
  free(viziati);

  // Free la numeObiective
  for (int i = 0; i < nrObiective; i++) {
    free(numeObiective[i]);
  }
  free(numeObiective);

  // free la graph
  free_list(graph);

  // Inchidem fisierul
  fclose(file);
  return 0;
}