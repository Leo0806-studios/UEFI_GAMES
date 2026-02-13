[bits 64]
extern MakeSyscall

MakeSyscall:
    push r15
    push r14
    mov r15, rcx    ;move syscall number to r15
    syscall         ;actual syscall
    mov rax ,r14
    pop r14
    pop r15
    ret