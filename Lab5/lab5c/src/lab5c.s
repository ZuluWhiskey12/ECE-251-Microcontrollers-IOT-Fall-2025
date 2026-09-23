//***************************************************************
// Program_Directives.s
// Copies the table from one location
// to another memory location.
// Directives and Addressing modes are
// explained with this program.
//***************************************************************

//***************************************************************
// EQU Directives
// These directives do not allocate memory
//***************************************************************
        .equ    OFFSET, 0x780 //Offset to 0x20000C00
        .equ    FIRST,  0x20000480 //Intial memory Location

//***************************************************************
// Directives - This Data Section is part of the code
// It is in the read only section so values cannot be changed.
//***************************************************************
// LABEL      DIRECTIVE   VALUE           COMMENT
        .section .rodata
CTR1:   .byte       0x20 //Table length
MSG:    .ascii      "Copying table..."
        .byte       0x0A
        .byte       0x00

//***************************************************************
// Program section
//***************************************************************
// LABEL      DIRECTIVE   VALUE           COMMENT
        .section .text
        .align      2
        .syntax     unified
        .thumb
        .global     main

main:
start:  MOV     R0, #0
        LDR     R1, =FIRST
        LDR     R2, =CTR1
        LDRB    R2, [R2]

loop1:  STRB    R0, [R1]
        ADD     R0, R0, #2 //Even Increment Value
        ADD     R1, R1, #1
        SUBS    R2, R2, #1
        BNE     loop1

        LDR     R0, =MSG
        BL      OutStr

        LDR     R1, =0x2000049f
        LDR     R3, [R1]

        LDR     R1, =FIRST
        MOV     R2, #0x20 //Table Length

loop2:  LDRB    R0, [R1]
        STRB    R3, [R1, #OFFSET]
        SUB     R3, R3, #2
        ADD     R1, R1, #1
        SUBS    R2, R2, #1
        BNE     loop2

done:   B       done

//***************************************************************
// End of the program section
//***************************************************************
// LABEL      DIRECTIVE   VALUE           COMMENT
        .section .bss
        .space      4096

        .end
