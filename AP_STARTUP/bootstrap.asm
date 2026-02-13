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
	jmp CodeProtectedMode:protectedMode
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
	CodeProtectedMode:  equ 08
	DataProtectedMode:  equ 16
	CodeLongMode:       equ 24
	DataLongMode:       equ 32
[bits 32]
align 32
protectedMode:;yayyyyyy no more real mode :3 :3 :3 :3

	mov ax, DataProtectedMode
	mov ds, ax
	mov es, ax
	mov ss, ax




	mov edx, cr4
	or  edx, (1 << 5)
	mov cr4, edx
; Set LME (long mode enable)
	mov ecx, 0xC0000080
	rdmsr
	or  eax, (1 << 8)
	wrmsr

;load pageMap
	mov eax, pml4
	mov cr3, eax

;enalble paging
	mov eax, cr0
	or eax, (1 << 31) | (1 << 0)
	mov cr0, eax

; Now reload the segment registers (CS, DS, SS, etc.) with the appropriate segment selectors...
	mov ax, DataLongMode
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp CodeLongMode:longMode
;constants to make page table easier
PRESENT         equ 1<<0
WRITABLE        equ 1<<1
PAGESIZE        equ 1<<7 ;sets pagesize to 2 mib
;page Table
align 4096
pml4:
	dq  pdpt+ (PRESENT | WRITABLE)
	times 511 dq 0 ;grrr must be one page in size. much wasted space
align 4096
pdpt:
	dq  pd+ (PRESENT | WRITABLE)
	times 511 dq 0 ;grrr must be one page in size. much wasted space
align 4096
pd:    
	dq 0x00000000 | PRESENT | WRITABLE | PAGESIZE   ; 0–2 MiB
	dq 0x00200000 | PRESENT | WRITABLE | PAGESIZE   ; 2–4 MiB
	dq 0x00400000 | PRESENT | WRITABLE | PAGESIZE   ; 4–6 MiB
	dq 0x00600000 | PRESENT | WRITABLE | PAGESIZE   ; 6–8 MiB
	dq 0x00800000 | PRESENT | WRITABLE | PAGESIZE   ; 8-10
	dq 0x00A00000 | PRESENT | WRITABLE | PAGESIZE   ; 10-12
	times 506 dq 0


	[bits 64]
	align 64
	longMode:;finaly long mode
		;todo in longmode
		;   query processor ID
		;   set reached long mode flag 
		;loop untill Continiue flag is set
		;   get stack pointer from array.
		;   update stack pointer
		;   jump to apKernelLoop

;get local core id
	mov eax, 1        
	cpuid             
	shr rbx, 24       
	mov rax, rbx  
	
	DEFAULT ABS
	mov rbx, qword [PtrReachedLongModeArray]
.loopWait:
	test rbx,rbx
	jz .loopWait

	add rbx, rax
	mov [rbx],byte 1 ;set Ready Flag
	.loopWaitContiniue:
	mov rcx, [ContiniueFlag]
	test rcx, rcx
	jz .loopWaitContiniue

	mov rbx, [PtrStackArray] 
	add rbx,rax
	mov rsp, [rbx]
	jmp [ApKernelLoop]
PtrStackArray: 				dq 0x0; will be filled by the kernel once all processors are ready. is a vodi** indexed by the processor id
PtrReachedLongModeArray: 	dq 0x0; will be filled before sipi is send so that processors can signal when they reachedlong mode. is a bool* indexed by the processor id
ContiniueFlag:              db 0x0; will get set by the kernel once all cores have reached long mode
ApKernelLoop:               dq 0x0; gets set by the kerenl on load before the sipi is send


