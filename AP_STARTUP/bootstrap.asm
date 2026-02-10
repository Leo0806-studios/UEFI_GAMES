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
    ;here i need to compute the actual values for the pagemap entries. ill do that before anzthing else so i dont clobber any values


mov ebx, cr0
shr ebx, 31
test ebx, ebx
jz .paginDisabled
mov ebx ,cr0
and ebx, ~(1 << 31)
mov cr0, ebx
jmp .paginDisabled
.paginDisabled:
xor eax , eax
or eax, PRESENT
or eax, 0<<1
or eax, 0<<2
or eax, 1<<3
or eax, 0<<4
or eax, 0<<5
or eax, 0<<6
or eax, 0<<7
or eax, 0<<8
or eax, 0<<9
or eax, 0<<10
or eax, 0<<11
mov ebx, pdpt
shr ebx,12
shl ebx,12
or eax, ebx
mov [pml4+4],eax




 mov edx, cr4
  or  edx, (1 << 5)
  mov cr4, edx


  mov eax, pml4
  mov cr3, eax

    or ebx, (1 << 31) | (1 << 0)
  mov cr0, ebx

  ; Now reload the segment registers (CS, DS, SS, etc.) with the appropriate segment selectors...

  mov ax, 64
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

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