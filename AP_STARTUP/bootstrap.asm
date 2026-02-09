[bits 16]
[org 0x7000]
align   16
realModeBootstrap:
cli
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov ax, StackBottom
add ax, 4096
mov sp, ax
lgdt [gdt_desc] 
mov eax, cr0
or eax, 1
mov cr0, eax
jmp 0x08:protectedMode
StackBottom: times 4096 DB 0x0
gdt:
    dq 0                ;null descriptor
    dq 0x00CF9A000000FFFF ;kernel code 32
    dq 0x00CF92000000FFFF;kernel data 32
    dq 0x00AF9A000000FFFF;kernel code 64
    dq 0x00CF92000000FFFF;kernel data 64
    dq 0 ;tss

gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt

gdt_end:
[bits 32]
align 32
protectedMode:;yayyyyyy no more real mode :3 :3 :3 :3
mov ebx, cr0


;constants to make page table easier
PRESENT         equ 1<<0
WRITABLE        equ 1<<1
PAGESIZE        equ 1<<7 ;sets pagesize to 2 mib
;page Table
align 4096
pml4:
    dq  0;need to fill on runtime
    times 511 dq 0 ;grrr must be one page in size. much wasted space
pdpt:
    dq  0;need to fill on runtime
    times 511 dq 0 ;grrr must be one page in size. much wasted space
pd:    
    dq 0x00000000 | PRESENT | WRITABLE | PAGESIZE   ; 0–2 MiB
    dq 0x00200000 | PRESENT | WRITABLE | PAGESIZE   ; 2–4 MiB
    dq 0x00400000 | PRESENT | WRITABLE | PAGESIZE   ; 4–6 MiB
    dq 0x00600000 | PRESENT | WRITABLE | PAGESIZE   ; 6–8 MiB
    times 508 dq 0
