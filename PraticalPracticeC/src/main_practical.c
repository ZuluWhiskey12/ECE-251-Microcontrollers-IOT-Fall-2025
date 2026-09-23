//*****************************************************************************
// Lab Practical
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h> // Included the string.h library for strlen
#include "ECE251_PIO_util.h"

//*****************************************************************************
// charToInt is a helper method that changes char values into integar values
//*****************************************************************************
int charToInt(char c)
{
    return c - 48; // will take char digit and convert in to an integer
}

int main(void)
{
    // Initialize the UART.
    UARTsetup(9600);

    // Hello!
    UARTprintf("Practical!\n\n");

    //*****************************************************************************
    // Intialized variables to be used in program, and prints out user prompt
    //*****************************************************************************
    char inputChar;
    char userInput[128];

    int index = 0;
    int numSum = 0;

    UARTprintf("Enter a string: ");

    //*****************************************************************************
    // While the user has not hit enter or exceeded 127 chars the program continues
    // to read input char and store it into an array. Once exit conditions are 
    // meet output null terminator at the last index of the array
    //*****************************************************************************
    do {
        inputChar = UARTgetc_echo();
        userInput[index] = inputChar;
        index++;
    } while (inputChar != '\r' && inputChar != '\n' && index < 127);
    userInput[--index] = '\0';

    //*****************************************************************************
    // iterates through userInput array and checks if the values are between 0 and 
    // 9. For values in the range it calls helper method charToInt to convert
    // them to integers and stores the integar value into a sum.
    //*****************************************************************************
    for (int i = 0; i < index; i++) {
        int number = userInput[i];

        if (number >= '0' && number <= '9')
        {
            numSum += charToInt(number);
        }
    }

    //*****************************************************************************
    // Prints out the users input from the userInput array, and prints out the 
    // sum of the digits collected in the nested loop above.
    //*****************************************************************************
    UARTprintf("\nOrignal String: %s\n", userInput);
    UARTprintf("Sum of Digits: %d\n", numSum);

    // We are finished. Since this is an embedded system, hang around
    while (1) { }
}
