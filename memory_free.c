#include <stdio.h>
#include <stdlib.h>

#include "project.h"

/**
 * @brief Frees the memory allocated for a list of nodes.
 *
 * This function frees the memory allocated for a list of nodes. It iterates
 * over the list and frees each node. After all nodes have been freed, it frees
 * the list itself.
 *
 * @param l A pointer to the list to be freed.
 */
static void freeNodes(List *l) {
  Node *n = l->head;
  while (n) {
    Node *next = n->next;
    free(n);
    n = next;
  }
  free(l);
}
/**
 * @brief Frees the memory allocated for a Park structure.
 *
 * This function frees the memory allocated for a Park structure. It frees the
 * memory for the name of the park, the list of records associated with the
 * park, and the Park structure itself.
 *
 * @param p A pointer to the Park structure to be freed.
 */
void freePark(Park *p) {
  if (p->name) free(p->name);
  if (p->records) freeNodes(p->records);
  free(p);
}

/**
 * @brief Frees the memory allocated for a list.
 *
 * This function frees the memory allocated for a list. It iterates over
 * the list and frees each node. After all nodes have been freed, it frees
 * the list itself.
 *
 * @param l A pointer to the list to be freed.
 */
static void listFree(List *l) {
  Node *n = l->head;
  while (n) {
    Node *next = n->next;
    freePark(n->item.park);
    free(n);
    n = next;
  }
  free(l);
}

/**
 * @brief Frees the memory allocated for a list of records.
 *
 * This function frees the memory allocated for a list of records. It iterates
 * over the list and frees each node and the record in the node. After all
 * nodes and records have been freed, it frees the list itself.
 *
 * @param l A pointer to the list to be freed.
 */
static void listRecordsFree(List *l) {
  Node *n = l->head;
  while (n) {
    Node *next = n->next;
    free(n->item.record);
    free(n);
    n = next;
  }
  free(l);
}

/**
 * @brief Frees the memory allocated for a date sum list.
 *
 * This function frees the memory allocated for a date sum list. It iterates
 * over the list and frees each node and the date sum item in the node. After
 * all nodes have been freed, it frees the list itself.
 *
 * @param l A pointer to the list to be freed.
 */
void freeDateSumList(List *l) {
  Node *n = l->head;
  while (n) {
    Node *next = n->next;
    free(n->item.dateSum);
    free(n);
    n = next;
  }
  free(l);
}
/**
 * @brief Frees the memory allocated for a hash table.
 *
 * This function frees the memory allocated for a hash table. It iterates over
 * the hash table and frees each list of records and each hash table entry.
 * After all entries have been freed, it frees the hash table itself.
 *
 * @param h A pointer to the hash table to be freed.
 */
static void freeHastable(HashTable *h) {
  for (int i = 0; i < HASH_SIZE; i++) {
    if (h->plate[i] != NULL) {
      listRecordsFree(h->plate[i]->records);
      free(h->plate[i]);
    }
  }
  free(h);
}

/**
 * @brief Frees the memory allocated for the system.
 *
 * This function frees the memory allocated for the system. It frees the list
 * of parks, the hash table of cars, and the Memory structure itself.
 *
 * @param m A pointer to the Memory structure to be freed.
 */
void cleanUp(Memory *m) {
  listFree(m->parks);
  freeHastable(m->cars);
  free(m);
}