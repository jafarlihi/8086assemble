bits 16

mov ax, [bx]
mov dx, [bx]
mov cx, [si]
mov [bx], ax
mov ax, bx
mov word [bx], 0xF0F0
mov ax, 0xF0F0
mov es, [bx]
mov [bx], es
mov ax, es
mov es, ax
