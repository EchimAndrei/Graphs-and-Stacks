/* ECHIM Eugen Andrei - Grupa 311CD */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"
#include "Stack.h"

// Definim structura "List"

typedef struct List {
  ListNode *first;   // Primul element din lista
  ListNode *last;    // Ultimul element din lista
  ListNode *finger;  // "degetul" listei
} List;

// Functia de creare a unui nod din lista

ListNode *createListNode(void) {
  ListNode *node = malloc(sizeof(struct ListNode));  // aici alocam spatiu
  node->elem = '#';                // facem ca vloarea nodului sa fie '#'
  node->next = node->prev = NULL;  // facem legaturile
  return node;
}

// Functia de creare a listei

List *createList(void) {
  List *list = malloc(sizeof(struct List));  // Definim lista
  ListNode *santinela = createListNode();    // Definim santinela
  ListNode *node = createListNode();         // cream primul nod din lista

  // Facem legaturile astfel incat primul nod din lista sa fie santinela si
  // Primul element din litsa sa fie "node"

  santinela->next = node;
  node->prev = santinela;
  node->next = NULL;
  list->first = node;
  list->finger = node;

  return list;
}

// Functia de free a listei

List *destroyList(List *list) {
  if (list == NULL) return NULL;  // daca lista e goala nu face nimic
  ListNode *iter, *temp;

  // Luam o variabila auxiliara care trece prin fiecare nod si ii da free
  iter = list->first->prev;
  while (iter != NULL) {
    temp = iter;
    iter = iter->next;
    free(temp);
  }

  // dam free listei
  free(list);
}

// Functia care muta degetul la stanga cu o pozitie

void MOVE_LEFT(ListNode **finger) {
  // Daca degetul nu este pe prima pozitie acesta ia valoarea nodului precedent
  if ((*finger)->prev->prev != NULL) {
    (*finger) = (*finger)->prev;
  }
}

// Functia care muta degetul la dreapta cu o pozitie

void MOVE_RIGHT(ListNode **finger) {
  // Daca nu exista un nod la dreapta atunci cream un nod si facem legaturile
  // astfetl incat degetul va lua valoarea acelui nod
  if ((*finger)->next == NULL) {
    ListNode *node = createListNode();
    (*finger)->next = node;
    node->prev = (*finger);
    node->next = NULL;
    (*finger) = node;
    return;
  }
  // Daca exista un nod in dreapta, atunci degetul va deveni urmatorul nod
  (*finger) = (*finger)->next;
}

// Functia care insereaza la stanga un caracter

void INSERT_LEFT(char elem, ListNode **finger, FILE **ft) {
  // Daca degetul nu este pe prima pozitie
  if ((*finger)->prev->prev != NULL) {
    ListNode *node = createListNode();
    // Cream un nod si facem legaturile astfel incat acesta
    // sa fie in stangad egetului
    node->prev = (*finger)->prev;
    node->next = (*finger);
    node->elem = elem;  // facem ca valoarea nodului sa fie elem
    (*finger)->prev = node;
    node->prev->next = node;
    (*finger) = node;  // degetul va deveni nodul creat
  } else {
    // Daca degetul se afla pe prima pozitie afisam mesajul "ERROR"
    fprintf(*ft, "ERROR\n");
  }
}

// Cream functia care insereaza un caracter la dreaota

void INSERT_RIGHT(char elem, ListNode **finger) {
  // Daca nu exista un nod in dreapta degetului, cream un nou nod si facem
  // legaturile astfel incat degetul sa devina noul nod creat
  if ((*finger)->next == NULL) {
    ListNode *node = createListNode();  // cream un nou nod
    // facem legaturile cu degetul si atribuim elementul valorii nodului
    node->prev = (*finger);
    node->next = NULL;
    node->elem = elem;

    (*finger)->next = node;
    (*finger) = node;  // mutam pozitia degetului pe nod
  } else {
    // Daca exista un nod in dreapta degetului
    // Cream un nod unde se va introduce noul caracter si
    // realizam legaturile cu celelalte noduri din banda
    ListNode *node = createListNode();
    node->prev = *finger;
    node->next = (*finger)->next;
    node->elem = elem;
    (*finger)->next = node;
    node->next->prev = node;
    (*finger) = node;  // Degetul va lua pozitia noului nod creat
  }
}

// Functia de mutare pana la un caracter in stanga

void MOVE_LEFT_CHAR(char elem, ListNode **finger, FILE **ft) {
  // Declaram un nod auxiliar cu ajutorul caruia vom parcurge banda
  ListNode *tmp = *finger;

  while (tmp->prev->prev != NULL) {
    // Cat timp tmp nu este primul nod din lista verificam daca degetul este
    // acelasi cu auxiliar, in caz afirmativ dam return, daca nu se trece la
    // urmatorul nod cu ajutorul functiei MOVE_LEFT
    if (tmp->elem == elem) {
      (*finger) = tmp;
      return;
    }
    MOVE_LEFT(&tmp);
  }

  // Acum verificam daca primul nod este nodul cautat, daca da atunci dam
  // return, daca nu afisam ERROR
  if (tmp->elem == elem) {
    (*finger) = tmp;
    return;
  } else {
    fprintf(*ft, "ERROR\n");
  }
}

// Functia de mutare pana la un caracter in dreapta

void MOVE_RIGHT_CHAR(char elem, ListNode **finger) {
  // Trecem prin fiecare nod din lista si verificam daca am gasit elem.
  while ((*finger)->next != NULL) {
    if ((*finger)->elem == elem) return;  // Daca l-am gasit oprim cautarea
    MOVE_RIGHT(finger);  // Mutam degetul cu o pozitie la dreapta
  }

  // Verificam daca ultimul element este cumva cel cautat
  if ((*finger)->elem == elem) {
    return;
  }

  // Daca am ajuns aici inseamna ca nu am gasit elementul, asa ca vom crea nodul
  // si ii vom atribui valoarea "elem" si il vom lega si la banda noastra
  ListNode *node = createListNode();
  node->prev = (*finger);
  node->next = NULL;
  node->elem = '#';
  (*finger)->next = node;
  (*finger) = node;
}

// Functia de "WRITE"

void WRITE(char elem, ListNode *finger) {
  // Doar vom atribui valoarea "elem" pe pozitia degetului
  finger->elem = elem;
}

// Functia de afisare a nodului curent

void ShowCurrent(ListNode *finger, FILE **ft) {
  fprintf(*ft, "%c", finger->elem);  // Afisam nodul curent
  fprintf(*ft, "\n");
}

// Functia de afisare a benzii

void Show(List *banda, ListNode *finger, FILE **ft) {
  // Avem nevoie de un nod auxiliar ce va deveni fiecare nod din banda
  ListNode *tmp = banda->first;

  // Cat timp nodul nu este nul
  while (tmp != NULL) {
    // Verificam daca tmp este degetul pentru a vedea cum afisam elementul
    // respectiv
    if (tmp == finger) {
      // Afisam elementul pozitiei degetului
      fprintf(*ft, "|%c|", tmp->elem);
    } else {
      // Afisam celelalte elemente
      fprintf(*ft, "%c", tmp->elem);
    }
    tmp = tmp->next;  // Trecem la pozitia viitoare
  }
  fprintf(*ft, "\n");
}

// Functia de executie a comenzilor

void EXECUTE(Queue *comenzi, ListNode **finger, List *banda, Stack *StivaUndo,
             FILE **ft, Stack *StivaRedo) {
  char comanda[20];  // declaram o variabila in care vom retine comanda
  strcpy(comanda, front(comenzi));  // introducem comanda in variabila comanda
  dequeue(comenzi);                 // eliminam comanda din coada

  if (!strcmp(comanda, "MOVE_LEFT")) {
    // Daca avem comanda de "Move_left"
    if ((*finger)->prev->prev != NULL) {
      // Daca degetul este pe prima pozitie nu vrem sa retinem pozitia aceasta
      // pentru a evita riscul nefunctionarii functiei UNDO
      // Ex: Daca suntem in al doilea nod si dam 3 Comenzi de MOVE_LEFT si dupa
      // UNDO, vrem sa ne intoarcem pe a doua pozitie
      push(StivaUndo, banda->finger);  // introducem in stiva pozitia degetului
    }
    MOVE_LEFT(&banda->finger);  // Ne mutam la stanga
  } else if (!strcmp(comanda, "MOVE_RIGHT")) {
    // Daca avem comanda MOVE_RIGHT introducem in stiva pozitia degetului si
    // apelam functia MOVE_RIGHt
    push(StivaUndo, banda->finger);
    MOVE_RIGHT(&banda->finger);
  } else if (strcmp(comanda, "WRITE") > 0) {
    // Daca avem comanda WRITE, eliberam stivele de UNDO SI REDO
    while (!isStackEmpty(StivaUndo)) pop(StivaUndo);
    while (!isStackEmpty(StivaRedo)) pop(StivaRedo);

    // Apelam functia de WRITE care va primi ca parametrii, elementul si degetul
    WRITE(comanda[strlen(comanda) - 1], banda->finger);
  } else if (strcmp(comanda, "MOVE_RIGHT_CHAR") > 0)
    // Daca avem comanda MOVE_RIGHT_CHAR, apelam functia MOVE_RIGHT_CHAR
    MOVE_RIGHT_CHAR(comanda[strlen(comanda) - 1], &banda->finger);
  else if (strcmp(comanda, "MOVE_LEFT_CHAR") > 0)
    // Daca avem comanda MOVE_LEFT_CHAR, apelam functia MOVE_LEFT_CHAR
    MOVE_LEFT_CHAR(comanda[strlen(comanda) - 1], &banda->finger, ft);
  else if (strcmp(comanda, "INSERT_RIGHT") > 0)
    // Daca avem comanda de INSERT_RIGHT, apelam functia de INSERT_RIGHT
    INSERT_RIGHT(comanda[strlen(comanda) - 1], &banda->finger);
  else if (strcmp(comanda, "INSERT_LEFT") > 0)
    // Daca avem comanda de INSERT_LEFT, apelam functia de INSERT_LEFT
    INSERT_LEFT(comanda[strlen(comanda) - 1], &banda->finger, ft);
}

// Functia de UNDO

void UNDO(ListNode **finger, Stack *StivaUndo, Stack *StivaRedo) {
  push(StivaRedo, *finger);    // Introducem in stivaRedo pozitia degetului
  (*finger) = top(StivaUndo);  // Schimbam pozitia degetului
  pop(StivaUndo);              // Eliminam pozitia degetului
}

// Functia de REDO

void REDO(ListNode **finger, Stack *StivaRedo, Stack *StivaUndo) {
  push(StivaUndo, *finger);    // Introducem in stivaUndo pozitia degetului
  (*finger) = top(StivaRedo);  // Schimbam pozitia degetului
  pop(StivaRedo);              // Eliminam pozitia degetului
}

int main() {
  List *banda = createList();  // cream banda

  Queue *comenzi = createQueue();    // cream coada pentru comenzi
  Stack *StivaUndo = createStack();  // cream stiva pentru UNDO
  Stack *StivaRedo = createStack();  // cream stiva pentru REDO

  // Deschidem fisierele de unde citim si unde vom scrie
  FILE *ft = fopen("tema1.out", "w");  // fisierul de unde citim datele
  FILE *fp = fopen("tema1.in", "r");   // fisierul unde scriem datele

  // Declaram variabilele
  int nrcomenzi = 0;  // Nr de comenzi
  char comanda[50];   // In aceasta variabila vom retine fiecare comanda pe rand
  // scanf("%d", &n);
  fscanf(fp, "%d", &nrcomenzi);  // Citim nr de comenzi

  // Cat timp i este mai mic decat nr de comenzi
  for (int i = 0; i < nrcomenzi; i++) {
    // scanf("%s", str);
    // Citim comanda
    fscanf(fp, "%s", comanda);

    // Daca avem o comanda ce presupune adaugarea/cautarea unui element
    if (strcmp(comanda, "WRITE") == 0 ||
        strcmp(comanda, "MOVE_LEFT_CHAR") == 0 ||
        strcmp(comanda, "MOVE_RIGHT_CHAR") == 0 ||
        strcmp(comanda, "INSERT_LEFT") == 0 ||
        strcmp(comanda, "INSERT_RIGHT") == 0) {
      char c;  // cream un char ce va lua locul elementului
      // scanf(" %c", &c);
      fscanf(fp, " %c", &c);      // citim elementul
      int len = strlen(comanda);  // si il concatenam cu sirul nostru
      comanda[len] = c;
      comanda[len + 1] = '\0';
    }

    // Daca avem o comanda pe care trebuie sa o bagam in stiva
    if (strcmp(comanda, "EXECUTE") != 0 && strcmp(comanda, "SHOW") != 0 &&
        strcmp(comanda, "SHOW_CURRENT") != 0 && strcmp(comanda, "UNDO") != 0 &&
        strcmp(comanda, "REDO") != 0) {
      enqueue(comenzi, comanda);
    } else if (!strcmp(comanda, "EXECUTE"))
      // Daca e comanda de execute
      EXECUTE(comenzi, &banda->finger, banda, StivaUndo, &ft, StivaRedo);
    else if (!strcmp(comanda, "SHOW")) {
      // Daca e comanda de SHOW
      Show(banda, banda->finger, &ft);
    } else if (!strcmp(comanda, "SHOW_CURRENT"))
      // Daca e comanda de SHOW_CURRENT
      ShowCurrent(banda->finger, &ft);
    else if (!strcmp(comanda, "UNDO"))
      // Daca e comanda de UNDO
      UNDO(&banda->finger, StivaUndo, StivaRedo);
    else if (!strcmp(comanda, "REDO"))
      // Daca e comanda de REDO
      REDO(&banda->finger, StivaRedo, StivaUndo);
  }

  // Inchidem fisierele
  fclose(fp);
  fclose(ft);

  // Dealocam memoria
  destroyStack(StivaUndo);
  destroyStack(StivaRedo);
  destroyList(banda);
  destroyQueue(comenzi);

  return 0;
}