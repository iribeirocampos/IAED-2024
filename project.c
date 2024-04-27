/* iaed24 - ist151948 - project */
/**
 * @file project.h
 * @brief Header file for the Parking Management System.
 *
 * This file contains the declarations of the data structures and functions
 * used in the Parking Management System. It includes the definitions of the
 * Memory, Park, Record, DateSum, List, Node, Item, and HashTable structures,
 * and the declarations of the functions for initializing the system, reading
 * commands, processing commands, and managing the hash table and list.
 *
 * @author Iuri Campos - 51948
 */
#include "project.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Executes a command.
 *
 * This function executes a command based on the first character of the command
 * string. It uses a switch statement to determine which command to execute.
 * The commands include adding a vehicle, getting bills, adding a park,
 * removing a park, and exiting a vehicle.
 *
 * @param m A pointer to the Memory structure that contains the current state
 * of the system.
 * @param command The first character of the command string.
 * @param buffer The command string.
 */
void execute(Memory *m, char command, char *buffer) {
  switch (command) {  // sorting the command according to first char
    case CM_ADD_VEHICLE:
      processCommand_e(m, buffer);
      break;
    case CM_GET_BILLS:
      processCommand_f(m, buffer);
      break;
    case CM_ADD_PARK:
      processCommand_p(m, buffer);
      break;
    case CM_REMOVE_PARK:
      processCommand_r(m, buffer);
      break;
    case CM_EXIT_VEHICLE:
      processCommand_s(m, buffer);
      break;
    case CM_PRINT_VEHICLES:
      processCommand_v(m, buffer);
      break;
  }
}

/**
 * @brief Main function of the program. Reads commands from stdin and acts
 * accordingly.
 *
 * The function first initializes the system memory. Then, it enters a loop
 * where it reads commands from stdin. Calls the execute function. The loop
 * continues until it reads the exit command or reaches the end of the file.
 *
 * @return Always returns 0.
 */
int main() {
  Memory *m = malloc(sizeof(Memory));
  char *buffer = malloc(sizeof(char) * BUF_SIZE + 1);
  char command;  // stores the first char in stdin that is the command
  initializeSystem(m);
  while ((command = getchar()) != CM_EXIT && command != EOF) {
    readLine(buffer);
    execute(m, command, buffer);
  }
  cleanUp(m);
  free(buffer);
  return 0;
}