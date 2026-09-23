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
int charToInt(char c)
{
    return c - '0';
}

int main(void)
{
    // Initialize the UART.
    UARTsetup(9600);

    // Hello!
    UARTprintf("Practical!\n\n");


    int numbers[128];
    char inputChar;
    int index = 0;

    UARTprintf("Enter digits (0-9), then press Enter:\n\n");

    do
    {
        inputChar = UARTgetc_echo();        // read + echo char

        if (inputChar == '\r' || inputChar == '\n')
            break;                          // stop on Enter

        numbers[index++] = charToInt(inputChar); // store digit as int
    } while (index < 128);

    for (int i = 0; i < index; i++)
    {
        int n = numbers[i];

        if ((n % 2) == 0)
            UARTprintf("%d is even\n", n);
        else
            UARTprintf("%d is odd\n", n);
    }

    while (1)
    {
    }
}