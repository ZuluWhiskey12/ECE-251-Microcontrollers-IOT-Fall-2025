//*************************************************************** 
// Lab7b.s  
// Programmer:
// Description:  
//***************************************************************	

//*************************************************************** 
// EQU Directives
// These directives do not allocate memory
//***************************************************************
    .equ DELAY_CLOCKS, 53333      //100 millisecond delay

    .equ GPIO_PORTA_BASE,	0x40004000	// Port B base address
    .equ GPIO_PORTB_BASE,	0x40005000	// Port B base address
    .equ GPIO_PORTC_BASE,	0x40006000	// Port C base address
    .equ GPIO_PORTD_BASE,	0x40007000	// Port D base address
    .equ GPIO_PORTE_BASE,	0x40024000	// Port E base address\
    .equ GPIO_PORTF_BASE,	0x40025000	// Port F base address

    .equ GPIO_DATA,     0x3FC	  // Data R/W (all bits)
    .equ GPIO_DIR,      0x400	  // Direction
    .equ GPIO_RIS,      0x414   // Raw interrupt state
    .equ GPIO_MIS,      0x418   // Masked interrupt state
    .equ GPIO_ICR,      0x41C   // Interrupt clear
    .equ GPIO_AFSEL,    0x420   // Alternate function
    .equ GPIO_DR2R,     0x500   // 2mA drive select
    .equ GPIO_DR4R,     0x504   // 4mA drive select
    .equ GPIO_DR8R,     0x508   // 8mA drive select
    .equ GPIO_ODR,      0x50C   // Open drain select
    .equ GPIO_PUR,      0x510   // Pull-ups
    .equ GPIO_PDR,      0x514   // Pull-downs
    .equ GPIO_SLR,      0x518   // Slew rate control
    .equ GPIO_DEN,      0x51C   // Digital enable
    .equ GPIO_LOCK,     0x520   // Lock
    .equ GPIO_CR,       0x524   // Commit
    .equ GPIO_AMSEL,    0x528   // Analog mode select
    .equ GPIO_PCTL,     0x52c   // Port control

        .equ SYSCTL_RCGCGPIO,	0x400FE608

//***************************************************************
// Data Section in READWRITE
// Values of the data in this section are initialazed, 
// and labels can be used in the program to change the data values.
//***************************************************************
//                .section .data
								
//***************************************************************
// Directives - This Data Section is part of the code
// It is in the read only section  so values cannot be changed.
//***************************************************************
                .section .rodata
charsAtIndex:           .asciz "D#0*C987B654A321"
columnMasks:            .byte 0x01, 0x02, 0x10, 0x20

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
        LDR     R1, =SYSCTL_RCGCGPIO
        LDR     R0, [R1]                
        ORR     R0, R0, #0x13 //Starts port clocks A,B,E
        STR     R0, [R1] 	
        nop
        nop
        nop

        //Port A Setup
        //DIR
        LDR     R1, =GPIO_PORTA_BASE      // Data direction setup
        LDR     R0, [R1, GPIO_DIR]
        BIC     R0, #0xFF
        ORR     R0, #0xE0                // Set bits 5,6,7 to output
        STR     R0, [R1, GPIO_DIR]

        //AFSEl
        LDR     R1, =GPIO_PORTA_BASE     // Set up standard GPIO functionality
        LDR     R0, [R1, GPIO_AFSEL]     
        BIC     R0, #0xFF
        STR     R0, [R1, GPIO_AFSEL]

        //Digtal Enable
        LDR     R1, =GPIO_PORTA_BASE    // Enable digital (vs. Analog) function
        LDR     R0, [R1, GPIO_DEN]
        ORR     R0, #0xFF
        STR     R0, [R1, GPIO_DEN] 


        //Port B Setup
        //DIR
        LDR     R1, =GPIO_PORTB_BASE      // Data direction setup
        LDR     R0, [R1, GPIO_DIR]
        BIC     R0, #0xFF
        ORR     R0, #0x10                 // Set bits 5,6,7 to output
        STR     R0, [R1, GPIO_DIR]

        //AFSEl
        LDR     R1, =GPIO_PORTB_BASE     // Set up standard GPIO functionality
        LDR     R0, [R1, GPIO_AFSEL]     
        BIC     R0, #0xFF
        STR     R0, [R1, GPIO_AFSEL]

        //Digtal Enable
        LDR     R1, =GPIO_PORTB_BASE    // Enable digital (vs. Analog) function
        LDR     R0, [R1, GPIO_DEN]
        ORR     R0, #0xFF
        STR     R0, [R1, GPIO_DEN] 
        
        //Pull Up Registers
        LDR     R1, =GPIO_PORTB_BASE    // Pull-up Resistor Register
        MOV     R0, #0x03              
        STR     R0, [R1, GPIO_PUR]      // Store this value into PUR 


        //Port E Setup
        //DIR
        LDR     R1, =GPIO_PORTE_BASE      // Data direction setup
        LDR     R0, [R1, GPIO_DIR]
        BIC     R0, #0xFF
        ORR     R0, #0x00
        STR     R0, [R1, GPIO_DIR]

        //AFSEl
        LDR     R1, =GPIO_PORTE_BASE     // Set up standard GPIO functionality
        LDR     R0, [R1, GPIO_AFSEL]     
        BIC     R0, #0xFF
        STR     R0, [R1, GPIO_AFSEL]

        //Digtal Enable
        LDR     R1, =GPIO_PORTE_BASE    // Enable digital (vs. Analog) function
        LDR     R0, [R1, GPIO_DEN]
        ORR     R0, #0xFF
        STR     R0, [R1, GPIO_DEN] 
        
        //Pull Up Registers
        LDR     R1, =GPIO_PORTE_BASE    // Pull-up Resistor Register
        MOV     R0, #0x30              
        STR     R0, [R1, GPIO_PUR]      // Store this value into PUR 

mainLoop:
        //Program flow
        BL      Scan

        LDR     R1, =charsAtIndex
        ADD     R1, R1, R0
        LDRB    R0, [R1]
        BL      OutChar
        
        MOV     R0, ' '
        BL      OutChar

        BL      NextKey
        B       mainLoop
//Code goes here			

Scan:
        Push            {R4, R5, R6, LR}
        BL              resetRowsLow

pollingForInput:   //Read and Compare bits in R1 and R2 to zero, if zero branch to delay to debounce key press
        //Read input bits from pins 1 and 2 : Inputs are Columns
        LDR             R1, =GPIO_PORTB_BASE
        LDR             R0, [R1, GPIO_DATA]
        AND             R0, #0x3

        //Read input bits for pins 3 and 4
        LDR             R1, =GPIO_PORTE_BASE
        LDR             R2, [R1, GPIO_DATA]
        AND             R2, #0x30

        //Compare value in R0 to value of pins 1 and 2 high, if not equal branch to debounce
        CMP             R0, #0x3
        BNE             debounce

        //Compare value in R2 to value of pins 3 and 4 high, if not equal branch to debounce
        CMP             R2, #0x30
        BNE             debounce

        //return to top of loop if no input detected
        B               pollingForInput

debounce:
        //Branch to Delay to handle debouce
        BL              Delay
        MOV             R4, #0 //Key number
        MOV             R5, #0 //Row pointer
        MOV             R6, #0 //Column pointer

setRowsHigh:
        //Output pins 6 through 8 set to 1 or high: Outputs are rows
        LDR             R1, =GPIO_PORTA_BASE
        MOV             R0, #0xE0
        STR             R0, [R1, GPIO_DATA]

        //Output pin 5 set to 1 or high
        LDR             R1, =GPIO_PORTB_BASE
        MOV             R0, #0x10
        STR             R0, [R1, GPIO_DATA]

        MOV             R6, #0

        //Compare RowPointer to 0, branch to row1 if true 
        CMP             R5, #0
        BEQ             row1

        //Compare RowPointer to 1, branch to row2 if true
        CMP             R5, #1
        BEQ             row2

        //Compare RowPointer to 2, branch to row3 if true
        CMP             R5, #2
        BEQ             row3

        //Compare RowPointer to 3, branch to row4 if true
        CMP             R5, #3
        BEQ             row4

row1:   //Output Pin or Row 1 set to 0
        LDR             R1, =GPIO_PORTB_BASE
        MOV             R0, #0x00
        STR             R0, [R1, GPIO_DATA]

        B               readColumnPointer
row2:   //Output Pin or Row 2 set to 0
        LDR             R1, =GPIO_PORTA_BASE
        MOV             R0, #0xC0
        STR             R0, [R1, GPIO_DATA]

        B               readColumnPointer   
row3:   //Output Pin or Row 3 set to 0
        LDR             R1, =GPIO_PORTA_BASE
        MOV             R0, #0xA0
        STR             R0, [R1, GPIO_DATA]

        B               readColumnPointer
row4:   //Output Pin or Row 4 set to 0
        LDR             R1, =GPIO_PORTA_BASE
        MOV             R0, #0x60
        STR             R0, [R1, GPIO_DATA]

        B               readColumnPointer

readColumnPointer:
        //Read input bits from pins 1 and 2 : Inputs are Columns
        LDR             R1, =GPIO_PORTB_BASE
        LDR             R0, [R1, GPIO_DATA]
        AND             R0, #0x3

        //Read input bits for pins 3 and 4
        LDR             R1, =GPIO_PORTE_BASE
        LDR             R2, [R1, GPIO_DATA]
        AND             R2, #0x30

        //Combines all input bits into one register from simpler testing
        ORR             R3, R0, R2

        //loads in a value from a reference table based on the an offset of the column pointer
        LDR             R1, =columnMasks
        LDRB            R0, [R1, R6]
        
        TST             R3, R0
        BEQ             keyFound
        B               nextColumn

nextColumn:
        //Increment Key and Column pointers
        ADD             R4, #1 
        ADD             R6, #1
        CMP             R6, #4
        BLT             readColumnPointer

        ADD             R5, #1
        CMP             R5, #4
        BLT             setRowsHigh

        BL              resetRowsLow
        B               pollingForInput

keyFound:
        MOV             R0, R4
        POP             {R4, R5, R6, LR}
        BX              LR

resetRowsLow:
        //Force all rows low so idle polling can detect the next press
        LDR             R1, =GPIO_PORTA_BASE
        MOV             R0, #0
        STR             R0, [R1, GPIO_DATA]

        //Also drive PB4 low (row 1) while leaving other bits unchanged
        LDR             R1, =GPIO_PORTB_BASE
        LDR             R2, [R1, GPIO_DATA]
        BIC             R2, #0x10
        STR             R2, [R1, GPIO_DATA]
        BX              LR

NextKey:
        //Read input bits from pins 1 and 2 : Inputs are Columns
        LDR             R1, =GPIO_PORTB_BASE
        LDR             R0, [R1, #GPIO_DATA]
        AND             R0, #0x3

        //Read input bits for pins 3 and 4
        LDR             R1, =GPIO_PORTE_BASE
        LDR             R2, [R1, #GPIO_DATA]
        AND             R2, #0x30

        CMP             R0, #0x3
        BNE             NextKey

        CMP             R2, #0x30
        BNE             NextKey

        B               Scan    

forever:		B			forever

//******************************************
// Subroutine to create delay, 
// DELAY_CLOCKS is the counter which is 
// decremented to zero
//******************************************	
Delay:  LDR R2, =DELAY_CLOCKS // set delay count
del:    SUBS R2, R2, #1       // decrement count
        BNE	del               // if not at zero, do again
        BX	LR                // return when done

//***************************************************************
// End of the program  section
//***************************************************************
		.end
