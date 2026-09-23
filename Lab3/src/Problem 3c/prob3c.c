/*****************************************************************************
* Programmer: Zach Whitmore
* Class: ECE 251; Friday 8am Lab Section
* Programming Assignment: Lab 3c - GPA Calculator
*
* Date: September 26, 2025
*
* Description:
*   Functions for Lab 3c. Reads grade and credit data for five
*   classes, maps grades to grade points, computes GPA, and
*   displays result. Includes input validation and guards (Lab 3d).
*
* Relevant Formulas:
*   GPA = (Sum of (GradePoints * Credits)) / (Sum of Credits)
*****************************************************************************/

#include "prob3c.h"

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
* Function: getAllClasses ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads 5 classes (credits + grade each).
* Input parameters: grades[], credits[]
* Returns: void
* Preconditions: UART initialized
* Postconditions: Arrays filled
************************************************************/
void getAllClasses(char grades[], int credits[]) {
    for (int i = 0; i < 5; i++) {
        credits[i] = getCredits(i);
        grades[i] = getGrade(i);
    }
}

/************************************************************
* Function: getGrade ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads a grade (A/B/C/D/F). Re-prompts until valid.
* Input parameters: index (int)
* Returns: char grade
* Preconditions: UART initialized
* Postconditions: Valid grade returned
************************************************************/
char getGrade(int index) {
	char grade;
	do {
		UARTprintf("Enter letter grade for class %d (A/B/C/D/F): ", index + 1);
		grade = UARTgetc_echo();
		UARTprintf("\n");

		if (grade != 'A' && grade != 'B' && grade != 'C' && grade != 'D' && grade != 'F') {
			UARTprintf("ERROR: grade must be A, B, C, D, or F.\n");
		}
	} while (grade != 'A' && grade != 'B' && grade != 'C' && grade != 'D' && grade != 'F');

	return grade;
}

/************************************************************
* Function: gradeToPoints ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Converts grade letter to grade points (A=4…F=0).
* Input parameters: grade (char)
* Returns: int gradePoints
* Preconditions: grade is A/B/C/D/F
* Postconditions: Points returned
************************************************************/
int gradeToPoints(char grade) {
    switch (grade) {
        case 'A': return 4;
        case 'B': return 3;
        case 'C': return 2;
        case 'D': return 1;
        default:  return 0;
    }
}

/************************************************************
* Function: getCredits ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Reads credits (0–9). Re-prompts until valid.
* Input parameters: index (int)
* Returns: int credits
* Preconditions: UART initialized
* Postconditions: Valid credits returned
************************************************************/
int getCredits(int index) {
	char charCredit;
	int credit = -1;   /* SENTINEL: invalid until proven valid */

	do {
		UARTprintf("Enter credits for class %d (0–9): ", index + 1);
		charCredit = UARTgetc_echo();
		UARTprintf("\n");

		if (!isDigitChar(charCredit)) {
			UARTprintf("ERROR: credits must be a single digit 0–9.\n");
			credit = -1;       /* keep it invalid so the loop repeats */
			continue;
		}

		credit = char2Int(charCredit);

		if (credit < 0 || credit > 9) {
			UARTprintf("ERROR: credits must be 0–9.\n");
			credit = -1;       /* force re-prompt on out-of-range */
		}
	} while (credit < 0 || credit > 9);

	return credit;
}

/************************************************************
* Function: computeGPA ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Computes GPA = (totalPoints / totalCredits).
* Input parameters: grades[], credits[]
* Returns: double GPA
* Preconditions: Arrays filled
* Postconditions: GPA returned (0 if totalCredits == 0)
************************************************************/
double computeGPA(char grades[], int credits[]) {
	int totalGradePoints = 0;
	int totalCredits = 0;

	for (int i = 0; i < 5; i++) {
		totalGradePoints += gradeToPoints(grades[i]) * credits[i];
		totalCredits     += credits[i];
	}

	if (totalCredits == 0) {
		UARTprintf("ERROR: total credits cannot be 0.\n");
		return 0.0;
	}

	UARTprintf("Total Grade Points: %d\n", totalGradePoints);
	UARTprintf("Total Credits: %d\n", totalCredits);

	return (double)totalGradePoints / totalCredits;
}

/************************************************************
* Function: displayGPA ()
* Date Created: September 19, 2025
* Date Last Modified: September 26, 2025
* Description: Prints GPA to UART.
* Input parameters: gpa (double)
* Returns: void
* Preconditions: UART initialized
* Postconditions: GPA printed
************************************************************/
void displayGPA(double gpa) {
    UARTprintf("GPA: %f\n", gpa);
}
