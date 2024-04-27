/**
 * @file aux_commands.c
 * @brief Source file for auxiliary command functions.
 *
 * This file contains the implementation of auxiliary functions that are used
 * to assist the main commands of the Parking Management System. These
 * functions include reading and processing input buffers, validating commands,
 * and other utility operations that support the main functionalities of the
 * system.
 *
 * @author Iuri Campos - 51948
 */
#include <stdio.h>
#include <string.h>

#include "project.h"

/**
 * @brief Reads a buffer and extracts a name and a date.
 *
 * This function reads a buffer and extracts a name and a date from it. The
 * name and date are expected to be separated by a space. If the name contains
 * spaces, it should be enclosed in quotes. The date is converted to an
 * integer.
 *
 * @param buffer The buffer to read from.
 * @param name A pointer to a char array where the extracted name will be
 * stored.
 * @param d1 A pointer to an int where the converted date will be stored.
 * @return Returns the number of items successfully read from the buffer. If
 * the name and date are successfully read, returns 2. If only the name is
 * successfully read, returns 1. If nothing is successfully read, returns 0.
 */
int readBuffer(char *buffer, char *name, int *d1) {
  char date[11];  // temporary placeholder for date
  int result;     // var to store number of reads from sscanf
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
 * @brief Validates the exit of a vehicle from a park for command S.
 *
 * This function checks various conditions to determine if a vehicle can exit a
 * park. It checks if the park exists, if the license plate is valid, if the
 * vehicle is in the park, and if the exit date and time are valid and after
 * the entry date and time.
 *
 * @param m Pointer to the Memory structure.
 * @param p1 Pointer to the Park structure.
 * @param r1 Pointer to the Record structure.
 * @param name Name of the park.
 * @param plates License plate of the vehicle.
 * @param date Date of exit.
 * @param time Time of exit.
 * @return Returns 1 if all validations pass and the vehicle can exit,
 * otherwise returns 0 and prints an error message.
 */
int validationsS(Memory *m, Park *p1, Record *r1, char *name, char *plates,
                 char *date, char *time) {
  if (!p1) {
    // checking if park exists
    printf(ERR_PARK_NOT_FOUND(name));
  } else if (checkPlates(plates) == 0) {
    // checking if license is valid
    printf(ERR_INVALID_LICENSE(plates));
  } else if (!r1 || r1->park->id != p1->id || r1->exitDay != NO_DATE) {
    printf(ERR_INVALID_EXIT(plates));
  } else if (!checkDates(m, date, time)) {
    // checking if date is valid and if date is after entry
    printf(ERR_INVALID_DATE);
  } else {
    return 1;
  }
  return 0;
}
/**
 * @brief Validates the creation of a new park for command P.
 *
 * This function checks various conditions to determine if a new park can be
 * created. It checks if the park already exists, if the capacity is valid, if
 * the cost is valid, and if the maximum number of parks has been reached.
 *
 * @param m Pointer to the Memory structure.
 * @param p1 Pointer to the Park structure.
 * @return Returns 1 if all validations pass and the park can be created,
 * otherwise returns 0 and prints an error message.
 */
int validationsP(Memory *m, Park *p1) {
  if (listGetPark(m->parks, p1->name)) {
    // checking if park already exists
    printf(ERR_PARK_ALREADY_EXISTS(p1->name));
  } else if (p1->capacity <= 0) {
    // checking if capacity is valid
    printf(ERR_INVALID_CAPACITY(p1->capacity));
  } else if (p1->less_60 <= 0 || p1->more_60 < p1->less_60 ||
             p1->dayly < p1->more_60) {
    // checking if costs are correct
    printf(ERR_INVALID_COST);
  } else if (m->parks->size == MAX_PARKS) {
    // checking if there are too many parks
    printf(ERR_TOO_MANY_PARKS);
  } else {
    return 1;
  }
  return 0;
}

/**
 * @brief Validates the entry of a vehicle into a park for command E.
 *
 * This function checks various conditions to determine if a vehicle can enter
 * a park. It checks if the park exists, if the park is full, if the license
 * plate is valid, if the vehicle is already in the park, and if the date and
 * time are valid.
 *
 * @param m Pointer to the Memory structure.
 * @param p1 Pointer to the Park structure.
 * @param r1 Pointer to the Record structure.
 * @param name Name of the park.
 * @param plates License plate of the vehicle.
 * @param date Date of entry.
 * @param data Date string.
 * @param time Time of entry.
 * @return Returns 1 if all validations pass, otherwise returns 0 and prints a
 * error message.
 */
int validationsE(Memory *m, Park *p1, Record *r1, char *name, char *plates,
                 char *data, char *time) {
  if (!p1) {
    // checking if park exists
    printf(ERR_PARK_NOT_FOUND(name));
  } else if (p1->occupancy >= p1->capacity) {
    // checking if park is full
    printf(ERR_FULL_PARK(name));
  } else if (!checkPlates(plates)) {
    // checking if license Plate is rightformat
    printf(ERR_INVALID_LICENSE(plates));
  } else if (r1 && r1->exitDay == NO_DATE) {
    // checking if vehicle is another park
    printf(ERR_INVALID_ENTRY(plates));
  } else if (!checkDates(m, data, time)) {
    // checking if date is correct
    printf(ERR_INVALID_DATE);
  } else {
    return 1;
  }
  return 0;
}
