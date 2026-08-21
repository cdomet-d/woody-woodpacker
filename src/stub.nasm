BITS 64
;[section .text]
    global _stub_start

_stub_start:
    mov rax, 1 
    mov rdi, 1 
    lea rsi, [rel msg]
    mov rdx, msg_len
    syscall
    xor rdx, rdx


_get_offset:
    lea r14, [rel _stub_start]
    mov rcx, [rel stub_vaddr]
    sub r14, rcx
    test r14, r14
    jns _decrypt_text
    neg r14

_mprotect:
    mov r15, [rel text_size]     ; text_size
    mov rax, [rel o_entry]
    add rax, r14
    and rax, ~0xFFF        
    mov rdi, rax 

    mov rsi, r15
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
    lea rax, [rel text]
    mov r13, [rax]              ; text
    add r13, r14

_loop_prga:
    cmp rdx, r15
    je _run_text
    add rdi, 1                      ; i + 1
    and rdi, 0xFF                   ; i % 256
    movzx r8, byte [rbx + rdi]      ; S[i]
    add rsi, r8                     ; j + S[i]
    and rsi, 0xFF                   ; j % 256

    call _swap_values

    mov rax, 0
    movzx r8, byte [rbx + rdi]      ; S[i]
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

    mov rax, [rel o_entry] 
    add rax, r14
    xor rdx, rdx 
    jmp rax

_stub_end:

text: dq 0
text_size: dq 0
key: times 16 db 0x00
key_len: equ 16
msg: db "....WOODY....", 10
msg_len: equ $ - msg 
o_entry: dq 0
stub_vaddr: dq 0
S: times 256 db 0x00
stub_size: equ _stub_end - _stub_start