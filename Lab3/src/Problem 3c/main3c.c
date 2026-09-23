/*****************************************************************************
* Programmer: Zach Whitmore
* Class: ECE 251; Friday 8am Lab Section
* Programming Assignment: Lab 3c - GPA Calculator
*
* Date: September 26, 2025
*
* Description:
*   Entry point for Lab 3c. Reads five classes’ grades and credits,
*   calls GPA functions, computes GPA, and prints results.
*
* Relevant Formulas:
*   GPA = (Sum of (GradePoints * Credits)) / (Sum of Credits)
*****************************************************************************/

#include <stdint.h>
#include "ECE251_PIO_util.h"
#include "prob3c.h"

int main(void) {
  UARTsetup(9600);

  // Problem 3a!
  UARTprintf("Hello, Lab 3!\n");
  UARTprintf("Problem c\n\n");

  // Code goes here! Should be variables and function calls.
  char grades[5];
  char credits[5];

  getAllClasses(grades, credits);
  double gpa = computeGPA(grades, credits);
  
  displayGPA(gpa);
  // We are finished. Since this is an embedded system, hang around
  while(1);
}

