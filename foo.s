;;;df/dx = h

section .rodata
    a dq -2.0
    b dq 14.0
    c dq 6.0	
section .text
global f1
f1:
    push ebp
    mov ebp, esp
    finit
    fldln2
    fld qword[ebp + 8]  ;st0 = x
    fyl2x
    leave
    ret             ;return ln (x)

global g1
g1:
    push ebp
    mov ebp, esp
    finit
    fld1
    fdiv qword[ebp + 8]     ;return 1 / x
    leave
    ret

global f2
f2:
    push ebp
    mov ebp, esp
    finit
    fld qword[a]
    fmul qword[ebp + 8]
    fadd qword[b]           
    leave           ;return -2 * x + 14
    ret

global g2
g2:
    push ebp
    mov ebp, esp
    finit
    fld qword[a]      ;return -2
    leave
    ret

global f3
f3:
    push ebp
    mov ebp, esp
    finit 
    fld1
    fld qword[a]  
    fabs
    fsub qword[ebp + 8]     ;st0 = 2 - x
    fdivp                  
    fadd qword[c]   ;return 1 / (2 - x) + 6
    leave
    ret     

global g3
g3:
    push ebp
    mov ebp, esp
    finit 
    fld1
    fld qword[a]  
    fabs
    fsub qword[ebp + 8]     ;st0 = 2 - x
    fdivp
    fld ST0
    fmulp       ;st0 = 1 / (2 - x) ^ 2
    leave
    ret
