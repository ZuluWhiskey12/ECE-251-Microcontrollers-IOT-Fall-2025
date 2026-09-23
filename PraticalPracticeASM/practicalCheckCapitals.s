//*************************************************************** 
// practical.s  
// Programmer:
// Description:  
//***************************************************************	

/*
* Write a program to count upper case letters in a string created from user input
*   1. Define string to that will be checked
*   2. Load that string to a register
*   3. Count uppercase letter in the Array
*   4. Print out the count
*/
//*************************************************************** 
// EQU Directives
// These directives do not allocate memory
//***************************************************************
                //.equ

//***************************************************************
// Data Section in READWRITE
// Values of the data in this section are initialazed, 
// and labels can be used in the program to change the data values.
//***************************************************************
                //.section .data
								
//***************************************************************
// Directives - This Data Section is part of the code
// It is in the read only section  so values cannot be changed.
//***************************************************************
                .section .rodata
Welcome:        .ascii "Pracical ASM!\n\0"
inStr:          .asciz      "Go CSU Rams" //Create string to be checked in read only data
outStr:         .asciz      "The count is: "

//***************************************************************
// Program section					      
//***************************************************************
//LABEL		DIRECTIVE	VALUE			COMMENT
        .section .text
        .align 		2
        .syntax 	unified
        .thumb
	.global  	main			// Make available
main:
        nop
        nop

        LDR     R0, =Welcome
        BL      OutStr
			
        MOV         R4, #0          //Create count in R4
        LDR         R0, =inStr     //read in the string we created in read only data, which we will check for Upper case letters

loop:   
        LDRB       R1, [R0], #1     //load in one bit of the string at a time
        CBZ        R1, Done         //Branch to done if R1 is equal to zero

        CMP        R1, #'A'          //CMP value in R1 to ASCII A (Can use decimal or hex value of ASCII Char)
        BLT        loop             //If less then ASCII A loop again

        CMP        R1, #'Z'          //Compare value in R1 to ASCII Z 
        BGT        loop             //If great then ASCII Z loop again

        ADD        R4, #1           //Increment the count by one if the value got through both compares
        B          loop             //Loop after incrementing the counter

Done:   
        LDR        R0, =outStr      //load output message into R0
        BL         OutStr           //Print out message using subroutine OutStr

        MOV        R0, R4          //Put 10 into R1     
        BL         OutHex           //Print outs least signifacant digit

        MOV        R0, #'\n'
        BL         OutChar

//***************************************************************
// End of the program  section
//***************************************************************
spin_loop: B    spin_loop       // we are finished

		.end
