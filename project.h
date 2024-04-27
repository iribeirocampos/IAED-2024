/**
 * @file main.h
 * @brief This file contains constants, macros, and error messages used
 * throughout the program.
 *
 * This file defines various constants and macros that are used in the program,
 * such as the hash table size, the maximum number of parks, and the buffer
 * size. It also defines the commands that the program can handle, such as
 * adding a new park or vehicle, exiting a vehicle, and getting park bills. In
 * addition, it defines error messages that are printed when certain conditions
 * are not met, such as when a park is full or not found.
 *
 * @author Iuri Campos - 51948
 */

/*-------\
| MACROS |
\-------*/
#ifndef MAIN_H
#define MAIN_H
#define NO_DATE 0      /*Used to mark if theres no date*/
#define NO_TIME 0      /*Used to mark if theres no date*/
#define HASH_SIZE 8191 /*Hashtable size*/

#define MAX_PARKS 20  /*Maximum number of parques in the program*/
#define BUF_SIZE 8192 /*Default buffer size limit*/

/*---------\
| COMMANDS |
\---------*/

#define CM_ADD_PARK 'p'       /*Command to add a new park*/
#define CM_ADD_VEHICLE 'e'    /*Command to add a new vehicle*/
#define CM_EXIT_VEHICLE 's'   /*Command to exit a vehicle*/
#define CM_EXIT 'q'           /*Command to exit the program*/
#define CM_PRINT_VEHICLES 'v' /*Command to print vehicles*/
#define CM_REMOVE_PARK 'r'    /*Command to remove a park*/
#define CM_GET_BILLS 'f'      /*Command to get parks billing*/

/*---------------\
| ERROR MESSAGES |
\---------------*/

#define ERR_FULL_PARK(p) "%s: parking is full.\n", p
#define ERR_PARK_NOT_FOUND(n) "%s: no such parking.\n", n
#define ERR_INVALID_LICENSE(p) "%s: invalid licence plate.\n", p
#define ERR_INVALID_ENTRY(p) "%s: invalid vehicle entry.\n", p
#define ERR_INVALID_DATE "invalid date.\n"
#define ERR_PARK_ALREADY_EXISTS(n) "%s: parking already exists.\n", n
#define ERR_INVALID_CAPACITY(c) "%d: invalid capacity.\n", c
#define ERR_INVALID_COST "invalid cost.\n"
#define ERR_INVALID_EXIT(p) "%s: invalid vehicle exit.\n", p
#define ERR_TOO_MANY_PARKS "too many parks.\n"
#define ERR_NO_ENTRYS_FOUND(p) "%s: no entries found in any parking.\n", p

typedef unsigned long int Key; /* tipo da chave */
#define key(a) (sumAsciiValues(a->plates))

/*-----------\
| STRUCTURES |
\-----------*/

/**
 * @brief A node in a hash table.
 *
 * This structure represents a node in a hash table. Each node has a unique
 * key, which is the sum of the ASCII values of the license plate of a vehicle.
 * The `records` field is a pointer to a list of records associated with the
 * vehicle.
 */

typedef struct {
  Key key;
  struct list *records;
} HashNode;

/**
 * @brief A hash table for storing HashNode objects.
 *
 * This structure represents a hash table. The hash table is an array of
 * pointers to HashNode objects. The size of the array is determined by the
 * HASH_SIZE macro. Each HashNode in the hash table is identified by a unique
 * key, which is the sum of the ASCII values of the license plate of a vehicle.
 */
typedef struct {
  HashNode *plate[HASH_SIZE];
} HashTable;

/**
 * @brief Structure to hold the exit day and the charged amount.
 *
 * @param exitDay The day asked for sum.
 * @param charged The sum of all payments on that day.
 */
typedef struct {
  int exitDay;
  float charged;
} DateSum;
/**
 * @brief Structure to hold the information of a park.
 *
 * @param name The name of the park.
 * @param capacity The maximum capacity of the park.
 * @param occupancy The current occupancy of the park.
 * @param id The id of the park.
 * @param less_60 The cost of 15 minutes of parking for the first 60 minutes.
 * @param more_60 The cost of 15 minutes of parking after the first 60 minutes.
 * @param dayly The cost of a full day of parking or maximum amount chraged in
 * @param records The list of records of the park
 * one day
 * */
typedef struct {
  char *name;
  int capacity, occupancy, id;
  float less_60, more_60, dayly;
  struct list *records;
} Park;

/**
 * @brief Structure to hold the information of a record.
 *
 * @param plates The license plates of the vehicle.
 * @param park The park where the vehicle is parked.
 * @param entryDay The day the vehicle entered the park.
 * @param entryHour The hour the vehicle entered the park.
 * @param exitDay The day the vehicle exited the park.
 * @param exitHour The hour the vehicle exited the park.
 * @param paid The amount paid by the vehicle.
 *
 */

typedef struct {
  char plates[9];
  Park *park;
  int entryDay, entryHour, exitDay, exitHour;
  float paid;
} Record;

/**
 * @brief Structure to hold the memory of the system, including the parks and
 * records. It stores that state of the system.
 *
 * @param parks The list of parks in the system.
 * @param cars The Hastable to store vehicle records.
 * @param date The current date of the system.
 * @param time The current time of the system.
 * @param idIncrementer The id of the next park to be added.
 */
typedef struct memory {
  struct list *parks;
  HashTable *cars;
  int date, time, idIncrementer;
} Memory;

/**
 * @brief Union to store the different types of items that can be stored in a
 * List
 *
 * @param record A pointer to a Record struct
 * @param park A pointer to a Park struct
 * @param dateSum A pointer to a DateSum struct
 *  *
 */
typedef union {
  Record *record;
  Park *park;
  DateSum *dateSum;
} Item;

/**
 * @brief Structure to hold the information of a node in a list.
 *
 * @param item The item stored in the node.
 * @param next A pointer to the next node in the list.
 *
 */
typedef struct node {
  Item item;
  struct node *next;
} Node;

/**
 * @brief Structure to hold the information of a list.
 *
 * @param head A pointer to the first node in the list.
 * @param tail A pointer to the last node in the list.
 * @param size The size of the list.
 *
 */
typedef struct list {
  Node *head;
  Node *tail;
  int size;
} List;

/*-----------\
| FUNCTIONS  |
\-----------*/

/*aux_commands.c*/
int validationsE(Memory *m, Park *p1, Record *r1, char *name, char *plates,
                 char *data, char *time);
int validationsP(Memory *m, Park *p1);
int validationsS(Memory *m, Park *p1, Record *r1, char *name, char *plates,
                 char *date, char *time);

/*commands.c*/
void processCommand_f(Memory *m, char *buffer);
void processCommand_r(Memory *m, char *buffer);
void processCommand_v(Memory *m, char *buffer);
void processCommand_s(Memory *m, char *buffer);
void processCommand_e(Memory *m, char *buffer);
void processCommand_p(Memory *m, char *buffer);

/*printer.c*/
void printSaida(Record *r);
int printRecords(Memory *m, char *plates);
void printRemainingParks(List *l);
void printParks(List *l);
void printBillsDate(List *r, int date);
void printBills(List *l);

/*data_structures.c*/
List *ListNew();
void listAddPark(List *l, Park *p);
void listAddRecord(List *l, Record *r);
Park *listGetPark(List *l, char *name);
void removePark(Memory *m, int parkId);
void addRecord(HashTable *h, Record *r1);
List *getListPlates(HashTable *h, char *plates);
Record *getLastRecord(HashTable *h, char *plates);

/*aux_functions.c*/
List *calculateSums(List *records);
int timeToInt(char *time);
int dateToInt(char *date);
void intToTime(int t, char *time);
void intToDate(int d, char *date);
int checkDates(Memory *m, char *d2, char *t2);
void readLine(char buffer[]);
void initializeSystem(Memory *m);
int checkPlates(char *l);
void calculatePrice(Record *v);
void updateMemoryTime(Memory *m, int d, int t);
int checkDateFormat(char *date);
void initializeHashTable(HashTable *h);

/*memory_free.c*/
void cleanUp(Memory *m);
void freeDateSumList(List *l);
void freePark(Park *p);
#endif