extern puts:far,inputDEC:far,outputDEC:far
.model small
.stack 200
.data
str6 db "Wrong Operator$"
str5 db "output condition$"
str4 db "if condition$"
str3 db "Enter operator (+,-,*,/)$"
str2 db "Enter operand 2$"
str1 db "Enter operand 1$"
.code
start:
mov ax,@data
mov ax,0h

mov dx,offset str1
call puts
call inputDEC
mov i,ax
mov dx,offset str2
call puts
call inputDEC
mov j,ax
mov dx,offset str3
call puts
call inputDEC
mov ch,ax


push dx
mov dx,a
cmp dx,11
jle if1_end
mov dx,offset str4
call puts
jmp if1_else_end
if1_end:
mov dx,offset str5
call puts
if1_else_end:
pop dx



push dx
mov dx,ch
switch1_case1:
cmp dx,'+'
jne switch1_case2
mov ax,i
mov bx,j
add ax,bx
mov result,ax
switch1_case2:
cmp dx,'+'
jne switch1_case3
mov bx,j
add ax,bx
mov result,ax
jmp switch1_end

switch1_case3:
mov dx,offset str6
call puts
switch1_end:
pop dx
push cx
push dx
mov dx,1
mov cx,10
