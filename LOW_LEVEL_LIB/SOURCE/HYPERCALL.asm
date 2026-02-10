global MakeHypercall
section .code
[bits 64]
MakeHypercall:
    push r15;
    push r14
    mov r15, rdi    ;move hypercall number to r15

    VMCALL;actual hypercall
    mov rcx,r13
    pop r14
    pop r15
    ret