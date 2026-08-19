BITS 64
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

_init_S:
    cmp rdi, 256
    je _ksa
    mov byte [rax + rdi], rdi
    inc rdi
    jmp _init_S

_ksa:
    mov rdi, 0
    mov rsi, 0

_loop:
    cmp rdi, 256
    je _prga
    mov rsi, rsi + byte [rax + rdi] + byte [key + rdi] % 16
    
    mov bl, byte [rax + rdi]
    mov cl, byte [rax + rsi]
    mov byte [rax + rdi], cl
    mov byte [rax + rsi], bl
    
    inc rdi
    jmp _loop

_prga:

;_swap_values:
;    mov bl, byte [rax + rdi]
;    mov cl, byte [rax + rsi]
;    mov byte [rax + rdi], cl
;    mov byte [rax + rsi], bl

.error:
    ret

mov rax, [ rel o_entry] 
jmp rax

msg: dq "....WOODY....", 10
msg_len : equ $ - msg 
o_entry: db 0