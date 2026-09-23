//********************************************************************
// Lab6b.s
// Programmer:
// Description:
//********************************************************************

// Your code here...
                .equ    DELAY_CLOCKS, 5333333

                .section    .text
                .align      2
                .thumb
                .syntax     unified
                .global     main
main:
                NOP
                MOV     R3, #10

// Your code here....
loop:
                BL      Delay
                SUBS    R3, R3, #1
                BNE     loop

                NOP
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

                .end
