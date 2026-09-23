/*
* Write a program to count upper case letters in a string created from user input
*   1. Define string to that will be checked
*   2. Load that string to a register
*   3. Count uppercase letter in the Array
*   4. Print out the count
*/
        .equ
        .section .data

        .section .rodata
inStr:  .asciz      "Go CSU Rams" //Create string to be checked in read only data
outStr: .asciz      "The count is: "

        .section    .text
        .align 2
        .syntax unified
        .thumb
        .global main

main:
        nop
        nop
        //Start of my program
        MOV         R2, #0          //Create count in R2 
        LDR         R0, = inStr     //read in the string we created in read only data, which we will check for Upper case letters

loop:   
        LDR        R1, [R0], #1     //load 
        CBZ        R1, #0 Done      //Branch to done if R1 is equal to zero

        CMP        R1, #65          //CMP value in R1 to ASCII A (Can use decimal or hex value of ASCII Char)
        BLT        loop             //If less then ASCII A loop again

        CMP        R1, #90          //Compare value in R1 to ASCII Z 
        BGT        loop             //If great then ASCII Z loop again

        ADD        R2, #1           //Increment the count by one if the value got through both compares
        B          loop             //Loop after incrementing the counter

Done:   
        LDR        R0, =outStr      //load output message into R0
        BL         OutStr           //Print out message using subroutine OutStr

        MOV        R1, #10          //Put 10 into R1
        UDIV       R3, R2, R1       //Put a number divided by 10 into R3
        MUL        R4, R3, R1       //Put the mulitplication of number divided by 10 from R3 and R1 which is 10 into R4
        SUB        R0, R2, R4       
        BL         OutHex           //Print outs least signifacant digit


        MOV        R2, R3           //
        CBZ        R2, Done2        //Compare value in counter R2 with zero if zero branch to Done2 subroutine
        B          Loop2

Done2:
        B          Done2



        



