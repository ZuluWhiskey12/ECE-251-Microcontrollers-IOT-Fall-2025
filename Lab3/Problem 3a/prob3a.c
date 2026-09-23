/*****************************************************************************
* Programmer: Zach Whitmore
* Class: ECE 251; Friday 8am Lab Section
* Programming Assignment: Lab 3a - BMI Calculator
*
* Date: September 26, 2025
*
* Description:
*   Functions for Lab 3a. Includes digit validation, height/weight
*   readers, BMI calculation, and display. Updated to include
*   error checking (Lab 3d).
*
* Relevant Formulas:
*   BMI = ((weight in pounds) / (height in inches)^2) * 703
*****************************************************************************/

#include "prob3a.h"
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
* Function: readDigitWithPrompt ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Prints prompt, reads one char, re-prompts until
*              a digit is entered.
* Input parameters: prompt (char*)
* Returns: char digit ('0'..'9')
* Preconditions: UART initialized
* Postconditions: Valid digit returned
************************************************************/
char readDigitWithPrompt(char *prompt) {
    char ch;

    do {
        UARTprintf("%s", prompt);
        ch = UARTgetc_echo();
        UARTprintf("\n");

        if (!isDigitChar(ch)) {
            UARTprintf("ERROR: %c is not a digit (0–9).\n", ch);
        }
    } while (!isDigitChar(ch));
	return ch;
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
int char2int (char digit) {
	return (digit - 48);
}

/*************************************************************
* Function: displayBmi ()                                  *
* Date Created:                                             *
* Date Last Modified:                                       *
* Description: Takes the bmi value as input and print       *
*		the value out to the serial monitor over UART       *
* Input parameters: BMI as a double                         *
* Returns: None                                             *
* Preconditions: BMI must be calculated, UART initialized   *
* Postconditions: BMI printed to Termite                    *
*************************************************************/
void displayBmi (double bmi) {
	UARTprintf("\nBMI: %f\n", bmi); // print out BMI over UART
}

/************************************************************
* Function: getWeight ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads a 3-digit weight (lbs). Validates range
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

		/* digit check */
		if (!isDigitChar(inputWeight100) || !isDigitChar(inputWeight10) || !isDigitChar(inputWeight1)) {
			UARTprintf("ERROR: weight must be three digits.\n");
			continue;
		}

		totalWeight = char2int(inputWeight100) * 100;
		totalWeight += char2int(inputWeight10)  * 10;
		totalWeight += char2int(inputWeight1);

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
		inputHeightFeet = readDigitWithPrompt("Enter height (Feet 0–9): ");
		if (char2int(inputHeightFeet) < 0 || char2int(inputHeightFeet) > 9) {
			UARTprintf("ERROR: feet must be 0–9.\n");
		}
	} while (char2int(inputHeightFeet) < 0 || char2int(inputHeightFeet) > 9);
	return char2int(inputHeightFeet);
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
		inputHeightInches = readDigitWithPrompt("Enter height (Inches 0–9): ");
		if (char2int(inputHeightInches) < 0 || char2int(inputHeightInches) > 9) {
			UARTprintf("ERROR: inches must be 0–9.\n");
		}
	} while (char2int(inputHeightInches) < 0 || char2int(inputHeightInches) > 9);
	return char2int(inputHeightInches);
}

/************************************************************
* Function: getHeight ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads both feet and inches. Stores them in pointers.
* Input parameters: heightFeet (int*), heightInches (int*)
* Returns: void
* Preconditions: UART initialized
* Postconditions: *heightFeet and *heightInches set
************************************************************/
void getHeight(int *heightFeet, int *heightInches) {
    *heightFeet = getHeightFeet();
    *heightInches = getHeightInches();
}

/************************************************************
* Function: convertHeightToInches ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Converts feet+inches to total inches.
* Input parameters: heightFeet (int), heightInches (int)
* Returns: int totalInches
* Preconditions: Inputs valid
* Postconditions: Returns height in inches
************************************************************/
int convertHeightToInches(int heightFeet, int heightInches) {
	return (heightFeet *12) + heightInches;
}

/************************************************************
* Function: calculateBmi ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Computes BMI = (weight / height^2) * 703.
*              Returns 0 if height == 0.
* Input parameters: weightInPounds (int), heightFeet (int), heightInches (int)
* Returns: double BMI
* Preconditions: Valid weight and height
* Postconditions: BMI returned
************************************************************/
double calculateBmi(int weightInPounds, int heightFeet, int heightInches) {
	int height = convertHeightToInches(heightFeet, heightInches);
	if (height == 0) {
		UARTprintf("ERROR: total height cannot be 0.\n");
		return 0.0;
	}
	return ((double)weightInPounds / (height * (double)height)) * 703.0;
}