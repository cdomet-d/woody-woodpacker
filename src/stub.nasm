BITS 64
;[section .text]
    global _stub_start
    extern malloc
    extern free

_stub_start:
    mov rax, 1 
    mov rdi, 1 
    lea rsi, [rel msg]
    mov rdx, msg_len
    syscall

_decrypt_text:
    ;lea rbx, [rel S]
    mov rdi, 256
    call malloc wrt ..plt
    cmp rax, 0
    je _error
    mov rbx, rax
    mov rdi, 0

_init_S:
    cmp rdi, 256
    je _ksa
    mov byte [rbx + rdi], dil   ; S[i] = i
    inc rdi
    jmp _init_S

_ksa:
    mov rdi, 0          ; int i
    mov rsi, 0          ; int j
    lea r12, [rel key]  ; key

_loop_ksa:
    cmp rdi, 256
    je _prga

    mov rax, rdi
    and rax, key_len - 1 ; % 16     ; i % 16
    movzx rax, byte [r12 + rax]     ; key[i % 16]
    add rsi, rax                    ; j + key[i % 16]
    movzx rax, byte [rbx + rdi]     ; S[i]
    add rsi, rax                    ; j + S[i]
    and rsi, 0xFF ; % 256           ; j % 256

    call _swap_values
    
    inc rdi
    jmp _loop_ksa

_prga:
    mov rdi, 0                  ; int i
    mov rsi, 0                  ; int j
    mov rdx, 0                  ; int idx
    mov r9, [rel text_size]     ; text_size
    lea rax, [rel text]
    mov r13, [rax]              ; text

_loop_prga:
    cmp rdx, r9
    je _run_text
    add rdi, 1                      ; i + 1
    and rdi, 0xFF                   ; i % 256
    movzx r8, byte [rbx + rdi]      ; S[i]
    add rsi, r8                     ; j + S[i]
    and rsi, 0xFF                   ; j % 256

    call _swap_values

    add rax, r8                     ; t + S[i]
    movzx r8, byte [rbx + rsi]      ; S[j]
    add rax, r8                     ; t + S[j]
    and rax, 0xFF                   ; t % 256
    movzx rax, byte [rbx + rax]     ; S[t]
    xor byte [r13 + rdx], al        ; text[idx] XOR S[t]

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
    mov rdi, rbx
    call free wrt ..plt
    lea rbx, [rel _stub_start]
    mov rcx, [rel stub_vaddr]
    sub rbx, rcx
    test rbx, rbx
    jns _positive
    neg rbx

_positive:
    mov rax, [rel o_entry] 
    add rax, rbx
    xor rdx, rdx 
    jmp rax

_error:
    mov rax, 60
    mov rdi, 1
    syscall

;[section .data]
    text: dq 0
    text_size: dq 0
    key: times 16 db 0x00
    key_len: equ 16
    msg: db "....WOODY....", 10
    msg_len: equ $ - msg 
    o_entry: dq 0
    stub_vaddr: dq 0

;[section .bss]
;    S: resb 256