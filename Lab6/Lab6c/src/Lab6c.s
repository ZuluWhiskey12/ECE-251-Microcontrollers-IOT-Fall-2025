//********************************************************************
// Lab6c.s
// Programmer:
// Description:
//********************************************************************

// Your code here...
                .equ    DELAY_CLOCKS, 5333333

                .data
                .align      2
currentFloor:   .word       1

                .section    .rodata
OpenMSG:        .asciz       "Door Open\n"
CloseMSG:       .asciz       "Door Close\n"
UpMSG:          .asciz       "Going UP\n"
DownMSG:        .asciz       "Going DOWN\n"
StayMSG:        .asciz       "Same Floor\n"

                .section    .text
                .align      2
                .thumb
                .syntax     unified
                .global     main
main:
                NOP
                LDR     R1, =currentFloor
                MOVS    R0, #1
                STR     R0, [R1]         // set current floor to 1

MainLoop:
                BL      OpenDoor

                BL      InChar_Echo
                SUBS    R0, R0, #0x30
                MOV     R4, R0

                BL      CloseDoor

                LDR     R1, =currentFloor
                LDR     R2, [R1]        
                CMP     R4, R2
                BEQ     SameFloor
                BGT     GoingUp

                MOV     R0, R4
                BL      Down
                B       MainLoop

GoingUp:    
                MOV     R0, R4
                BL      Up
                B       MainLoop

SameFloor:      
                BL      Stay
                B       MainLoop
                
forever:        B       forever

//******************************************
// Subroutine to create delay, 
// DELAY_CLOCKS is the counter which is 
// decremented to zero
//******************************************	
Delay:          LDR     R2,=DELAY_CLOCKS // set delay count

del:            SUBS    R2, R2, #1	     // decrement count
	            BNE     del		         // if not at zero, do again
	            BX      LR		         // return when done

// Your code here....
OpenDoor:
                PUSH    {LR}
                BL      Delay

                LDR     R0, =OpenMSG
                BL      OutStr

                POP     {LR}
                BX     LR

CloseDoor:
                PUSH    {LR}
                BL      Delay

                LDR     R0, =CloseMSG
                BL      OutStr

                POP     {LR}
                BX      LR

Up:
                PUSH    {R4, R5, R6, LR}
                
                LDR     R4, =currentFloor
                LDR     R5, [R4]
                MOV     R6, R0

                LDR     R0, =UpMSG
                BL      OutStr

UpLoop:     
                ADDS    R5, R5, #1
                BL      Delay

                MOV     R0, R5
                BL      OutHex

                MOV     R0, #0x20 //0x20 is ascii for space
                BL      OutChar

                CMP     R5, R6
                BNE     UpLoop

                STR     R5, [R4]
                POP     {R4, R5, R6, LR}
                BX      LR

Down:
                PUSH    {R4, R5, R6, LR}
                
                LDR     R4, =currentFloor
                LDR     R5, [R4]
                MOV     R6, R0

                LDR     R0, =DownMSG
                BL      OutStr

DownLoop: 
                SUBS    R5, R5, #1
                BL      Delay

                MOV     R0, R5
                BL      OutHex

                MOV     R0, #0x20 //0x20 is ascii for space
                BL      OutChar

                CMP     R5, R6
                BNE     DownLoop

                STR     R5, [R4]

                POP     {R4, R5, R6, LR}
                BX      LR

Stay:
                PUSH    {LR}
                BL      Delay

                LDR     R0, =StayMSG
                BL      OutStr

                POP     {LR}
                BX      LR

                .end
