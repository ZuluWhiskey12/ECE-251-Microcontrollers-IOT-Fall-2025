//*****************************************************************************
// Lab Practical
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h> // Included the string.h library for strlen
#include "ECE251_PIO_util.h"
/*
 * Write a program to count upper case letters in a string created from user input
 *   1. Prompt user for string input
 *   2. Store input into an Array
 *   3. Count uppercase letter in the Array
 *   4. Print out the count
 */
int main(void)
{
    // Initialize the UART.
    UARTsetup(9600);

    // Hello!
    UARTprintf("Practical!\n\n");

#define maxLine 128
    int position = 0;

    char inStr[maxLine];
    char inChar;

    UARTprintf("Enter a String\n\n");

    do
    {
        inChar = UARTgetc_echo();
        inStr[position] = inChar;
        position++;

    } while (inChar != '\r' && inChar != '\n' && position < maxLine - 1);

    inStr[--position] = '\0';

    int count = 0;
    for (int i = 0; inStr[i] != '\0'; i++)
    {
        char c = inStr[i];
        if ((c >= 'A') && (c <= 'Z'))
        {
            count++;
        }
    }

    UARTprintf("\nThe count is: %d", count);
    // We are finished. Since this is an embedded system, hang around
    while (1)
    {
    }
}
