.code

; void load_gdt(void* gdtr);
load_gdt proc
    ; RCX holds the first parameter (gdtr)
    lgdt fword ptr [rcx]   ; Load 6-byte GDT descriptor from [RCX]
    ret
load_gdt endp

end
