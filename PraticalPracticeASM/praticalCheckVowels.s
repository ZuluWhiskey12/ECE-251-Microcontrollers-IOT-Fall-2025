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
inStr:          .asciz  "Go CSU Rams" //Create string to be checked in read only data
vLabel:         .asciz "Vowels: "

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

        LDR     R0, =Welcome           // R0 = welcome string
        BL      OutStr                 // print welcome

        LDR     R0, =vLabel            // R0 = "Vowels: "
        BL      OutStr                 // print label

        LDR     R4, =inStr             // R0 = &inStr

loop:
        LDRB    R1, [R4], #1           // R1 = *R0; R0++
        CBZ     R1, Done               // if R1 == 0, done

        // check for vowel (lowercase + uppercase)
        CMP     R1, #'a'
        BEQ     printVowel

        CMP     R1, #'e'
        BEQ     printVowel

        CMP     R1, #'i'
        BEQ     printVowel

        CMP     R1, #'o'
        BEQ     printVowel

        CMP     R1, #'u'
        BEQ     printVowel

        CMP     R1, #'A'
        BEQ     printVowel

        CMP     R1, #'E'
        BEQ     printVowel

        CMP     R1, #'I'
        BEQ     printVowel

        CMP     R1, #'O'
        BEQ     printVowel

        CMP     R1, #'U'
        BEQ     printVowel

        B       loop                   // not a vowel, next char

printVowel:
        MOV     R0, R1                 // R0 = vowel char
        BL      OutChar                // print vowel

        MOV     R0, #' '               // R0 = space
        BL      OutChar                // print space
        B       loop                   // next char

Done:
        MOV     R0, #'\n'              // R0 = newline
        BL      OutChar                // print newline


//***************************************************************
// End of the program  section
//***************************************************************
spin_loop: B    spin_loop       // we are finished

		.end
