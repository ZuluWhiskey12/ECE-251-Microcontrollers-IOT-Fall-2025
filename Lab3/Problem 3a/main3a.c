/*****************************************************************************
* Programmer: Zach Whitmore
* Class: ECE 251; Friday 8am Lab Section
* Programming Assignment: Lab 3a - BMI Calculator
*
* Date: September 26, 2025
*
* Description:
*   Entry point for Lab 3a. Prompts user for height and weight,
*   calls prob3a functions to calculate BMI, and prints results
*   over UART.
*
* Relevant Formulas:
*   BMI = ((weight in pounds) / (height in inches)^2) * 703
*****************************************************************************/

#include <stdint.h>
#include "ECE251_PIO_util.h"
#include "prob3a.h"

int main(void) {
  // Initialize the UART.
  UARTsetup(9600);

  // Problem 3a!
  UARTprintf("Hello, Lab 3!\n");
  UARTprintf("Problem a\n\n");

  // Code goes here! Should be variables and function calls.
  // getWeight();
  int weight = getWeight();
  int feet = 0;
  int inches = 0;
  getHeight(&feet, &inches);

  displayBmi(calculateBmi(weight, feet, inches));

  // We are finished. Since this is an embedded system, hang around
  while(1);
}

