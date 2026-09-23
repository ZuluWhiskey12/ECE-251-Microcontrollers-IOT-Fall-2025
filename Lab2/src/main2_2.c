//*****************************************************************************
// Lab 2.2 - BMI Calculator
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ECE251_PIO_util.h"

//Converts a character digit ('0' - '9') to its integer value (0-9)
//Input: char digit
//Return: int representation of digit 
int char2int (char digit)
{
	//Modify this function to return the correct values	
	int num = digit - 48;
	return num;
}

int main(void)
{
    // Initialize the UART.
    UARTsetup(9600);

    // Problem 2.2!
    UARTprintf("Hello, Lab 2!\n");
	UARTprintf("Problem 2\n\n");
	
	//Variables to capture input from Termite
	char inputHeightFeet = '\0';
	char inputHeightInches = '\0';
	char inputWeight100 = '\0';
	char inputWeight10 = '\0';
	char inputWeight1 = '\0';
		
	//Program Here:
	UARTprintf("Enter height (Feet): ");
	inputHeightFeet = UARTgetc_echo();
	UARTprintf("\n");

	UARTprintf("Enter height (Inches): ");
	inputHeightInches = UARTgetc_echo();
	UARTprintf("\n");

	int height = (char2int(inputHeightFeet) * 12) + char2int(inputHeightInches); 
	UARTprintf("Height in inches = %d", height);
	UARTprintf("\n");

	UARTprintf("Enter Weight: ");
	inputWeight100 =  UARTgetc_echo();
	inputWeight10 =  UARTgetc_echo();
	inputWeight1 =  UARTgetc_echo();
	UARTprintf("\n");

	double totalWeight = 0;
	totalWeight += char2int(inputWeight100) * 100;
	totalWeight += char2int(inputWeight10) * 10;
	totalWeight += char2int(inputWeight1);

	
	UARTprintf("Total Weight: %f", totalWeight);
	UARTprintf("\n");

	int heightSquared = height * height;
    double bmi = (totalWeight/heightSquared) * 703;
		
	UARTprintf("BMI: %f\n", bmi);


    // We are finished. Since this is an embedded system, hang around
    while(1)
    {    
    }
}

