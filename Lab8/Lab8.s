//***************************************************************
// Lab8 template
// Modified by Steve Undy
// Date 11-01-23
// 
// Student Version Adapted from version from Sanja Manic
// Date 10-19-18
// Lab #8 - Maskable interrupts 
// Uses Monitor and PortF LED (RG&B)
//***************************************************************

//*************************************************************** 
// EQU Directives
//*************************************************************** 

// Interupt symbols
        .equ    NVIC_ST_CTRL,		0xE000E010
        .equ    NVIC_ST_RELOAD,  	0xE000E014
        .equ    NVIC_ST_CURRENT,	0xE000E018
        .equ    SHP_SYSPRI3,	    0xE000ED20 	
        .equ    RELOAD_VALUE,		3999999

// Use built-in LED on Port F. Not all may be needed
        .equ    GPIO_PORTF_BASE,        0x40025000
        .equ    GPIO_DATA,  	        0x3FC
        .equ    GPIO_DIR,   	        0x400
        .equ    GPIO_AFSEL, 	        0x420
        .equ    GPIO_PUR,   	        0x510
        .equ    GPIO_DEN,   	        0x51C
        .equ    GPIO_AMSEL, 	        0x528
        .equ    GPIO_PCTL,  	        0x52C

        .equ    SYSCTL_RCGCGPIO_R,  	0x400FE608
        .equ    PCTLCNST,           	0x0000FFF0

//***************************************************************
// Read only Data Section
//***************************************************************
                .section .rodata
// messgaes
Hello:          .ascii  "Hello from Lab8!\n\0"
MSGTime:		.ascii  "\nSet the Clock Time (hh:mm:ss)\0"
MSGAlarm:       .ascii  "\nSet the Alarm Time (hh:mm:ss)\0"
entrT1:			.ascii	"\nEnter hours (hh): \0"
entrT2:			.ascii	"\nEnter minutes (mm): \0"
entrT3:			.ascii	"\nEnter seconds (ss): \0"
entrT4:			.ascii	"\nEnter A (for AM) or P (for PM): \0"
CR:             .ascii  "\n\0"
BadMsg:			.asciz	"Invalid entry, try again."

//***************************************************************
// Data Section in READWRITE
// Values of the data in this section are initialazed at reset 
//***************************************************************
                .section .data

// memory where I store if LED should be on or off 
// this also controls interrupts, while LED is off, interrupts are on                
LedIsOn:		.byte   0x00

// memory for time and alarm. It gets updated each second
time:	        .ascii  "00:00:00 AM\n\0"

// memory where we save alarm time
alarm:	        .ascii   "00:00:00 AM\n\0"

//***************************************************************
// Program section					      
//***************************************************************
//LABEL		DIRECTIVE	VALUE			COMMENT
        .section .text
        .align 	2
        .syntax unified
        .thumb
		.global main			// Make available
main:
        nop
        nop

        // init UART and print out welcome message
		BL      	InitUART

        LDR     	R0,=Hello
        BL      	OutStr
        
		// main code goes here:
		// - Call Port F Initialization subroutine. 
		BL			_Init_PortF
		// - Calls subroutine setme prompts for inputting time and alarm data: 
		LDR			R0, =time
		LDR			R1, =MSGTime
		BL			setme

		LDR			R0, =alarm
		LDR			R1, =MSGAlarm
		BL			setme

		// - Initialize variable LedIsOn to 0.
		LDR			R0, =LedIsOn
		MOV			R1, #0
		STRB		R1, [R0]

		// - Call subroutine to initialize interrupts.
		BL			systick_ini
		// - Turn on interrupts.
		BL			sys_tick_handler

wait:
		WFI
		// - Wait for LedIsOn to be set; then turn on LED (RG&B).
		LDR			R1,=LedIsOn		// pointer to alarm flag
		LDRB		R0,[R1]			// read flag value

		CMP			R0, #1
		BEQ    		turnOnLED
		B 			wait

turnOnLED:
		LDR     	R1, =GPIO_PORTF_BASE     
        MOV     	R0,#0x0E                 
        STR     	R0, [R1, GPIO_DATA] 

		// - Disable interrupts:
		CPSID		I
		//   Turn off Led when any key is entered (monitor).
		BL			InChar_Echo

        MOV     	R0, #0x00
        STR     	R0, [R1, GPIO_DATA]

		// About 35 lines of assembly code
		// Deadloop ends program.
done:   
		BL      	done	

//*********************************************************
// systick_ini subroutine
//*********************************************************
systick_ini:
		LDR 		R1, =NVIC_ST_CTRL
		MOV 		R0, #0
		STR 		R0, [R1]

		LDR 		R1, =NVIC_ST_RELOAD
		LDR 		R0, =RELOAD_VALUE
		STR 		R0, [R1]

		LDR 		R1, =NVIC_ST_CURRENT
		MOV 		R0, #0
		STR			R0, [R1]

		LDR 		R1, =SHP_SYSPRI3
		MOV 		R0, #0x40000000
		STR 		R0, [R1]

		LDR 		R1, =NVIC_ST_CTRL
		MOV 		R0, #0x03
		STR 		R0, [R1]

		CPSIE 		I
		BX 			LR

//*********************************************************
// setme subroutine
//*********************************************************
setme:
		// Input: R0 is value of the address to store current time or alarm
		//        R1 points to prompt string
		// 
		// this subroutine reads in and saves 6 digits of current time or alarm in
		// the following format "hh:ss:mm AM" and adding new line and end of transmission
		// at the end of string for displaying convenience
		//
		// NOTE:  This code does NOT check for valid data entry. That code is to be added.
		PUSH		{R4, R5, R6, R7, LR}

        MOV     	R4, R0      // save parameter
		MOV			R5, R0
        MOV			R0, R1
		BL			OutStr		// displays message asking for current time or alarm time

setmeLoop:
		MOV			R4, R5

		LDR			R0,=entrT1	// Message that asks for 2 digits of the hour
		BL			OutStr				
		BL			InChar_Echo	// gets tens hour digit
		STRB		R0,[R4],#1	// stores it and increments address
		BL			InChar_Echo	// gets units digit
		STRB		R0,[R4],#1	// stores it and increments address

		LDR			R0,=':'		// stores : sign between hours and minutes
		STRB		R0,[R4],#1

		LDR			R0,=entrT2	// Message that asks for 2 digits of the minutes
		BL			OutStr
		BL			InChar_Echo	// Gets tens minutes digit
		STRB		R0,[R4],#1
		BL			InChar_Echo	// Gets units minutes digit
		STRB		R0,[R4],#1

		LDR			R0 ,=':'	// Stores ":" between minutes and seconds
		STRB		R0,[R4],#1

		LDR			R0,=entrT3	// Message that asks for 2 digits of the seconds
		BL			OutStr
		BL			InChar_Echo	// Gets tens seconds digit
		STRB		R0,[R4],#1
		BL			InChar_Echo	// Gets units seconds digit
		STRB		R0,[R4],#1

		LDR			R0 ,=' '	// Stores " " after seconds
		STRB		R0,[R4],#1

		LDR			R0,=entrT4	// Message that asks for AM or PM (A or P)
		BL			OutStr
        BL			InChar_Echo     // Gets 'A' or 'P'
		STRB		R0,[R4],#1	// Stores that A or P

        LDR    	 	R0,=CR
		BL     		OutStr 		// 'M', newline and NULL are already initialized in line. No need to store

		MOV			R0, R5
		BL			checkTime

		CMP			R0, #0
		BEQ			setmeDone

		LDR			R0, =BadMsg
		BL			OutStr
		B			setmeLoop

setmeDone:
		POP			{R4, R5, R6, R7, LR}	
		BX			LR

checkTime:
        PUSH    {R1-R3, LR}
        MOV     R1, R0          

        //Check hours
        LDRB    R2, [R1]        
        LDRB    R3, [R1, #1]    

        // Both must be ASCII digits 
        CMP     R2, #'0'
        BLT     invalidTime
        CMP     R2, #'9'
        BGT     invalidTime

        CMP     R3, #'0'
        BLT     invalidTime
        CMP     R3, #'9'
        BGT     invalidTime

        // Convert to num
        SUB     R2, #'0'
        SUB     R3, #'0'
        MOV     R0, #10
        MUL     R2, R0
        ADD     R2, R3     

        CMP     R2, #1
        BLT     invalidTime    
        CMP     R2, #12
        BGT     invalidTime   

        //Check minutes
        LDRB    R2, [R1, #3]   
        LDRB    R3, [R1, #4]    

        CMP     R2, #'0'
        BLT     invalidTime
        CMP     R2, #'9'
        BGT     invalidTime

        CMP     R3, #'0'
        BLT     invalidTime
        CMP     R3, #'9'
        BGT     invalidTime

        SUB     R2, #'0'
        SUB     R3, #'0'
        MOV     R0, #10
        MUL     R2, R0
        ADD     R2, R3     

        CMP     R2, #59
        BGT     invalidTime    // allow 0 to 59

        //Check seconds
        LDRB    R2, [R1, #6]   
        LDRB    R3, [R1, #7]   

        CMP     R2, #'0'
        BLT     invalidTime
        CMP     R2, #'9'
        BGT     invalidTime

        CMP     R3, #'0'
        BLT     invalidTime
        CMP     R3, #'9'
        BGT     invalidTime

        SUB     R2, #'0'
        SUB     R3, #'0'
        MOV     R0, #10
        MUL     R2, R0
        ADD     R2, R3    

        CMP     R2, #59
        BGT     invalidTime    // allow 0 to 59

        //Check A/P 
        LDRB    R2, [R1, #9]
        BIC     R2, #0x20

        CMP     R2, #'A'
        BEQ     validTime
        CMP     R2, #'P'
        BEQ     validTime

        B       invalidTime

validTime:
        MOV     R0, #0          // success
        POP     {R1-R3, LR}
        BX      LR

invalidTime:
        MOV     R0, #1          // failure
        POP     {R1-R3, LR}
        BX      LR

//*********************************************************
// clock Subroutine
// no inputs or outputs
// updates clock by 1 second (handles rollovers)
//*********************************************************
clock:
        PUSH    	{R4,R5}

		LDR			R5,=time		// Puts in R5 address of the variable time
		ADD			R5,R5,#7		// Adds 7. R5 contains address of the units digit of seconds number
		
		LDRB		R4,[R5]
		ADD			R4,R4,#1		// Increments the units digit of seconds
		CMP			R4,#0x3A		// if it is equal to ascii character 0x3A (would display A = 10), there should be rollover
		BEQ			rosec			// if larger than 9s, rollover second
		STRB		R4,[R5]			// if there is no rollover, save updated lower digit
		B			doneCl

rosec:  	
		MOV			R4,#0x30		// if there was rollover, save "0"
		STRB		R4,[R5],#-1		// and point to the tens digit of seconds number
		LDRB		R4,[R5]			// Get tens digit
		ADD			R4,R4,#1		// Increment tens digit
		CMP			R4,#0x36		// Compare with "6"
		BEQ			rosec2			// If "6", rollover
		STRB		R4,[R5]			// If not, save and done
		B			doneCl

rosec2:	        
		MOV			R4,#0x30		// Tens digit rollover. Save "0"
		STRB		R4,[R5],#-2		// skip over tens digit and ':' sign
		LDRB		R4,[R5]			// Get units digit of minutes
		ADD			R4,R4,#1		// Increment minutes units digit
		CMP			R4,#0x3A		// Compare with "9"+1
		BEQ			romin			// if larger than 9, rollover
		STRB		R4,[R5]			// If not, save and done
		B			doneCl

romin:	
        MOV			R4,#0x30		// Minutes rollover. Set to "0"
		STRB		R4,[R5],#-1		// and move to minutes tens digit
		LDRB		R4,[R5]			// Get minutes tens digit
		ADD			R4,R4,#1		// Increment tens digit	
		CMP			R4,#0x36		// Check for "6"
		BEQ			romin2			// if so, rollover
		STRB		R4,[R5]			// if not, save and done
		B			doneCl

romin2:	       
		MOV			R4,#0x30		// Tens of minutes rollover. Set to 0
		STRB		R4,[R5],#-2		// skip this digit and  ':' sign
		LDRB		R4,[R5],#-1		// Get hours units digit and move to hours tens digit
		ADD			R4,R4,#1		// Increment hours units digit			
		LDRB		R3,[R5]			// Get hours tens digit
		CMP			R3,#0x30		// compare this tens digit to "0"
		BNE			hour1x			// If not "0" (ie if "1") then check for hours overflow
		
		CMP			R4,#0x3A		// Tens is "0", so check if units is now "9" +1
		BEQ			hour10			// Units overflow, so set go to set to "10"
		STRB		R4,[R5,#1]		// no units overflow, so point to units and store that digit
		B			doneCl

hour10:         
		MOV			R3,#0x31		// put 10 hours
		STRB		R3,[R5],#1		// store "1" (tens digit) and increment pointer
		MOV			R4,#0x30
		STRB		R4,[R5]			// store "0" (units digit)
		B			doneCl

hour1x: 	
		CMP			R4,#0x33		// is hour "13"?
		BEQ			swAll			// If so, go to change to "01"
		
		ADD			R5,R5,#1		// Hour not "13"
		STRB		R4,[R5]			// So store hour units digit
		CMP			R4, #0x32		// Check if 12:00:00
		BEQ			swAMPM			// If so, go swap AM and PM
		B			doneCl			// If not, done

swAll:	        
		MOV			R3,#0x30		// Put "0" in hours tens digit
		STRB		R3,[R5],#1
		MOV			R4,#0x31		// Put "1" in hours units digit
		STRB		R4,[R5]	
		B			doneCl

swAMPM:	        
		LDR			R5,=time
		ADD			R5,R5,#0x09 		// pointing to A/P character
		LDRB		R4,[R5]
		RSB			R4,R4,#0x91		// Turn A to P or P to A 
		STRB		R4,[R5]			// and save it

doneCl:
        POP     	{R4,R5}
		BX			LR                      // return

//*********************************************************
// almon subroutine
// checks if current time and alarm time are the same
//*********************************************************	
almon:
		LDR			R2, =time		// puts in R2 address of current time
		LDR			R3, =alarm		// puts in R3 address of alarm time

loop: 
		LDRB		R4, [R2], #1
		LDRB		R5, [R3], #1

		CBZ			R4, skip

		CMP			R4, R5
		BEQ			loop

		BX			LR	
skip:	
		LDR			R6, =LedIsOn
		MOV			R7, #1
		STRB		R7, [R6]

		BX			LR

//*********************************************************
// cdisp subroutine
// subroutine to display current time in montior
//*********************************************************
cdisp:
		PUSH   		{LR}

		LDR			R0, =time
		BL			OutStr

		POP			{LR}
		BX			LR
//*********************************************************
// _Init_PortF subroutine
//*********************************************************
// Make Port F 1-3 outputs, enable digital I/O, ensure alt. functions off.
// Input: none  Output: none   Modifies: R0, R1
// 32 lines of assembly code available in several examples using built-in LEDs

_Init_PortF:
		// Code to initialize PortF goes here
		// 1) activate clock for Port F
		LDR     	R1, =SYSCTL_RCGCGPIO_R
        LDR     	R0, [R1]                
        ORR     	R0, #0x20
        STR     	R0, [R1] 

		nop
		nop
		//Set R1 to Port F base address
		LDR     R1, =GPIO_PORTF_BASE

		// 3) disable analog functionality
		LDR     	R0, [R1, GPIO_AMSEL]
        BIC     	R0, #0xFF
		STR     	R0, [R1, GPIO_AMSEL] 

		// 4) configure as GPIO
        LDR     	R0, [R1, GPIO_AFSEL]    
        BIC     	R0, #0xFF
        STR     	R0, [R1, GPIO_AFSEL]

		// 5) set direction register
        LDR     	R0, [R1, GPIO_DIR]
        BIC     	R0, #0xFF
        ORR     	R0, #0x0E             
        STR     	R0, [R1, GPIO_DIR]

		// 6) regular port function

		// 7) enable Port F digital port
		LDR     	R0, [R1, GPIO_DEN]
        ORR     	R0, #0x0E
        STR     	R0, [R1, GPIO_DEN] 
		
		// Turn off LED (RG&B)
		LDR     	R0, [R1, GPIO_DATA]
        MOV     	R0, #0x00
        STR     	R0, [R1, GPIO_DATA]

		BX			LR

//*********************************************************
// SysTick ISR
//*********************************************************
// Interrupt Service routine
// This gets called every 1 s (determined by interrupt setup)
// calls clock,almon and cdisp
		.global sys_tick_handler
        .thumb_func
sys_tick_handler:
		PUSH		{LR}

		BL			clock
		BL			almon
		BL			cdisp

		POP			{LR}
		BX			LR

//***************************************************************
// End of the program  section
//***************************************************************
		.end
