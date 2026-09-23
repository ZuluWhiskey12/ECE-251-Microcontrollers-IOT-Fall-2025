//*****************************************************************************
// Lab Practical
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h> // Included the string.h library for strlen
#include "ECE251_PIO_util.h"
/*
 * Write a program to print to all the vowels in a string
 *   1. Prompt user for string input
 *   2. Store input into an Array
 *   3. identify and store vowels in the Array
 *   4. Print out the vowels
 */
int main(void)
{
    // Initialize the UART.
    UARTsetup(9600);

    // Hello!
    UARTprintf("Practical!\n\n");
    char userInput[128], vowels[128];
    char inputChar;
    int index = 0, vowelIndex = 0;

    UARTprintf("Enter String\n\n"); // prompt user

    do
    {
        inputChar = UARTgetc_echo();    // read + echo char
        userInput[index++] = inputChar; // store char
    } while (inputChar != '\r' &&       // until Enter
             inputChar != '\n' &&       // CR or LF
             index < 127); // leave room for '\0'

    userInput[index] = '\0'; // terminate input

    for (index = 0; userInput[index] != '\0'; index++)
    {
        inputChar = userInput[index]; // current char
        switch (inputChar)
        { // check vowel
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            vowels[vowelIndex++] = inputChar; // save vowel
            vowels[vowelIndex++] = ' ';       // save space
            break;
        }
    }

    vowels[vowelIndex] = '\0';                 // terminate vowels
    UARTprintf("User Input: %s\n", userInput); // print user input
    UARTprintf("Vowels: %s\n", vowels);        // print vowels
}
