[bits 64]
extern MakeSyscall

MakeSyscall:
    push r15
    push r14
    mov r15, rdi    ;move hypercall number to r15
    syscall
    mov rcx,r14
    pop r14
    pop r15
    mov 