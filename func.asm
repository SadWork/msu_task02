section .text
global f1
f1:
	push ebp
	mov ebp, esp
	fld qword [ebp + 8]
	fld qword[constant2]
	fdivp
	fptan
	fstp st0
	fld qword[constant1]
	fxch st0, st1
	fsubp
	pop ebp
	ret

global f2
f2:
	push ebp
	mov ebp, esp
	fld qword [ebp + 8]
	pop ebp
	ret

global f3
f3:
	push ebp
	mov ebp, esp
	fld qword[constant3]
	fld qword[constant4]
	fmulp
	pop ebp
	ret

section .data
	constant1 dq 2.000000
	constant2 dq 4.000000
	constant3 dq 0.200000
	constant4 dq 3.141593

