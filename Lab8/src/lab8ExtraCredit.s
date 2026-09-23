            .global SysTick_Init
            .global OutStr

    .equ    RELOAD_VALUE, 3999999           //Changes to a value to represent a 1 sec delay at 4
    .equ    NVIC_ST_CTRL, 0xE000E010
    .equ    NVIC_ST_RELOAD, 0xE000E014
    .equ    NVIC_ST_CURRENT, 0xE000E018
    .equ    SHP_SYSPRI3, 0xE000ED20

//*********************************************************
// ROM area
//*********************************************************
            .section .rodata
starttxt:   .ascii "Start\n\0"
stoptxt:    .ascii "Stop\n\0"
delsec:     .word 10

//*********************************************************
//Program area
//*********************************************************
            .section .text
            .syntax unified
            .thumb
            .global main
main:
            BL      InitUART
            LDR     R1,=NVIC_ST_CTRL
            MOV     R0,#0
            STR     R0,[R1]

            LDR     R1,=NVIC_ST_RELOAD
            LDR     R0,=RELOAD_VALUE       // load reload val into R0
            STR     R0,[R1]

            LDR     R1,=NVIC_ST_CURRENT
            MOV     R0,#0
            STR     R0,[R1]

            LDR     R1,=SHP_SYSPRI3
            MOV     R0,#0x40000000
            STR     R0,[R1]

            LDR     R1,=NVIC_ST_CTRL
            MOV     R0,#0x03                //Changed from 0x01 to 0x03
            STR     R0,[R1]

            LDR     R0,=starttxt
            BL      OutStr

            LDR     R1,=delsec
            LDR     R2,=count
            LDR     R0,[R1]
            STR     R0,[R2]

            CPSIE   I

check:
            LDR     R1,=count
            LDR     R0,[R1]
            CMP     R0,#0
            BNE     check

            LDR     R0,=stoptxt
            BL      OutStr

loop:
            B       loop

//*********************************************************
// SysTick ISR
//*********************************************************
            .global sys_tick_handler
            .thumb_func
sys_tick_handler:
            LDR     R1,=count
            LDR     R0,[R1]
            SUBS    R0,R0,#1
            STR     R0,[R1]
            BX      LR

//*********************************************************
// RAM area
//*********************************************************
            .section .data

count: .space 1

.end
