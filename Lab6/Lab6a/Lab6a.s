//********************************************************************
// Lab6a.s
// Programmer:
// Description:
//********************************************************************

                .equ        TABLE_SIZE, 10

                .section    .bss
TABLE_ABS:      .space      TABLE_SIZE

TABLE_SQ:       .space      TABLE_SIZE * 2

                .section    .rodata
TABLE_CONST:    .byte       10,45,63,-12,43,-56,32,45,-98,1

                .section    .text
                .align      2
                .thumb
                .syntax     unified
                .global     main
main:
                NOP
                MOV         R0, #TABLE_SIZE
                LDR         R1, =TABLE_CONST
                LDR         R2, =TABLE_ABS
                LDR         R3, =TABLE_SQ

// Your code goes here....
loop:               CBZ         R0, forever
                    //ABS
                    LDRSB       R4, [R1], #1
                    CMP         R4, #0
                    IT          MI
                    RSBMI       R4, R4, #0
                    STRB        R4, [R2], #1

                    //SQ
                    MUL         R5, R4, R4
                    STRH        R5, [R3], #2

                    SUBS        R0, R0, #1
                    BNE          loop

forever:        B           forever
                .end