/**
 * @file data_structures.c
 * @brief Source file for data structure management functions.
 *
 * This file contains the implementation of functions that manage the data
 * structures used in the Parking Management System. These functions include
 * initializing the hash table, adding a record to the hash table, and
 * calculating hash values. The hash table is used to store and retrieve
 * records efficiently.
 *
 * @author Iuri Campos - 51948
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"

/**
 * @brief Calculates the sum of the ASCII values of a string.
 *
 * This function calculates the sum of the ASCII values of a string using the
 * djb2 hash function algorithm. The sum is used as a key in the hash table.
 *
 * @param str The string for which to calculate the sum of ASCII values.
 * @return Returns the sum of the ASCII values of the string.
 */
static Key sumAsciiValues(char* str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

/**
 * @brief Calculates the hash value for a given key.
 *
 * This function calculates the hash value for a given key. The hash value is
 * the remainder of the key divided by the size of the hash table. The hash
 * value is used to determine the index at which to store the key in the hash
 * table.
 *
 * @param value The key for which to calculate the hash value.
 * @param m The size of the hash table.
 * @return Returns the hash value for the key.
 */
static int hash(Key value, int m) { return value % m; }

/**
 * @brief Initializes a hash table.
 *
 * This function initializes a hash table by setting all of its entries to
 * NULL. It should be called before the hash table is used for the first time.
 *
 * @param h A pointer to the hash table to be initialized.
 */
void initializeHashTable(HashTable* h) {
  for (int i = 0; i < HASH_SIZE; i++) {
    h->plate[i] = NULL;
  }
}

/**
 * @brief Adds a record to a hash table.
 *
 * This function adds a record to a hash table. The record is added at the
 * index determined by the hash value of the license plate of the vehicle. If
 * there is a collision (i.e., if the calculated index is already occupied),
 * the function uses linear probing to find the next available index. The
 * record is inserted in a list.
 *
 *
 * @param h A pointer to the hash table to which the record is to be added.
 * @param r1 A pointer to the record to be added.
 */
void addRecord(HashTable* h, Record* r1) {
  int i = hash(sumAsciiValues(r1->plates), HASH_SIZE);
  while (h->plate[i] != NULL) {
    if (sumAsciiValues(r1->plates) == h->plate[i]->key) {
      listAddRecord(h->plate[i]->records, r1);
      break;
    }
    i = (i + 1) % HASH_SIZE;
  }
  if (h->plate[i] == NULL) {
    HashNode* newNode = malloc(sizeof(HashNode));
    h->plate[i] = newNode;
    h->plate[i]->key = sumAsciiValues(r1->plates);
    h->plate[i]->records = ListNew();
    listAddRecord(h->plate[i]->records, r1);
  }
}

/**
 * @brief Retrieves the list of records for a vehicle from a hash table.
 *
 * This function retrieves the list of records for a vehicle with a given
 * license plate from a hash table. The function calculates the hash value of
 * the license plate and uses it to find the corresponding record in the hash
 * table. If the record is not found at the calculated index, the function uses
 * linear probing to search the rest of the hash table.
 *
 * @param h A pointer to the hash table from which the record is to be
 * retrieved.
 * @param plates The license plate of the vehicle for which the record is to be
 * retrieved.
 * @return Returns a pointer to the list of records for the vehicle if the
 * record is found, or NULL if the record is not found.
 */
List* getListPlates(HashTable* h, char* plates) {
  int i = hash(sumAsciiValues(plates), HASH_SIZE);
  while (h->plate[i] != NULL) {
    if (sumAsciiValues(plates) == h->plate[i]->key) {
      return h->plate[i]->records;
    }
    i = (i + 1) % HASH_SIZE;
  }
  return NULL;
}
/**
 * @brief Retrieves the last record for a vehicle from a hash table.
 *
 * This function retrieves the last record for a vehicle with a given license
 * plate from a hash table. The function uses the `getListPlates` function to
 * get the list of records for the vehicle, and then returns the last record in
 * the list, in order to check if vehicle is inside any park.
 *
 * @param h A pointer to the hash table from which the record is to be
 * retrieved.
 * @param plates The license plate of the vehicle for which the record is to be
 * retrieved.
 * @return Returns a pointer to the last record for the vehicle if the record
 * is found, or NULL if the record is not found.
 */
Record* getLastRecord(HashTable* h, char* plates) {
  List* l = getListPlates(h, plates);
  if (l) {
    return l->tail->item.record;
  }
  return NULL;
}

/**
 * @brief Creates a new list.
 *
 * This function creates a new list by allocating memory for a List structure
 * and initializing its fields. The head and tail pointers are set to NULL, and
 * the size is set to 0.
 *
 * @return Returns a pointer to the newly created list.
 */
List* ListNew() {
  List* l = malloc(sizeof(List));
  l->head = NULL;
  l->tail = NULL;
  l->size = 0;
  return l;
}
/**
 * @brief Adds a park to a list.
 *
 * This function adds a park to the end of a list. It creates a new node, sets
 * the park field of the node to the given park, and adds the node to the list.
 * If the list is empty, the head and tail pointers are set to the new node.
 * Otherwise, the new node is added after the current tail, and the tail
 * pointer is updated to the new node.
 *
 * @param l A pointer to the list to which the park is to be added.
 * @param p A pointer to the park to be added.
 */
void listAddPark(List* l, Park* p) {
  Node* n = malloc(sizeof(Node));
  n->item.park = p;
  n->next = NULL;
  if (l->size == 0) {
    l->head = n;
    l->tail = n;
  } else {
    l->tail->next = n;
    l->tail = n;
  }
  l->size++;
}
/**
 * @brief Adds a record to a list.
 *
 * This function adds a record to the end of a list. It creates a new node,
 * sets the record field of the node to the given record, and adds the node to
 * the list. If the list is empty, the head and tail pointers are set to the
 * new node. Otherwise, the new node is added after the current tail, and the
 * tail pointer is updated to the new node.
 *
 * @param l A pointer to the list to which the record is to be added.
 * @param r A pointer to the record to be added.
 */
void listAddRecord(List* l, Record* r) {
  Node* n = malloc(sizeof(Node));
  n->item.record = r;
  n->next = NULL;
  if (l->size == 0) {
    l->head = n;
    l->tail = n;
  } else {
    l->tail->next = n;
    l->tail = n;
  }
  l->size++;
}
/**
 * @brief Retrieves a park from a list by name.
 *
 * This function retrieves a park from a list by name. It iterates over the
 * list and returns the first park that matches the given name. If no park with
 * the given name is found, the function returns NULL.
 *
 * @param l A pointer to the list from which the park is to be retrieved.
 * @param name The name of the park to be retrieved.
 * @return Returns a pointer to the park if found, or NULL if not found.
 */
Park* listGetPark(List* l, char* name) {
  Node* n = l->head;
  while (n != NULL) {
    if (strcmp(n->item.park->name, name) == 0) {
      return n->item.park;
    }
    n = n->next;
  }
  return 0;
}

/**
 * @brief Removes a record from a list.
 *
 * This function removes a record from a list. It iterates over the list and
 * removes the first node that contains the given record. If the node to be
 * removed is the head of the list, the head pointer is updated to the next
 * node. If the node to be removed is the tail of the list, the tail pointer is
 * updated to the previous node. If the node is in the middle of the list, the
 * previous node's next pointer and the next node's previous pointer are
 * updated to skip over the removed node.
 *
 * @param l A pointer to the list from which the record is to be removed.
 * @param r A pointer to the record to be removed.
 */
static void listRemoveRecord(List* l, Record* r) {
  Node* prev = NULL;
  Node* n = l->head;
  while (n) {
    Node* next = n->next;
    if (n->item.record == r) {
      if (prev) {
        prev->next = n->next;
      } else {
        l->head = n->next;
      }
      if (n == l->tail) {
        l->tail = prev;
      }
      free(n->item.record);
      free(n);
      l->size--;
      break;
    }
    prev = n;
    n = next;
  }
}

/**
 * @brief Removes all records associated with a specific park from the hash
 * table.
 *
 * This function iterates over the hash table and removes all records
 * associated with a specific park. It does this by iterating over each list of
 * records in the hash table, and removing any records that match the given
 * park ID.
 *
 * @param m A pointer to the Memory structure that contains the hash table.
 * @param parkId The ID of the park for which the records are to be removed.
 */
static void removeParkRecords(Memory* m, int parkId) {
  for (int i = 0; i < HASH_SIZE; i++) {
    if (m->cars->plate[i]) {
      Node* n = m->cars->plate[i]->records->head;
      while (n) {
        Node* next = n->next;
        if (n->item.record->park->id == parkId) {
          listRemoveRecord(m->cars->plate[i]->records, n->item.record);
        }
        n = next;
      }
      if (m->cars->plate[i]->records->size == 0) {
        free(m->cars->plate[i]->records);
        free(m->cars->plate[i]);
        m->cars->plate[i] = NULL;
      }
    }
  }
}
/**
 * @brief Removes a park from the list of parks and all its records from
 * hashtable.
 *
 * This function removes a park with a given ID from the list of parks and the
 * hash table. It first calls the `removeParkRecords` function to remove all
 * records associated with the park from the hash table. Then it iterates over
 * the list of parks and removes the park. If the park to be removed is the
 * head of the list, the head pointer is updated to the next park. If the park
 * to be removed is the tail of the list, the tail pointer is updated to the
 * previous park. If the park is in the middle of the list, the previous park's
 * next pointer and the next park's previous pointer are updated to skip over
 * the removed park.
 *
 * @param m A pointer to the Memory structure that contains the list of parks
 * and the hash table.
 * @param parkId The ID of the park to be removed.
 */
void removePark(Memory* m, int parkId) {
  removeParkRecords(m, parkId);
  Node* prev = NULL;
  Node* n = m->parks->head;
  while (n) {  // removing Park from List
    Node* next = n->next;
    if (n->item.park->id == parkId) {
      if (prev) {
        prev->next = n->next;
      } else {
        m->parks->head = n->next;
      }
      if (n == m->parks->tail) {
        m->parks->tail = prev;
      }
      m->parks->size--;
      break;
    }
    prev = n;
    n = next;
  }
  freePark(n->item.park);
  free(n);
}
