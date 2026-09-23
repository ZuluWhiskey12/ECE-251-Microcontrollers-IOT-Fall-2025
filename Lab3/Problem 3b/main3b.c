/*****************************************************************************
* Programmer: Zach Whitmore
* Class: ECE 251; Friday 8am Lab Section
* Programming Assignment: Lab 3b - Daily Calorie Expenditure
*
* Date: September 26, 2025
*
* Description:
*   Entry point for Lab 3b. Collects age, gender, height, weight,
*   and activity level; calculates BMR and daily calorie needs;
*   outputs results over UART.
*
* Relevant Formulas:
*   Women: BMR = 655 + (4.35 * W) + (4.7 * H) - (4.7 * A)
*   Men:   BMR =  66 + (6.23 * W) + (12.7 * H) - (6.8 * A)
*   Daily Calories = BMR * ActivityMultiplier
*****************************************************************************/

#include <stdint.h>
#include "ECE251_PIO_util.h"
#include "prob3b.h"

int main(void) {
  // Initialize the UART.
  UARTsetup(9600);

  // Problem 3a!
  UARTprintf("Hello, Lab 3!\n");
  UARTprintf("Problem b\n\n");

  // Code goes here! Should be variables and function calls.
  int weight = getWeight();
  int heightFeet = getHeightFeet();
  int heightInches = getHeightInches();
  int totalHeight = convertHeightToInches(heightFeet, heightInches);
  int age = getAge();
  int gender = getGender();
  int activityLevel = getActivityLevel();

  double bmr = calculateBMR(weight, totalHeight, age, gender);
  double calories = appliedActivity(bmr, activityLevel);

  displayBMR(bmr);
  displayCalories(calories);

  // We are finished. Since this is an embedded system, hang around
  while(1);
}

