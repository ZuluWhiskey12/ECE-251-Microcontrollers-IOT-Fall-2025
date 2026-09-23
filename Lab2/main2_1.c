//*****************************************************************************
// Lab 2.1 - Perpendicular Bisector Generator
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

    // Hello!
    UARTprintf("Hello, Lab 2!\n");
	UARTprintf("Problem 1\n\n");
	
	//Variables to capture input from Termite
	char inputX1 = '\0';
	char inputY1 = '\0';
	char inputX2 = '\0';
	char inputY2 = '\0';
	//***********************
	//Problem 1 Part 1
	//***********************
	//Ask for and receive point 1's X coordinate place in inputX1
	UARTprintf("Enter point1 X value: ");
	inputX1 = UARTgetc_echo();
	UARTprintf("\n");
	//Ask for and receive point 1's Y coordinate place in inputY1
	UARTprintf("Enter point1 y value: ");
	inputY1 = UARTgetc_echo();
	UARTprintf("\n");
	//Ask for and receive point 2's X coordinate place in inputX2
	UARTprintf("Enter point2 X value: ");
	inputX2 = UARTgetc_echo();
	UARTprintf("\n");
	//Ask for and receive point 2's Y coordinate place in inputY2
	UARTprintf("Enter point2 y value: ");
	inputY2 = UARTgetc_echo();
	UARTprintf("\n");
	
	int X1 = 0;
	int Y1 = 0;
	int X2 = 0;
	int Y2 = 0;
	
	//Using char2int convert inputX1 to X1
	X1 = char2int(inputX1);
	//Using char2int convert inputY1 to Y1
	Y1 = char2int(inputY1);
	//Using char2int convert inputX2 to X2
	X2 = char2int(inputX2);
	//Using char2int convert inputY2 to Y2
	Y2 = char2int(inputY2);
	
	//Output (X1, Y1) and (X2, Y2)
	UARTprintf("(%d, %d) and (%d, %d)\n",X1,Y1,X2,Y2);
	UARTprintf("\n");

	//***********************
	//Problem 1 Part 2
	//***********************
	double slope = 0;
	
	//Calculate the slope for points (X1, Y1) and (X2, Y2)
	double Y = Y2 - Y1;
	double X = X2 - X1;

	slope = Y/X;
	//Print out slope
	UARTprintf("Slope: %f\n", slope);

	//***********************
	//Problem 1 Part 3
	//***********************
	
	//Define and initialize midpointX and midpointY
	double midpointX = (double)(X1 + X2) / 2;
	double midpointY = (double)(Y1 + Y2) / 2;
	//Calculate the midpoint between (X1, Y1) and (X2, Y2)

	//Some code to print out midpoint
	UARTprintf("Midpoint: (%f, %f)\n", midpointX, midpointY);
	
	//***********************
	//Problem 1 Part 4
	//***********************
	
	//Define and initialize perpBisectorSlope
	double perpBisectorSlope = -(1/slope);
	//Calculate the perpendicular bisector slope.

	
	UARTprintf("Slope of Perpendicular Bisector: %f\n", perpBisectorSlope);
	
	// //***********************
	//Problem 1 Part 4
	//***********************

	//Define and initialize perpBisectorYIntercept
	double perpBisectorYIntercept = midpointY - (perpBisectorSlope * midpointX);
	//Calculate the perpendicular bisector y-intercept.
	

	UARTprintf("Y-Intercept of Perpendicular Bisector: %f\n", perpBisectorYIntercept);
	UARTprintf("y = %f * x + %f\n", perpBisectorSlope, perpBisectorYIntercept);
		
    // We are finished. Since this is an embedded system, hang around
    while(1)
    {
    }
}

