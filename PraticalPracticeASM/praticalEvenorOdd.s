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
numsLabel:  .asciz "Digit tests:\n"
numStr:     .asciz "12340"          // predefined digits
evenMsg:    .asciz " is even\n"
oddMsg:     .asciz " is odd\n"

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

        LDR     R0, =Welcome        // print banner
        BL      OutStr

        LDR     R0, =numsLabel      // "Digit tests:"
        BL      OutStr

        LDR     R4, =numStr         // R4 = &numStr (pointer)

loop:
        LDRB    R1, [R4], #1        // R1 = *R4; R4++
        CBZ     R1, spin_loop       // if 0, end of string

        MOV     R2, R1              // R2 = digit char
        SUB     R2, R2, #'0'        // R2 = digit 0–9
        ANDS    R2, R2, #1          // test LSB
        BEQ     evenCase            // if 0 => even

        // oddCase:
        MOV     R0, R1              // print digit
        BL      OutChar
        LDR     R0, =oddMsg         // print " is odd"
        BL      OutStr
        B       loop

evenCase:
        MOV     R0, R1              // print digit
        BL      OutChar
        LDR     R0, =evenMsg        // print " is even"
        BL      OutStr
        B       loop


//***************************************************************
// End of the program  section
//***************************************************************
spin_loop: B    spin_loop       // we are finished

		.end
