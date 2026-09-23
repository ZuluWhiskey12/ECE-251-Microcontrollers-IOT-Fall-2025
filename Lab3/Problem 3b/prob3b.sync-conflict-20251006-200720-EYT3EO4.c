/*****************************************************************************
* Programmer: Zach Whitmore
* Class: ECE 251; Friday 8am Lab Section
* Programming Assignment: Lab 3b - Daily Calorie Expenditure
*
* Date: September 26, 2025
*
* Description:
*   Functions for Lab 3b. Handles validated input (weight, height,
*   age, gender, activity), computes BMR and daily calories,
*   and displays values over UART. Uses strict error checking (Lab 3d).
*
* Relevant Formulas:
*   Women: BMR = 655 + (4.35 * W) + (4.7 * H) - (4.7 * A)
*   Men:   BMR =  66 + (6.23 * W) + (12.7 * H) - (6.8 * A)
*   Daily Calories = BMR * ActivityMultiplier
*****************************************************************************/

#include "prob3b.h"
/************************************************************
* Function: isDigitChar ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Checks if input char is a digit (0–9).
* Input parameters: ch (char)
* Returns: int (1 if digit, 0 if not)
* Preconditions: None
* Postconditions: Returns 1 or 0
************************************************************/
int isDigitChar(char ch) {
	int inputTrue = (ch >= '0' && ch <= '9');
    return inputTrue;
}

/************************************************************
* Function: char2int ()                                     *
* Date Created:                                             *
* Date Last Modified:                                       *
* Description: Converts an char digit to it's integer value *
* Input parameters: The character digit                     *
* Returns: The integer value of the character digit         *
* Preconditions: None (other functions to be run first)     *
* Postconditions: The integer value of the character        *
*             digit is returned                             *
************************************************************/
int char2Int (char digit) {
	return (digit - 48);
}

/************************************************************
* Function: getWeight ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads 3-digit weight (lbs). Validates range
*              100–999. Re-prompts until valid.
* Input parameters: None
* Returns: int weight
* Preconditions: UART initialized
* Postconditions: Valid weight returned
************************************************************/
int getWeight() {
	char inputWeight100;
	char inputWeight10;
	char inputWeight1;
	int totalWeight = 0;

	do {
		UARTprintf("Enter Weight (100–999): ");
		inputWeight100 = UARTgetc_echo();
		inputWeight10  = UARTgetc_echo();
		inputWeight1   = UARTgetc_echo();
		UARTprintf("\n");

		if (!isDigitChar(inputWeight100) || !isDigitChar(inputWeight10) || !isDigitChar(inputWeight1)) {
			UARTprintf("ERROR: weight must be three digits.\n");
			continue;
		}

		totalWeight  = char2Int(inputWeight100) * 100;
		totalWeight += char2Int(inputWeight10)  * 10;
		totalWeight += char2Int(inputWeight1);

		if (totalWeight < 100 || totalWeight > 999) {
			UARTprintf("ERROR: weight out of range (100–999).\n");
		}
	} while (totalWeight < 100 || totalWeight > 999);

	return totalWeight;
}

/************************************************************
* Function: getHeightFeet ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads height in feet (0–9). Re-prompts until valid.
* Input parameters: None
* Returns: int feet
* Preconditions: UART initialized
* Postconditions: Valid feet returned
************************************************************/
int getHeightFeet() {
	char inputHeightFeet;
	do {
		UARTprintf("Enter height (Feet 0–9): ");
		inputHeightFeet = UARTgetc_echo();
		UARTprintf("\n");

		if (!isDigitChar(inputHeightFeet)) {
			UARTprintf("ERROR: feet must be a digit 0–9.\n");
			continue;
		}
		if (char2Int(inputHeightFeet) < 0 || char2Int(inputHeightFeet) > 9) {
			UARTprintf("ERROR: feet must be 0–9.\n");
		}
	} while (char2Int(inputHeightFeet) < 0 || char2Int(inputHeightFeet) > 9);

	return char2Int(inputHeightFeet);
}

/************************************************************
* Function: getHeightInches ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads height in inches (0–9). Re-prompts until valid.
* Input parameters: None
* Returns: int inches
* Preconditions: UART initialized
* Postconditions: Valid inches returned
************************************************************/
int getHeightInches() {
	char inputHeightInches;
	do {
		UARTprintf("Enter height (Inches 0–9): ");
		inputHeightInches = UARTgetc_echo();
		UARTprintf("\n");

		if (!isDigitChar(inputHeightInches)) {
			UARTprintf("ERROR: inches must be a digit 0–9.\n");
			continue;
		}
		if (char2Int(inputHeightInches) < 0 || char2Int(inputHeightInches) > 9) {
			UARTprintf("ERROR: inches must be 0–9.\n");
		}
	} while (char2Int(inputHeightInches) < 0 || char2Int(inputHeightInches) > 9);

	return char2Int(inputHeightInches);
}

/************************************************************
* Function: convertHeightToInches ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Converts feet+inches to total inches.
* Input parameters: heightFeet (int), heightInches (int)
* Returns: int totalInches
* Preconditions: Valid inputs
* Postconditions: Height in inches returned
************************************************************/
int convertHeightToInches(int heightFeet, int heightInches) {
	return (heightFeet * 12) + heightInches;
}

/************************************************************
* Function: getAge ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads age as two digits (10–99). Re-prompts until valid.
* Input parameters: None
* Returns: int age
* Preconditions: UART initialized
* Postconditions: Valid age returned
************************************************************/
int getAge() {
	char inputAge10;
	char inputAge1;
	int age = 0;

	do {
		UARTprintf("Enter age (10–99): ");
		inputAge10 = UARTgetc_echo();
		inputAge1  = UARTgetc_echo();
		UARTprintf("\n");

		if (!isDigitChar(inputAge10) || !isDigitChar(inputAge1)) {
			UARTprintf("ERROR: age must be two digits.\n");
			continue;
		}

		age  = char2Int(inputAge10) * 10;
		age += char2Int(inputAge1);

		if (age < 10 || age > 99) {
			UARTprintf("ERROR: age out of range (10–99).\n");
		}
	} while (age < 10 || age > 99);

	return age;
}

/************************************************************
* Function: getGender ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads gender. Strict: 'M' or 'F'. Re-prompts until valid.
* Input parameters: None
* Returns: char gender
* Preconditions: UART initialized
* Postconditions: Returns 'M' or 'F'
************************************************************/
char getGender() {
    char gender;
    do {
        UARTprintf("Enter gender (M/F): ");
        gender = UARTgetc_echo();
        UARTprintf("\n");

        if (gender != 'M' && gender != 'F') {
            UARTprintf("ERROR: enter M or F.\n");
        }
    } while (gender != 'M' && gender != 'F');

    return gender;
}

/************************************************************
* Function: getActivityLevel ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads activity level (0–4). Validates digit and range.
* Input parameters: None
* Returns: int level
* Preconditions: UART initialized
* Postconditions: Valid level returned
************************************************************/
int getActivityLevel() {
	char activityLevelChar;
	int activityLevel = 0;

	do {
		UARTprintf("For the Following:\n 0: Sedentary\n 1: Low\n 2: Moderate\n 3: High\n 4: Extra\nEnter activity level (0–4): ");
		activityLevelChar = UARTgetc_echo();
		UARTprintf("\n");

		if (!isDigitChar(activityLevelChar)) {
			UARTprintf("ERROR: enter a digit 0–4.\n");
			continue;
		}

		activityLevel = char2Int(activityLevelChar);

		if (activityLevel < 0 || activityLevel > 4) {
			UARTprintf("ERROR: activity must be 0–4.\n");
		}
	} while (activityLevel < 0 || activityLevel > 4);

	return activityLevel;
}

/************************************************************
* Function: calculateBMR ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Computes BMR using Harris-Benedict formula.
* Input parameters: weightLbs (int), heightInches (int), age (int), gender (char)
* Returns: double BMR
* Preconditions: Valid inputs
* Postconditions: BMR returned
************************************************************/
double calculateBMR(int weightLbs, int heightInches, int age, char gender) {
	if (gender == 'F') {
		return 655.00 + (4.35 * weightLbs) + (4.7 * heightInches) - (4.7 * age);
	} else {
		return 66.00 + (6.23 * weightLbs) + (12.7 * heightInches) - (6.8 * age); 
	}
}

/************************************************************
* Function: appliedActivity ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Applies activity multiplier (0–4) to BMR.
* Input parameters: bmr (double), level (int)
* Returns: double dailyCalories
* Preconditions: Valid BMR and activity level
* Postconditions: Total calories returned
************************************************************/
double appliedActivity(double bmr, int level) {
	double mulitplier;

	switch (level) {
		case 0: mulitplier = 1.2;
			break;
		case 1: mulitplier = 1.375;
			break;
		case 2: mulitplier = 1.55;
			break;
		case 3: mulitplier = 1.725;
			break;
		case 4: mulitplier = 1.9;
			break;
		default: mulitplier = 1.2;
			break;
	}
	return bmr * mulitplier;
}

/************************************************************
* Function: displayBMR ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Prints BMR value to UART.
* Input parameters: bmr (double)
* Returns: void
* Preconditions: UART initialized
* Postconditions: BMR printed
************************************************************/
void displayBMR(double bmr) {
	UARTprintf("BMR = %f\n", bmr);
}

/************************************************************
* Function: displayCalories ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Prints total daily calories to UART.
* Input parameters: calories (double)
* Returns: void
* Preconditions: UART initialized
* Postconditions: Calories printed
************************************************************/
void displayCalories(double calories) {
	UARTprintf("Total daily calorie needs: %f\n", calories);
}