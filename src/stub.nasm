BITS 64
global _stub_start

_stub_start:
mov rax, 1 
mov rdi, 1 
lea rsi, [rel msg]
mov rdx, msg_len
syscall

mov rax, [ rel o_entry] 
jmp rax

msg: db "....WOODY....", 10
msg_len : equ $ - msg 
o_entry: dq 0