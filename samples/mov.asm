bits 16

mov ax, [bx]
mov [ax], bx
mov ax, bx
mov [ax], 0xF0F0
mov ax, 0xF0F0
mov es, [ax]
mov [ax], es
mov ax, es
mov es, ax
