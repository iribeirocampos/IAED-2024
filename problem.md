# Project Statement 1 - IAED 2023/24

## Due date: April 01, 2024, at 19:59

## LOG changes

- 8mar24 - Statement published.
- 14mar24 - Example of invoicing added

## 1. Introduction

The aim is to build a parking management system.
The system should be able to define car parks and record vehicle entries and exits,
as well as consultation and invoicing.

Interaction with the program should take place via a set of
lines made up of a letter (command) and a number of arguments
depending on the command to be executed.
You can assume that all *input* provided will respect the types indicated,
for example, where a decimal integer value is expected, a letter will never be
a letter is expected.
The possible commands are listed in the following table and indicate
the operations to be performed.

| Command | Action |
|:---:|:---|
| __q__ | ends the program |
| __p__ | Creates a parking lot with the billing system or lists existing parking lots |
| __e__ | Records the entry of a vehicle |
| __s__ | Records the exit of a vehicle |
| __v__ | Lists the entries and exits of a vehicle |
| __f__ | Displays parking lot billing |
| __r__ | Remove a parking lot from the system |


## 2. problem specification

The aim of the project is to have a parking management system
up to a maximum of __20__ parking lots.

Each __park__ is characterized by a name, a maximum capacity and
a billing system.
The name describing the car park may contain white characters
(spaces or horizontal tab `\t`).
In this case, the name is enclosed in quotation marks.
If there are no white characters, the name can be delimited by quotation marks
or not.
The name never contains the quotation mark character or the `\n` character in its description.

The amount to be invoiced is defined in 15-minute intervals. Depending on
the duration of the vehicle's stay in the car park, the amount to be invoiced
for each period varies. The billing system for all car parks
is defined by three values:

- *X* : the amount for every 15 minutes in the first hour;
- Y* : the amount for every 15 minutes after the first hour;
- Z* : the maximum daily value (24 hours);

For the first 4 periods of 15 minutes, *X* is charged for each period.
After the first hour, *Y* is charged for each additional 15-minute period.
However, if the period of stay in the park is less than 24 hours,
then the maximum charge cannot exceed *Z*.
Note that in the tariff we always have *Z* > *Y* > *X*.

Vehicles can remain in a car park for more than 24 hours. In this
In this case, the maximum daily charge *Z* is applied to each full 24-hour period
that remains in the car park. The amount to be charged for the remaining period is calculated
as defined for a period of less than 24 hours as described in the
previous paragraph.

Suppose a park whose tariff is 0.25 (15 min. in the 1st hour), 0.30 (15 min. after the 1st hour),
15.00 (maximum per 24 hours).
Consider a vehicle that enters the park on 01-04-2024 08:00 and leaves on
04-04-2024 10:00. In this case, the vehicle has been in the car park for 3 periods of
24 hours and 2 additional hours. Therefore, the amount to be charged is
defined by 3\*15.00+0.25\*4+0.30\*4.
If the vehicle left on 04-04-2024 23:00, then the amount to be charged would be
3\*15.00+15.00 because the maximum amount to be charged in the last period is limited to
a 15.00.

Monetary values are represented as floating-point real numbers
and must be printed to two decimal places (`%.2f`).
Dates are represented in DD-MM-YYYY format and times are
represented in HH:MM format.

The majority shareholders of the company that manages the parking lots
are very superstitious. On February 29th, the car park is always
closed (no entries or exits). As a result, no charge is made for the 24 hours
are not charged for the 24 hours corresponding to that day.

A __vehicle__ is characterized by a license plate. A license plate
is a sequence of 3 pairs of characters separated by the
character _'-'_, one pair of which may only contain capital letters
from _'A'_ to _'Z'_ or can only contain decimal digits.
A pair cannot consist of a letter and a digit.
A license plate must always contain at least one pair of letters and
at least one pair of decimal digits.

Every time a vehicle enters a parking lot, the number of available spaces
available decreases by one. The space is released when the vehicle leaves
the car park.

Vehicle entries and exits are always recorded in chronological order.
In other words, there can never be an entry or exit prior to the last
entry or exit) in the system. In addition, if a vehicle has
in a parking lot, another entry (in the same or another parking lot) cannot be recorded
without the vehicle having left.


## 3. input data
The program should read the input data from the command line of the
terminal.
No command line exceeds __BUFSIZ__ *bytes* (*8192 bytes* on most
systems).

During program execution the instructions must be read from the terminal
(*standard input*) in the form of a set of lines beginning with a
character, which is then called a command, followed by a number of
information depending on the command to be executed; the command and each of the
information are separated by at least one white character.

The available commands are described below. The characters `<` and `>`
are only used in the command description to indicate the parameters.
Optional parameters are indicated by the characters `[` and `]`.
Repeats are indicated by the characters `{` and `}`.
Each command always has all the parameters necessary for its correct
execution.
The characters `...` indicate possible repetitions of a parameter.

Each command indicates a specific action, which is characterized in terms of
input format, output format and errors to be returned.

<u>If the command generates more than one error, only the first one should be indicated.</u>

- __q__ - ends the program:
    - Input format: `q`
    - Output format: NOTHING
- __p__ - Creates a parking lot with the billing system or lists existing parking lots:
    - Input format: `p [ <park-name> <capacity> <value-15> <value-15-apos-1hour> <value-max-day> ]`
    - Output format without arguments: `<park-name> <capacity> <available-seats>`, in the order in which the parks were created.
    - Output format with arguments: NOTHING
    - Errors:
        - `<park-name>: parking already exists.` in case the park name already exists.
        - `<capacity>: invalid capacity.` if the capacity is less than or equal to 0.
        - `invalid cost.` if one of the costs is less than or equal to 0 or if the tariff values are not increasing.
        - `too many parks.` if the number of parks created is at its limit.
- __e__ - Records the entry of a vehicle:
    - Input format: `e <park-name> <registration> <date> <time>`
    - Output format: `<park-name> <available-seats>`.
    - Errors:
        - `<park-name>: no such parking.` in case the park name does not exist.
        - `<park-name>: parking is full.` in case the park is full.
        - `<registration>: invalid license plate.` in case the license plate is invalid.
        - `<registration>: invalid vehicle entry.` in case the car is inside a parking lot.
        - `invalid date.` if the date or time is invalid or before the last entry or exit in the system.
- __s__ - Records the exit of a vehicle:
    - Input format: `s <park-name> <registration> <date> <time>`
    - Output format: `<registration> <entry date> <entry time> <exit date> <exit time> <paid amount>`.
    - Errors:
        - `<park-name>: no such parking.` in case the park name does not exist.
        - `<license plate>: invalid license plate.` if the license plate is invalid.
        - `<registration>: invalid vehicle exit.` if the car is not in the specified parking lot.
        - `invalid date.` if the date or time is invalid or before the last entry or exit in the system.
- __v__ - Lists the entries and exits of a vehicle:
    - Input format: `v <registration>`
    - Exit format: `<park-name> <entry-date> <entry-time> <exit-date> <exit-time>`, sorted first by park name and then by entry date and time. If the vehicle is inside a park, it does not show the exit date and time associated with that entry.
    - Errors:
        - `<registration>: invalid license plate.` in case the registration is invalid.
        - `<registration>: no entries found in any parking.` if the registration is valid, but there are no records of park entries.
- __f__ - Shows the billing for a parking lot:
    - Input format: `f <park-name> [ <date> ]`
    - Output format with one argument: `<date> <billed-value>`, sorted by date. In this option, the parking lot's daily billing summary is shown in the output.
    - Output format with two arguments: `<registration> <exit time> <paid amount>`, sorted by exit date. In this option, the list of amounts billed on a given day is shown in the output.
    - Errors:
        - `<park-name>: no such parking.` in case the park name does not exist.
        - `invalid date.` if the date is invalid or later than the day of the last entry or exit in the system.
- __r__ - Removes a parking lot from the system and all vehicle entries and exits from that lot:
    - Input format: `r <park-name>`
    - Output format: `<park-name>`, sorted by park name.
    - Errors:
        - `<park-name>: no such parking.` in case the park name doesn't exist.

__You can only use the library functions defined in `stdio.h`,
`stdlib.h`, `ctype.h` and `string.h`__

*Important note: you may not use the `goto` statement, the `extern` statement,
or the native C function `qsort` are not allowed and none of these *names* should appear in your code.

## Examples of command usage

### __Command `p`__

The `p` command with no arguments allows you to list all the parks in the system.

```text
p
```

The `p` command with arguments allows you to add a park to the system. In this case, if there are no errors in the input, there is nothing to show in the output.

```text
p Saldanha 200 0.20 0.30 12.00
p “CC Colombo” 400 0.25 0.40 20.00
```

### __Command `e`__

The `e` command allows you to record the entry of a vehicle into a car park. The entry date cannot be earlier than the last entry or exit of another vehicle and the vehicle cannot be registered as being in a parking lot.

```text
e Saldanha AA-00-AA 01-03-2024 08:34
```

### __Command `s`__

The `s` command allows you to record the exit of a vehicle. The vehicle must be registered as being in the specified car park and the exit date must not be earlier than the last entry or exit of another vehicle.

```text
s Saldanha AA-00-AA 01-03-2024 10:59
```

### __Command `v`__

The `v` command allows you to list the use of the car parks by a particular vehicle.

```text
v AA-00-AA
```

### __Command `f`__

The `f` command allows you to list the billing for a parking lot. The amount charged to a vehicle is billed on the date the vehicle leaves, regardless of the date of entry.

The following example shows the parking lot's daily billing summary.

```text
f Saldanha
```

The following example shows all the amounts billed in the parking lot on a given day.

```text
f Saldanha 01-03-2024
```

### __Command `r`__

The `r` command allows you to remove a parking lot from the system. All vehicle entries and exits from the removed car park are also removed from the system.

```text
r “CC Colombo”
```
