#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"

// TODO - CRIAR ABSTRAÇÃO READBUFFER (passar tuple de ponteiros)

static int readBuffer(char *buffer, char *name, int *d1) {
  char date[11];
  int result;
  if (strchr(buffer, '\"')) {
    result = sscanf(buffer, " \"%[^\"]\" %s", name, date);
  } else {
    result = sscanf(buffer, "%s %s", name, date);
  }
  if (result > 1) {
    *d1 = dateToInt(date);
  }
  return result;
}

/**
 * @brief Processes command 'f', retrieves all billings from a park or billings
 * from a specific date.
 *
 * The function reads the park name and date from
 * the buffer. If only the park name is read, it prints all billings for that
 * park. If both the park name and date are read, it prints the billings for
 * that park on that date. If the park is not found, it prints an error
 * message. If the date is in the past, it prints an error message.
 *
 * @param m Pointer to the Memory structure, which contains the list of parks
 * and records.
 * @param buffer Buffer containing the command arguments.
 * @return void
 */
void processCommand_f(Memory *m, char *buffer) {
  char nome[BUF_SIZE];
  int date, checkValue;
  checkValue = readBuffer(buffer, nome, &date);
  Park *p1 = listGetPark(m->parks, nome);
  if (checkValue == 1) {
    if (!p1) {
      printf(ERR_PARK_NOT_FOUND(nome));
    } else {
      printBills(p1->records);
    }
  } else if (checkValue > 1) {
    if (!p1) {
      printf(ERR_PARK_NOT_FOUND(nome));
    } else if (m->date - date < 0) {
      printf(ERR_INVALID_DATE);
    } else {
      printBillsDate(p1->records, date);
    }
  }
}

/**
 * @brief Processes command 'r', removes a park and all related records.
 *
 * If the buffer is not empty, the function reads the park name from the
 * buffer. If the park is found, it removes the park and all related records,
 * and then prints the remaining parks. If the park is not found, it prints
 * an error message.
 *
 * @param m Pointer to the Memory structure, which contains the list of parks
 * and records.
 * @param buffer Buffer containing the command arguments.
 * @return void
 */
void processCommand_r(Memory *m, char *buffer) {
  char nome[BUF_SIZE];
  if (strchr(buffer, '\"')) {
    sscanf(buffer, " \"%[^\"]\"", nome);
  } else {
    sscanf(buffer, "%s", nome);
  }
  Park *p1 = listGetPark(m->parks, nome);
  if (!p1) {
    // checking if park exists
    printf(ERR_PARK_NOT_FOUND(nome));
  } else {
    removePark(m, p1->id);
    printRemainingParks(m->parks);
  }
}

/**
 * @brief Processes command 'v', checks if input license plates are valid and
 * lists all records with that license plate.
 *
 * If the buffer is not empty, the function reads the license plates from the
 * buffer. If the license plates are not valid, it prints an error message.
 * If the license plates are valid, it prints all records with that license
 * plate. If no records are found, it prints an error message.
 *
 * @param m Pointer to the Memory structure, which contains the list of
 * records.
 * @param buffer Buffer containing the command arguments.
 * @return void
 */
void processCommand_v(Memory *m, char *buffer) {
  char plates[9];
  sscanf(buffer, "%s", plates);
  if (!checkPlates(plates)) {
    printf(ERR_INVALID_LICENSE(plates));
  } else if (!printRecords(m, plates)) {
    printf(ERR_NO_ENTRYS_FOUND(plates));
  }
}

/**
 * @brief Processes command 's', checks if inputs are valid and removes the
 * vehicle from the park.
 *
 * The function reads the park name, license
 * plates, date, and time from the buffer. If the park is found, it removes
 * the vehicle with the given license plates from the park. If the park is
 * not found, it prints an error message. If the license plates are not
 * valid, it prints an error message.
 *
 * @param m Pointer to the Memory structure, which contains the list of parks
 * and records.
 * @param buffer Buffer containing the command arguments.
 * @return void
 */
void processCommand_s(Memory *m, char *buffer) {
  char nome[BUF_SIZE], plates[9], data[11], hora[6];
  int time, date;
  if (strchr(buffer, '\"')) {
    sscanf(buffer, " \"%[^\"]\" %s %s %s", nome, plates, data, hora);
  } else {
    sscanf(buffer, "%s %s %s %s", nome, plates, data, hora);
  }
  Park *p1 = listGetPark(m->parks, nome);
  Record *r1 = getLastRecord(m->cars, plates);
  date = dateToInt(data);
  time = timeToInt(hora);
  if (validationsS(m, p1, r1, nome, plates, data, hora)) {
    // everything is valid, adding vehicle to the list
    r1->exitDay = date;
    r1->exitHour = time;
    r1->park->occupancy--;
    updateMemoryTime(m, date, time);
    calculatePrice(r1);
    printSaida(r1);
    listAddRecord(r1->park->records, r1);
  }
}

/**
 * @brief Processes command 'e', checks if inputs are valid and inserts the
 * vehicle in the park.
 *
 * The function reads the park name, license
 * plates, date, and time from the buffer. If the park is found, it inserts
 * the vehicle with the given license plates into the park. If the park is
 * not found, it prints an error message. If the license plates are not
 * valid, it prints an error message. If the date is not valid, it prints an
 * error message.
 *
 * @param m Pointer to the Memory structure, which contains the list of parks
 * and records.
 * @param buffer Buffer containing the command arguments.
 * @return void
 */
void processCommand_e(Memory *m, char *buffer) {
  char nome[BUF_SIZE], plates[20], data[11], hora[6];
  int time, date;
  if (strchr(buffer, '\"')) {
    sscanf(buffer, " \"%[^\"]\" %s %s %s", nome, plates, data, hora);
  } else {
    sscanf(buffer, "%s %s %s %s", nome, plates, data, hora);
  }
  Park *p1 = listGetPark(m->parks, nome);
  Record *r1 = getLastRecord(m->cars, plates);
  date = dateToInt(data);
  time = timeToInt(hora);
  if (validationsE(m, p1, r1, nome, plates, data, hora)) {
    Record *newR = malloc(sizeof(Record));
    newR->park = p1;
    newR->entryDay = date;
    newR->entryHour = time;
    newR->exitDay = NO_DATE;
    newR->exitHour = NO_TIME;
    newR->park->occupancy++;
    updateMemoryTime(m, date, time);
    strcpy(newR->plates, plates);
    addRecord(m->cars, newR);
    printf("%s %d\n", nome, p1->capacity - p1->occupancy);
  }
}

/**
 * @brief Processes command 'p', creates a new park and adds it to the queue
 * if arguments are provided. If no arguments, prints the existing parks.
 *
 * If the buffer is not empty, the function reads the park name, capacity,
 * and costs from the buffer. If the park name is found, it creates a new
 * park and adds it to the queue. If the park name is not found, it prints an
 * error message. If the capacity is not valid, it prints an error message.
 * If the costs are not valid, it prints an error message.
 *
 * @param m Pointer to the Memory structure, which contains the list of
 * parks.
 * @param buffer Buffer containing the command arguments.
 * @return void
 */
void processCommand_p(Memory *m, char *buffer) {
  if (buffer[0]) {  // checking if buffer is empty
    Park *p1 = malloc(sizeof(Park));
    char *name = malloc(sizeof(char) * BUF_SIZE + 1);
    p1->name = name;
    p1->records = NULL;
    if (strchr(buffer, '\"')) {
      sscanf(buffer, " \"%[^\"]\" %d %f %f %f", p1->name, &p1->capacity,
             &p1->less_60, &p1->more_60, &p1->dayly);
    } else {
      sscanf(buffer, "%s %d %f %f %f", p1->name, &p1->capacity, &p1->less_60,
             &p1->more_60, &p1->dayly);
    }
    if (validationsP(m, p1)) {
      // everything is valid, adding park to the list
      p1->occupancy = 0;
      p1->id = m->idIncrementer++;
      p1->records = ListNew();
      listAddPark(m->parks, p1);
    }
  } else {
    printParks(m->parks);
  }
}
