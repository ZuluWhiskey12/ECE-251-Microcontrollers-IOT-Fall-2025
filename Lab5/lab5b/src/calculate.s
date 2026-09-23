///////////////////////////////////////////////////////////////////
// ECE251
// Lab5 part B
//
// Fill in the code and data according to the lab manual
//
///////////////////////////////////////////////////////////////////
        .section .text
        .align 2
        .syntax unified
        .thumb
		.global  	main			// Make available
main:   NOP
        // Addition of two single byte numbers
        LDR R0, =Operand1_1
        LDRB R1, [R0]
        LDR R0, =Operand1_2
        LDRB R2, [R0]
        ADD R1, R2
        LDR R0, =Result1
        STRH R1, [R0]

        // Subtraction of two single byte number
        LDR R0, =Operand2_1
        LDRB R1, [R0]
        LDR R0, =Operand2_2
        LDRB R2, [R0]
        SUBS R1, R2
        LDR R0, =Result2
        STRH R1, [R0]

        // Three byte subtraction
        LDR R0, =Operand3_1
        LDR R1, [R0]
        LDR R0, =Operand3_2
        LDR R2, [R0]
        SUBS R1, R2
        LDR R0, =Result3
        STR R1, [R0]

        // Multibyte addition
        LDR R0, =Operand4_1
        LDR R1, [R0]
        LDR R0, =Operand4_2
        LDR R2, [R0]
        ADDS R1, R2
        LDR R0, =Result4
        STR R1, [R0]

done:   B done

// Place operand data here
            .section .rodata
        Operand1_1: .byte   0xC3
        Operand1_2: .byte   0x8D

        Operand2_1: .byte   0xF5
        Operand2_2: .byte   0x34

        Operand3_1: .word   0xFE6B34
        Operand3_2: .word   0x58CF21

        Operand4_1: .word   0x2E68B3F4
        Operand4_2: .word   0x5C2A

// Storage for results
            .section .bss
        Result1:        .space  4

        Result2:        .space  4

        Result3:        .space  4

        Result4:        .space  4
// ...

        .end