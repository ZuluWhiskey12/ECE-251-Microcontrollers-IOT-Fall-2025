    .section .text
    .align 2
    .syntax unified
    .thumb
    .global main

main:
    MOV     R0, #0x42        @ Initialize R0
    MOV     R1, #0x55        @ Initialize R1
    PUSH    {R0}             @ Save R0 on stack
    PUSH    {R1}             @ Save R1 on stack
    BL      subnop           @ Call subroutine
    POP     {R1}             @ Restore R1
    POP     {R0}             @ Restore R0
done:
    B       done             @ Infinite loop (end of program)

subnop:
    NOP                      @ Does nothing
    NOP
    BX      LR               @ Return to main program
    .end
