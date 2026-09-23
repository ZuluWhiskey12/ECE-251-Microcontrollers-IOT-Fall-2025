//*************************************************************** 
// practical.s  
// Programmer: Zach Whitmore
// Description:  See program for comments
//***************************************************************	

//*************************************************************** 
// EQU Directives
// These directives do not allocate memory
//***************************************************************
//                .equ

//***************************************************************
// Data Section in READWRITE
// Values of the data in this section are initialazed, 
// and labels can be used in the program to change the data values.
//***************************************************************
//                .section .data
								
//***************************************************************
// Directives - This Data Section is part of the code
// It is in the read only section  so values cannot be changed.
//
// Created 3 predefined messages for using in the program
//              a. Orignal String:
//              b. ECE2025
//              c. Sum of digits
//***************************************************************
                .section .rodata
Welcome:        .ascii "Pracical ASM!\n\0"
OrignalMsg:     .asciz "Original String: "                //Expected input predefined
inputString:    .asciz "ECE2025"
outputString:   .asciz "Sum of digits: "
//***************************************************************
// Program section					      
//***************************************************************
//LABEL		DIRECTIVE	VALUE			COMMENT
        .section .text
        .align 		2
        .syntax 	unified
        .thumb
	.global  	main			// Make available
//***************************************************************
// 2. main loads and prints welcome and orignal message,and
//    loads input string into register 4 then prints newline.					      
//***************************************************************
main:
        nop
        nop
        LDR     R0, =Welcome
        BL      OutStr

        MOV     R0, #'\n'
        BL      OutChar   

        LDR     R0, =OrignalMsg 
        BL      OutStr

        LDR     R4, =inputString
        MOV     R0, R4
        BL      OutStr

        MOV     R0, #'\n'               //new line
        BL      OutChar

//***************************************************************
// 3. Loop loads in 1 bit from Register storing the input string, 
//    then post increments by 1.    
// 4. Loop checks if bit in R1 is between 0 and 9, if not it loops
//    again. If is goes to sumNumber	
// 6. Loop branches to Done when it finds the null terminator			      
//***************************************************************
loop:
        LDRB    R1, [R4], #1
        CBZ     R1, Done

        CMP     R1, #'0'
        BLT     loop

        CMP     R1, #'9'
        BGT     loop

        BL      sumNumbers
        B       loop

//***************************************************************
// 5. sumNumber converts the chars to ints, and stores the value 
//    into register 5 and loops again.					      
//***************************************************************
sumNumbers:
        MOV     R2, R1
        SUB     R2, R2, #0x30
        ADD     R5, R2

        B       loop

//***************************************************************
// 7. Done prints outputString and sum of numbers, then falls
//    into spin_loop to finish to program					      
//***************************************************************
Done:
        LDR     R0, =outputString
        BL      OutStr

        MOV     R0, R5
        BL      OutHex

//***************************************************************
// End of the program  section
//***************************************************************
spin_loop: B    spin_loop       // we are finished

		.end
