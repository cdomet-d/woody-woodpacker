BITS 64
    global _stub_start

_stub_start:
    mov rax, 1 
    mov rdi, 1 
    lea rsi, [rel msg]
    mov rdx, msg_len + hex_key_len
    syscall
	
    xor rdx, rdx

_get_offset:
    lea r14, [rel _stub_start]
    mov rcx, [rel stub_vaddr]
    sub r14, rcx

_mprotect:
	mov rax, [rel text]
    add rax, r14
    mov rdi, rax 
    and rdi, ~0xFFF
    sub rax, rdi
    mov rsi, rax
    mov r15, [rel text_size]
    add rsi, r15
    add rsi, stub_size + 256
    add rsi, 4095
    and rsi, -4096
    mov rdx, 7
    mov rax, 10
    syscall

_decrypt_text:
    lea rbx, [rel S]
    mov rdi, 0

_init_S:
    cmp rdi, 256
    je _ksa
    mov byte [rbx + rdi], dil   
    inc rdi
    jmp _init_S

_ksa:
    mov rdi, 0
    mov rsi, 0
    lea r12, [rel key]  

_loop_ksa:
    cmp rdi, 256
    je _prga

    mov rax, rdi
    and rax, key_len - 1 
    movzx rax, byte [r12 + rax]     
    add rsi, rax  
    movzx rax, byte [rbx + rdi]     
    add rsi, rax  
    and rsi, 0xFF 

    call _swap_values
    
    inc rdi
    jmp _loop_ksa

_prga:
    mov rdi, 0
    mov rsi, 0
    mov rdx, 0
    lea rax, [rel text]
    mov r13, [rax]    
    add r13, r14

_loop_prga:
    cmp rdx, r15
    je _run_text
    add rdi, 1    
    and rdi, 0xFF 
    movzx r8, byte [rbx + rdi]      
    add rsi, r8   
    and rsi, 0xFF 

    call _swap_values

    mov rax, 0
    movzx r8, byte [rbx + rdi]      
    add rax, r8   
    movzx r8, byte [rbx + rsi]      
    add rax, r8   
    and rax, 0xFF 
    movzx rax, byte [rbx + rax]     
    xor byte [r13 + rdx], al        

    inc rdx
    jmp _loop_prga

_swap_values:
    mov al, byte [rbx + rdi]
    mov cl, byte [rbx + rsi]
    mov byte [rbx + rdi], cl
    mov byte [rbx + rsi], al
    ret

_run_text:
    mov r12, 0
    mov r13, 0

    mov rax, [rel o_entry] 
    add rax, r14
    xor rdx, rdx 
    jmp rax

text: dq 0
text_size: dq 0
key: times 16 db 0x00
key_len: equ 16
msg: db "....WOODY....", 10, "Encryption key: "
msg_len: equ $ - msg 
hex_key: times 33 db 0x00
hex_key_len: equ $ - hex_key 
o_entry: dq 0
stub_vaddr: dq 0
S: times 256 db 0x00
stub_size: equ _stub_end - _stub_start

_stub_end: