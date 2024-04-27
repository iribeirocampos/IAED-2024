/**
 * @file printer.c
 * @brief Source file for printing and sorting functions.
 *
 * This file contains the implementation of functions that handle printing and
 * sorting operations for the Parking Management System. These functions
 * include sorting nodes in an array using the insertion sort algorithm and
 * comparing names for sorting purposes. The sorted results are then printed to
 * provide organized output for the system.
 *
 * @author Iuri Campos - 51948
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"

/**
 * @brief Sorts an array of Node pointers using the insertion sort algorithm.
 *
 * This function sorts an array of Node pointers in ascending order using the
 * insertion sort algorithm. The comparison function passed as a parameter is
 * used to determine the order of the nodes.
 *
 * @param array The array of Node pointers to be sorted.
 * @param n The number of elements in the array.
 * @param compar A pointer to a comparison function. This function should take
 * two void pointers (these will be pointers to Nodes), and should return a
 * negative integer if the first node should come before the second, zero if
 * they are equal, and a positive integer if the first node should come after
 * the second.
 */
static void insertionSort(Node** array, int n,
                          int (*compar)(const void*, const void*)) {
  int i, j;
  Node* key;
  for (i = 1; i < n; i++) {
    key = array[i];
    j = i - 1;
    /* Move elements of array[0..i-1], that are
    greater than key, to one position ahead
    of their current position */
    while (j >= 0 && compar(&array[j], &key) > 0) {
      array[j + 1] = array[j];
      j = j - 1;
    }
    array[j + 1] = key;
  }
}

/**
 * @brief Compares the names of the parks in two nodes.
 *
 * This function compares the names of two parks. It is intended to be used as
 * a comparison function `insertionSort`.
 *
 * @param a A pointer to the first node to be compared.
 * @param b A pointer to the second node to be compared.
 * @return Returns a negative integer if the name of the park in the first node
 * comes before the name of the park in the second node in lexicographic order,
 * zero if they are equal, and a positive integer if the name of the park in
 * the first node comes after the name of the park in the second node.
 */
static int compareNames(const void* a, const void* b) {
  Node* nodeA = *(Node**)a;
  Node* nodeB = *(Node**)b;
  return strcmp(nodeA->item.record->park->name,
                nodeB->item.record->park->name);
}
/**
 * @brief Compares the names of two parks.
 *
 * This function compares the names of two parks. It is intended to be used as
 * a comparison function `insertionSort`.
 *
 * @param a A pointer to the first node to be compared.
 * @param b A pointer to the second node to be compared.
 * @return Returns a negative integer if the name of the first park comes
 * before the name of the second park in lexicographic order, zero if they are
 * equal, and a positive integer if the name of the first park comes after the
 * name of the second park.
 */
static int compareParkNames(const void* a, const void* b) {
  Node* nodeA = *(Node**)a;
  Node* nodeB = *(Node**)b;
  return strcmp(nodeA->item.park->name, nodeB->item.park->name);
}

/**
 * @brief Sorts a list of nodes.
 *
 * This function sorts a list of nodes in ascending order using the insertion
 * sort algorithm. The comparison function passed as a parameter is used to
 * determine the order of the nodes.
 *
 * @param l A pointer to the list to be sorted.
 * @param compar A pointer to a comparison function. This function should take
 * two void pointers (these will be pointers to Nodes), and should return a
 * negative integer if the first node should come before the second, zero if
 * they are equal, and a positive integer if the first node should come after
 * the second.
 * @return Returns a pointer to an array of Node pointers sorted in ascending
 * order according to the comparison function. The caller is responsible for
 * freeing this array when it is no longer needed.
 */
static Node** sortList(List* l, int (*compar)(const void*, const void*)) {
  // Allocate an array of pointers to Node
  Node** array = malloc(l->size * sizeof(Node*));

  // Fill the array with pointers to the nodes in the list
  Node* node = l->head;
  for (int i = 0; i < l->size; i++) {
    array[i] = node;
    node = node->next;
  }

  insertionSort(array, l->size, compar);

  return array;
}

/**
 * @brief Prints the records of a vehicle.
 *
 * This function prints the records of a vehicle with a given license plate.
 * The records are sorted by the name of the park, and each record is printed
 * on a separate line. Each line includes the name of the park, the entry date
 * and time, and the exit date and time if the vehicle has exited the park.
 *
 * @param m A pointer to the Memory structure.
 * @param plates The license plate of the vehicle.
 * @return Returns 0 if the records are successfully printed, or 1 if there are
 * no records for the vehicle or an error occurs.
 */
int printRecords(Memory* m, char* plates) {
  // Temporary placeholders for date and time
  char date[11], date2[11], time[6], time2[6];
  List* l = getListPlates(m->cars, plates);
  if (l && l->size > 0) {
    Node** array = sortList(l, compareNames);
    for (int i = 0; i < l->size; i++) {
      Record* r = array[i]->item.record;
      if (!r->exitDay) {
        intToDate(r->entryDay, date);
        intToTime(r->entryHour, time);
        printf("%s %s %s\n", r->park->name, date, time);
      } else {
        intToDate(r->entryDay, date);
        intToTime(r->entryHour, time);
        intToDate(r->exitDay, date2);
        intToTime(r->exitHour, time2);
        printf("%s %s %s %s %s\n", r->park->name, date, time, date2, time2);
      }
    }
    free(array);
    return 1;
  }
  return 0;
}

/**
 * @brief Prints the records of a vehicle.
 *
 * This function prints the records of a vehicle with a given license plate and
 * specific date. The records are sorted by the name of the park, and each
 * record is printed on a separate line. Each line includes the name of the
 * park, the entry date and time, and the exit date and time.
 *
 * @param m A pointer to the Memory structure.
 * @param plates The license plate of the vehicle.
 */
void printBillsDate(List* r, int date) {
  Node* n = r->head;
  for (int i = 0; i < r->size; i++) {
    if (date == n->item.record->exitDay) {
      char time[6];
      intToTime(n->item.record->exitHour, time);
      printf("%s %s %.2f\n", n->item.record->plates, time,
             n->item.record->paid);
    }

    n = n->next;
  }
}
/**
 * @brief Prints the bills for a specific park.
 *
 * This function calculates the sums of the charges for each day for a specific
 * park and prints them. For each day, it prints the date and the total amount
 * charged on that day.
 *
 * @param l A pointer to the list of records.
 */
void printBills(List* l) {
  // temporary placeholder for date
  char date[11];
  List* newList = calculateSums(l);
  Node* n = newList->head;
  for (int i = 0; i < newList->size; i++) {
    intToDate(n->item.dateSum->exitDay, date);
    printf("%s %.2f\n", date, n->item.dateSum->charged);
    n = n->next;
  }
  freeDateSumList(newList);
}
/**
 * @brief Prints the exit record for a vehicle.
 *
 * This function prints the exit record for a vehicle. The record includes the
 * license plate, the entry date and time, the exit date and time, and the
 * amount paid. Each piece of information is printed on a separate line.
 *
 * @param r A pointer to the Record structure that contains the exit record to
 * be printed.
 */
void printSaida(Record* r) {
  // temporary place holders for date and time
  char date[11], date2[11], time[6], time2[6];
  intToDate(r->entryDay, date);
  intToTime(r->entryHour, time);
  intToDate(r->exitDay, date2);
  intToTime(r->exitHour, time2);
  printf("%s %s %s %s %s %.2f\n", r->plates, date, time, date2, time2,
         r->paid);
}
/**
 * @brief Prints the names of all remaining parks.
 *
 * This function sorts the list of parks in ascending order by name and prints
 * the name of each park on a separate line. The parks are sorted using the
 * `compareParkNames` function.
 *
 * @param l A pointer to the list of parks.
 */
void printRemainingParks(List* l) {
  Node** array = sortList(l, compareParkNames);
  for (int i = 0; i < l->size; i++) {
    printf("%s\n", array[i]->item.park->name);
  }
  free(array);
}

/**
 * @brief Prints the names and details of all parks.
 *
 * This function iterates over the list of parks and prints the name and
 * details of each park. The details include the park's capacity, the number of
 * vehicles currently in the park, and the park's hourly rate.
 *
 * @param l A pointer to the list of parks.
 */
void printParks(List* l) {
  Node* n = l->head;
  for (int i = 0; i < l->size; i++) {
    {
      printf("%s %d %d\n", n->item.park->name, n->item.park->capacity,
             n->item.park->capacity - n->item.park->occupancy);
      n = n->next;
    }
  }
}
