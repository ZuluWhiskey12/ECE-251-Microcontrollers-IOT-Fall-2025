        .section .text
        .align 2
        .syntax unified
        .thumb 
        .global main
main:
        BL      InChar
        CMP     R0, #0x20
        BEQ     done
        BL      OutChar
        B       main
done:   B       done
        
        .end 