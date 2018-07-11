.model small
.stack 100h

.data
x22 dw ?
a22 dw 0 dup(?)
a22 dw 0 dup(?)
a22 dw 0 dup(?)
a22 dw 0 dup(?)
a22 dw 0 dup(?)
a22 dw 0 dup(?)

.code 
main PROC NEAR

	mov bx, 1
	add bx, bx
	mov ax, 3
	mov  a2[bx], ax
	mov bx, 2
	add bx, bx
	mov ax, 12
	mov  a2[bx], ax
	mov bx, 1
	add bx, bx
	mov ax, a2[bx]
	mov t0, ax
	mov bx, 2
	add bx, bx
	mov ax, a2[bx]
	mov t1, ax
	mov ax, t0
	add ax, t1
	mov t2 , ax
	mov bx, 3
	add bx, bx
	mov ax, t2
	mov  a2[bx], ax
	mov bx, 3
	add bx, bx
	mov ax, a2[bx]
	mov t0, ax
	mov ax, t0
	mov x2, ax
	mov ax, x
	call DECIMAL_OUT
	mov dx,0
	jmp   

	mov ah,4ch
	int 21h

main ENDP



DECIMAL_OUT PROC NEAR

	push ax
	push bx
	push cx
	push dx
	or ax,ax
 	jge enddif
	push ax
	mov dl,'-'
	mov ah,2
	int 21h
	pop ax
	neg ax
enddif:
	xor cx,cx
	mov bx,10d
repeat:
	xor dx,dx
	div bx
	 push dx
	inc cx
	or ax,ax
	jne repeat
	mov ah,2
print_loop:
	pop dx
	or dl,30h
	int 21h
	loop print_loop
	pop dx
	pop cx
	pop bx
	pop ax
	ret

DECIMAL_OUT ENDP
