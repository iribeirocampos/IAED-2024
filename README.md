# <p align="center">School Project</p>

## School and Course
<img src="https://epg.ulisboa.pt/sites/ulisboa.pt/files/styles/logos_80px_vert/public/uo/logos/logo_ist.jpg?itok=2NCqbcIP" width="100" height="50">

[Instituto Superior Técnico](https://tecnico.ulisboa.pt/)

[Engenharia Informática e de Computadores](https://tecnico.ulisboa.pt/en/education/courses/undergraduate-programmes/computer-science-and-engineering/)

## Class Subject and Goals
### Class: [IAED](https://fenix.tecnico.ulisboa.pt/cursos/leic-t/disciplina-curricular/1408903891910860) - Introduction to algorithms and data structures
### Goals

- C language 
- Sort Algorithms
- Data structures
    - Hash tables
    - Linked lists
 
### Grade: 20/20 ![Grade](https://img.shields.io/badge/Grade-A%2B-brightgreen)


## Problem Specification

The aim of the project is to have a parking management system
up to a maximum of __20__ parking lots.

Each __park__ is characterized by a name, a maximum capacity and
a billing system.

The amount to be invoiced is defined in 15-minute intervals. Depending on
the duration of the vehicle's stay in the parking lot, the amount to be invoiced
for each period varies. The billing system for all parking lots
is defined by three values:

- *X* : the amount for every 15 minutes in the first hour;
- Y* : the amount for every 15 minutes after the first hour;
- Z* : the maximum daily value (24 hours);

For the first 4 periods of 15 minutes, *X* is charged for each period.
After the first hour, *Y* is charged for each additional 15-minute period.
However, if the period of stay in the park is less than 24 hours,
then the maximum charge cannot exceed *Z*.
Note that in the tariff we always have *Z* > *Y* > *X*.

Vehicles can remain in a parking lot for more than 24 hours. In this
In this case, the maximum daily charge *Z* is applied to each full 24-hour period
that remains in the parking lot. The amount to be charged for the remaining period is calculated
as defined for a period of less than 24 hours as described in the
previous paragraph.

A __vehicle__ is characterized by a license plate. A license plate
is a sequence of 3 pairs of characters separated by the
character _'-'_, one pair of which may only contain capital letters
from _'A'_ to _'Z'_ or can only contain decimal digits.
A pair cannot consist of a letter and a digit.
A license plate must always contain at least one pair of letters and
at least one pair of decimal digits.

Vehicle entries and exits are always recorded in chronological order.
In other words, there can never be an entry or exit prior to the last
entry or exit) in the system. In addition, if a vehicle has
in a parking lot, another entry (in the same or another parking lot) cannot be recorded
without the vehicle having left.

[Full Specification](problem.md)

## Compilation
To compile the program, use the following command:

```bash
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c
```
## Run
Run the program using the following command:

```bash
./proj1
```

## Testing
To run all tests:
1. cd into folder containing all tests
```bash
cd tests
```
2. Run command make:
```bash
make
``` 

<h2>Credits</h2>

- Author: <a href="https://github.com/iribeirocampos" target="_blank">Iuri Campos</a>

<h2>Copyright</h2>
This project is licensed under the terms of the MIT license and protected by IST Honor Code and Community Code of Conduct. 

<img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white">
