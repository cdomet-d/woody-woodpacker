BITS 64
global _stub_start

_stub_start:
mov rax, 1 
mov rdi, 1 
lea rsi, [rel msg] ; "loads" the adress of the label "msg" declared later
mov rdx, msg_len
syscall

mov rax, 60 ; exit
mov rdi, 0
syscall

msg: db "....WOODY....", 10
msg_len : equ $ - msg 