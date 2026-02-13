global MakeHypercall
section .code
[bits 64]
MakeHypercall:
    push r15;
    push r14
    mov r15, rcx    ;move hypercall number to r15
    vmcall;actual hypercall
    mov rax,r14
    pop r14
    pop r15
    ret