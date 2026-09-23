//*************************************************************** 
// Lab7a.s  
// Programmer:
// Description:  
//***************************************************************	

//*************************************************************** 
// EQU Directives
// These directives do not allocate memory
//***************************************************************
    .equ DELAY_CLOCKS, 533333    // To be filled in....

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
    .equ Lock_Key,              0x4C4F434B

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
//                .section .rodata

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
        //Enable Clock for PORT F
        LDR     R1, =SYSCTL_RCGCGPIO
        LDR     R0, [R1]                
        ORR     R0, R0, #0x20
        STR     R0, [R1] 	
        nop
        nop
        nop

        //Unlock PORT F
        LDR     R1, =GPIO_PORTF_BASE    // load R1 with PortF base
        LDR     R0, =Lock_Key           // load R0 with lock key
        STR     R0, [R1, GPIO_LOCK]     // store key in PORTF_LOCK_R
        MOV     R0, #0xFF               // 1 means allow access
        STR     R0, [R1, GPIO_CR] 	

        //DIR
        LDR     R1, =GPIO_PORTF_BASE      // Data direction setup
        LDR     R0, [R1, GPIO_DIR]
        BIC     R0, #0xFF
        ORR     R0, #0x0E                 // Set bits 1,2,3 as output; 0,4 as input
        STR     R0, [R1, GPIO_DIR]

        //AFSEl
        LDR     R1, =GPIO_PORTF_BASE     // Set up standard GPIO functionality
        LDR     R0, [R1, GPIO_AFSEL]     
        BIC     R0, #0xFF
        STR     R0, [R1, GPIO_AFSEL]

        //Digtal Enable
        LDR     R1, =GPIO_PORTF_BASE    // Enable digital (vs. Analog) function
        LDR     R0, [R1, GPIO_DEN]
        ORR     R0, #0xFF
        STR     R0, [R1, GPIO_DEN] 

        //Pull Up Registers
        LDR     R1, =GPIO_PORTF_BASE    // Pull-up Resistor Register
        MOV     R0, #0x11               // Pull-ups on pins 0 and 4
        STR     R0, [R1, GPIO_PUR]      // Store this value into PUR 

        // Read Switches
polling:
        LDR     R1, =GPIO_PORTF_BASE

loop:
        LDR     R2, [R1, GPIO_DATA]   // live pin states
        AND     R2, #0x11             // keep PF4 (SW1) and PF0 (SW2)

        CMP     R2, #0x01
        BEQ     callBlue

        CMP     R2, #0x10
        BEQ     callRed

        CMP     R2, #0x00
        BEQ     callGreen

        CMP     R2, #0x11
        BEQ     ledsOff

        B       loop

ledsOff:
        MOV     R0, #0x00
        STR     R0, [R1, GPIO_DATA]
        B       loop

callRed:
        BL      red
        B       loop

callBlue: 
        BL      blue 
        B       loop

callGreen:
        BL      green
        B       loop

red:
        Push    {LR}

        LDR     R1, =GPIO_PORTF_BASE     // load R1 with PFdata address
        MOV     R0,#0x02                 // set bit 1 for red
        STR     R0, [R1, GPIO_DATA]     // store in data register
        BL      Delay

        POP     {LR}
        BX      LR

blue:
        Push    {LR}
                                                // turn green led on
        LDR     R1, =GPIO_PORTF_BASE    // load R1 with PFdata address
        MOV     R0,#0x04               // set bit 3 for green
        STR     R0, [R1, GPIO_DATA]     // store in data register
        BL      Delay

        POP     {LR}
        BX      LR

green:
        Push    {LR}

        LDR     R1, =GPIO_PORTF_BASE    //load R1 with PFdata address
        MOV     R0,#0x08                // set bits 1 and 2 for red and blue
        STR     R0, [R1, GPIO_DATA]     // store in data register
        BL      Delay

        POP     {LR}
        BX      LR

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
