/**
 * @file aux_functions.c
 * @brief Source file for auxiliary functions.
 *
 * This file contains the implementation of auxiliary functions that are used
 * to assist the main functionalities of the Parking Management System. These
 * functions include reading lines of characters into a buffer, and freeing
 * memory allocated for date sum lists.
 *
 * @author Iuri Campos - 51948
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"
/**
 * @brief Reads a line of characters into a buffer.
 *
 * This function reads a line of characters from the standard input into a
 * buffer. It stops reading when it encounters an end-of-file (EOF) character
 * or a newline character ('\n'). The resulting string in the buffer is
 * null-terminated.
 *
 * @param buffer The buffer into which the line of characters is to be read.\
 * The buffer should be large enough to hold the line of characters, plus the
 * null terminator.
 */
void readLine(char buffer[]) {
  int index = 0;
  char c;
  // getchar();  // eliminate the first character
  while ((c = getchar()) != EOF && c != '\n') {
    buffer[index++] = c;
  }
  buffer[index] = '\0';
}

static void addSumNode(Node *recordNode, List *dates) {
  Node *newdatesNode = malloc(sizeof(Node));
  newdatesNode->item.dateSum = malloc(sizeof(DateSum));
  newdatesNode->next = NULL;
  newdatesNode->item.dateSum->charged = 0;
  if (dates->size == 0) {
    dates->head = newdatesNode;
    dates->tail = newdatesNode;
  } else {
    dates->tail->next = newdatesNode;
    dates->tail = newdatesNode;
  }
  newdatesNode->item.dateSum->exitDay = recordNode->item.record->exitDay;
  newdatesNode->item.dateSum->charged += recordNode->item.record->paid;
  dates->size++;
}

static int dateExists(Node *record, Node *date) {
  return record->item.record->exitDay - date->item.dateSum->exitDay == 0;
}

/**
 * @brief Calculates the total amount charged for each exit day for a specific
 * park.
 *
 * This function iterates over a list of records and calculates the total
 * amount charged for each exit day for a specific park. It creates a new list
 * of date sum items, where each item contains an exit day and the total amount
 * charged on that day. If a record for the specified park and exit day already
 * exists in the list, the amount paid in the record is added to the total
 * charged for that day. If no such record exists, a new date sum item is
 * created and added to the list.
 *
 * @param records A pointer to the list of records.
 * @return Returns a pointer to the list of date sum items.
 */

List *calculateSums(List *records) {
  List *dates = ListNew();  // initializes all fields of List
  Node *recordNode = records->head;
  int found = 0;
  for (int i = 0; i < records->size; i++) {
    // Looping through parks records
    if (recordNode->item.record->exitDay != NO_TIME) {
      // Checking if vehicle has left the park
      Node *datesNode = dates->head;
      for (int j = 0; j < dates->size; j++) {
        // Looping through dates records checking if date already exists
        if (dateExists(recordNode, datesNode)) {
          datesNode->item.dateSum->exitDay = recordNode->item.record->exitDay;
          datesNode->item.dateSum->charged += recordNode->item.record->paid;
          found = 1;
          break;
        }
        datesNode = datesNode->next;
      }
      if (found == 0) {
        // No date record found, creating new one
        addSumNode(recordNode, dates);
      }
    }
    found = 0;
    recordNode = recordNode->next;
  }
  return dates;
}
/**
 * @brief Updates the current date and time in the Memory structure.
 *
 * This function updates the current date and time in the Memory structure. The
 * date and time are stored as integers in the format yyyymmdd and hhmm,
 * respectively.
 *
 * @param m A pointer to the Memory structure to be updated.
 * @param date The new date.
 * @param time The new time.
 */
void updateMemoryTime(Memory *m, int date, int time) {
  m->date = date;
  m->time = time;
}
/**
 * @brief Initializes the system.
 *
 * This function initializes the system by creating a new list of parks, a new
 * hash table for cars, and setting the current date and time to 0. It also
 * sets the ID incrementer to 1, which is used to assign unique IDs to new
 * parks.
 *
 * @param m A pointer to the Memory structure to be initialized.
 */
void initializeSystem(Memory *m) {
  m->parks = ListNew();
  HashTable *h = malloc(sizeof(HashTable));
  m->cars = h;
  initializeHashTable(h);
  m->date = 0;
  m->time = 0;
  m->idIncrementer = 1;
}
/**
 * @brief Checks the validity of a license plate.
 *
 * This function checks the validity of a license plate. A valid license plate
 * has the format XX-XX-XX, where each X can be either a digit or an uppercase
 * letter. The license plate must have exactly two sets of digits and one set
 * of letters.
 *
 * @param l The license plate to be checked.
 * @return Returns 1 if the license plate is valid, or 0 if it is not.
 */
int checkPlates(char *l) {
  // Variables to store number of sets found
  int digitSets = 0, letterSets = 0;

  for (int i = 0; i < 9; i += 3) {
    if (isdigit(l[i]) && isdigit(l[i + 1])) {
      digitSets++;
    } else if (isalpha(l[i]) && isalpha(l[i + 1]) && isupper(l[i]) &&
               isupper(l[i + 1])) {
      letterSets++;
    } else {
      return 0;
    }
  }
  if (digitSets >= 1 && letterSets >= 1) {
    return 1;  // License plate is formatted correctly
  }
  return 0;  // License plate is not formatted correctly
}
/**
 * @brief Retrieves the number of days in a given month.
 *
 * This function retrieves the number of days in a given month. It uses an
 * array to store the number of days in each month, and returns the number of
 * days in the specified month. Note that this function does not account for
 * leap years.
 *
 * @param i The month for which the number of days is to be retrieved.
 * January is 1, February is 2, etc.
 * @return Returns the number of days in the specified month.
 */
static int getDaysOfMonth(int i) {
  int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return days[i - 1];
}
/**
 * @brief Calculates the cumulative number of days up to a given month.
 *
 * This function calculates the cumulative number of days up to a given
 * month. It iterates from January to the month before the specified month,
 * and adds the number of days in each month to a total. Note that this
 * function does not account for leap years.
 *
 * @param month The month up to which the number of days is to be calculated.
 * January is 1, February is 2, etc.
 * @return Returns the cumulative number of days up to the specified month.
 */
static int sumDaysAllMonths(int month) {
  int days = 0;  // variable
  for (int i = 1; i < month; i++) {
    days += getDaysOfMonth(i);
  }
  return days;
}
/**
 * @brief Checks the validity of a date.
 *
 * This function checks the validity of a date. A valid date has the format
 * dd-mm-yyyy, where dd is the day, mm is the month, and yyyy is the year.
 * The day must be between 1 and the number of days in the month, the month
 * must be between 1 and 12, and the year must be 2024 or later.
 *
 * @param date The date to be checked.
 * @return Returns 1 if the date is valid, or 0 if it is not.
 */
int checkDateFormat(char *date) {
  int day, month, year;
  sscanf(date, "%d-%d-%d", &day, &month, &year);
  if (day < 1 || day > getDaysOfMonth(month) || month < 1 || month > 12 ||
      year < 0) {
    return 0;
  }
  return 1;
}
/**
 * @brief Checks the validity of a time.
 *
 * This function checks the validity of a time. A valid time has the format
 * hh:mm, where hh is the hour and mm is the minute. The hour must be between
 * 0 and 23, and the minute must be between 0 and 59.
 *
 * @param time The time to be checked.
 * @return Returns 1 if the time is valid, or 0 if it is not.
 */
static int checkTimeFormat(char *time) {
  int hours, minutes;
  sscanf(time, "%d:%d", &hours, &minutes);
  if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
    return 0;
  }
  return 1;
}
/**
 * @brief Checks if a given date and time is in the past.
 *
 * This function checks if a given date and time is in the past relative to
 * the current date and time stored in the Memory structure. If the current
 * date is NO_DATE (which represents an unset date), the function returns 0.
 * If the given date is less than the current date, or if the given date is
 * the same as the current date but the given time is less than or equal to
 * the current time, the function returns 0. Otherwise, it returns 1.
 *
 * @param m A pointer to the Memory structure that contains the current date
 * and time.
 * @param date The date to be checked.
 * @param time The time to be checked.
 * @return Returns 0 if the given date and time is not in the past, or 1 if
 * it is.
 */
static int dateInPast(Memory *m, int date, int time) {
  if (m->date == NO_DATE) {
    return 0;
  } else if (m->date < date) {
    return 0;
  } else if (m->date == date && m->time <= time) {
    return 0;
  }
  return 1;
}
/**
 * @brief Checks the validity of a date and time and whether they are in the
 * past.
 *
 * This function checks the validity of a date and time and whether they are
 * in the past relative to the current date and time stored in the Memory
 * structure. It first checks if the date and time are in the correct format.
 * If they are not, the function returns 1. Then it converts the date and
 * time to integers and checks if they are in the past. If they are in the
 * past, the function returns 1. Otherwise, it returns 0.
 *
 * @param m A pointer to the Memory structure that contains the current date
 * and time.
 * @param date2 The date to be checked.
 * @param time2 The time to be checked.
 * @return Returns 1 if the date and time are not valid or are in the past,
 * or 0 if they are valid and not in the past.
 */
int checkDates(Memory *m, char *date2, char *time2) {
  // Checking if days, months, years are compatible values
  if (checkDateFormat(date2) == 0 || checkTimeFormat(time2) == 0) {
    return 0;
  } else {
    int date = dateToInt(date2);
    int time = timeToInt(time2);
    if (dateInPast(m, date, time)) {
      return 0;
    }
  }
  return 1;
}

/**
 * @brief Calculates the price for a parking record.
 *
 * This function calculates the price for a parking record. The price is
 * calculated based on the number of days and minutes the car was parked. The
 * price is calculated in blocks of 15 minutes. If the exit hour is earlier
 * than the entry hour, it is assumed that the car was parked for an
 * additional day.
 *
 * @param r A pointer to the Record structure for which the price is to be
 * calculated.
 */
void calculatePrice(Record *r) {
  // Temporary placeholder for price of parking
  float price = 0;
  // Temporary placeholders for auxiliary calculations
  int total, days, minutes, blocks;
  // Converting days into minutes and adding to minutes
  total = (r->exitDay - r->entryDay) * 24 * 60 + r->exitHour - r->entryHour;
  // Calculating how many days of parking (24 hours)
  days = total / 1440;
  // Calculating how many minutes passed days
  minutes = total % 1440;
  // Calculating how many blocks of 15 minutes
  blocks = minutes / 15;
  // Ceiling the blocks calculation
  if (minutes % 15 != 0) blocks++;
  // Checking if blocks are more then 4, price changes
  if (blocks > 4) {
    price = 4 * r->park->less_60 + (blocks - 4) * r->park->more_60;
  } else {
    price = blocks * r->park->less_60;
  }
  // Checking if price is higher the daily fee.
  price = price >= r->park->dayly ? r->park->dayly : price;
  // Adding to record the correct price.
  r->paid = days * r->park->dayly + price;
}

/**
 * @brief Converts a date string to an integer.
 *
 * This function converts a date string in the format dd-mm-yyyy to an
 * integer. The integer represents the number of days since 01-01-0000. This
 * is done by multiplying the year by 365, adding the cumulative number of
 * days up to the given month, and adding the day.
 *
 * @param d The date string to be converted.
 * @return Returns the date as an integer.
 */
int dateToInt(char *d) {
  int day, month, year;
  sscanf(d, "%d-%d-%d", &day, &month, &year);
  return year * 365 + sumDaysAllMonths(month) + day;
}
/**
 * @brief Converts a time string to an integer.
 *
 * This function converts a time string in the format hh:mm to an integer.
 * The integer represents the number of minutes since 00:00. This is done by
 * multiplying the hours by 60 and adding the minutes.
 *
 * @param t The time string to be converted.
 * @return Returns the time as an integer.
 */
int timeToInt(char *t) {
  int hours, minutes;
  sscanf(t, "%d:%d", &hours, &minutes);
  return hours * 60 + minutes;
}

/**
 * @brief Converts an integer to a time string.
 *
 * This function converts an integer to a time string in the format hh:mm.
 * The integer represents the number of minutes since 00:00. This is done by
 * dividing the integer by 60 to get the hours and taking the remainder to
 * get the minutes.
 *
 * @param t The integer to be converted.
 * @param time The buffer into which the time string is to be written.
 */
void intToTime(int t, char *time) {
  sprintf(time, "%02d:%02d", t / 60, t % 60);
}

/**
 * @brief Converts an integer to a date string.
 *
 * This function converts an integer to a date string in the format
 * dd-mm-yyyy. The integer represents the number of days since 01-01-0000.
 * This is done by dividing the integer by 365 to get the years, subtracting
 * the number of days in each month from the remainder until it is less than
 * the number of days in a month to get the month, and taking the remaining
 * days as the day.
 *
 * @param d The integer to be converted.
 * @param date The buffer into which the date string is to be written.
 */
void intToDate(int d, char *date) {
  int year = d / 365, month = 0, days;
  d -= year * 365;

  for (int i = 1; i <= 12; i++) {
    days = getDaysOfMonth(i);
    if (d - days <= 0) {
      month++;
      break;
    } else {
      d -= days;
      month++;
    }
  }
  sprintf(date, "%02d-%02d-%4d", d, month, year);
}