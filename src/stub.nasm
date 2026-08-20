BITS 64
[section .text]
    global _stub_start
    extern malloc

_stub_start:
    mov rax, 1 
    mov rdi, 1 
    lea rsi, [rel msg] ; "loads" the adress of the label "msg" declared later
    mov rdx, msg_len
    syscall

_decrypt_text:
    mov rdi, 256
    call malloc wrt ..plt
    cmp rax, 0
    je .error
    mov rdi, 0
    call _init_S

_ksa:
    mov rdi, 0
    mov rsi, 0

_loop_ksa:
    cmp rdi, 256
    je _prga

    lea rdx, [rel key]
    mov rcx, rdi
    and rcx, 0xF ; % 16
    mov r8b, byte [key + rcx]
    mov r9b, byte [rax + rdi]
    add rsi, r8b
    add rsi, r9b
    and rsi, 0xFF ; % 256

    call _swap_values
    
    inc rdi
    jmp _loop_ksa

_prga:
    mov rdi, 0
    mov rsi, 0
    mov rdx, 0
    lea rcx, [rel text_size]
    mov r10, [rcx]
    mov rcx, 0
    lea r11, [rel text]

_loop_prga:
    cmp rcx, r10
    je _run_text
    add rdi, 1
    and rdi, 0xFF
    mov r8b, byte [rax + rdi]
    add rsi, r8b
    and rsi, 0xFF

    call swap_values

    add rdx, r8b
    mov r9b, byte [rax + rsi]
    add rdx, r9b
    and rdx, 0xFF
    mov r8b, byte [rax + rdx]
    and byte [r11 + rcx], r8b

    inc rcx
    jmp _loop_prga

_run_text:
    mov rax, [ rel o_entry] 
    jmp rax


;utils

_swap_values:
    mov bl, byte [rax + rdi]
    mov cl, byte [rax + rsi]
    mov byte [rax + rdi], cl
    mov byte [rax + rsi], bl

_init_S:
    cmp rdi, 256
    je _ksa
    mov byte [rax + rdi], rdi
    inc rdi
    jmp _init_S

.error:
    ret

[section .data]
    text: dq 0
    text_size: dq 0
    key: times 16 db 0x00
    key_len: equ 16
    msg: dq "....WOODY....", 10
    msg_len: equ $ - msg 
    o_entry: db 0