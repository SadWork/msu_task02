section .text
global f1
f1:
	push ebp
	mov ebp, esp
	fld qword [ebp + 8]
	fcos
	pop ebp
	ret

global f2
f2:
	push ebp
	mov ebp, esp
	fld qword[constant1]
	pop ebp
	ret

global f3
f3:
	push ebp
	mov ebp, esp
	fld qword[constant2]
	fld qword [ebp + 8]
	fmulp
	pop ebp
	ret

section .data
	constant1 dq 0.000000
	constant2 dq 2.000000

