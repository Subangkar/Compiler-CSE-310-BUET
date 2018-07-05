mov ax, 1
mov bx, 2
mul bx
mov t0, ax
mov bx, t0
add bx, bx
mov ax, a[bx]
mov t1, ax
mov ax, i
mov bx, t1
mul bx
mov t2, ax
mov bx, 1
add bx, bx
mov ax, t2
mov  a[bx], ax
mov ax, i
cmp ax, 2
jl L0
mov t3, 0
jmp L1
L0:
mov t3, 1
L1:
mov ax, t3
cmp ax, 0
je L2
mov bx, 2
add bx, bx
mov ax, 5
mov  a[bx], ax
L2:
