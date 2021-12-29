	bits	64
	global	__switch_ctx, __jmp_ctx
__switch_ctx:
	pushf
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rsi
	push rdi
	push rdx
	push rcx
	push rbx
	push rax

	mov [rdi], rsp
__jmp_ctx:
	mov rsp, rsi

	pop  rax
	pop  rbx
	pop  rcx
	pop  rdx
	pop  rdi
	pop  rsi
	pop  rbp
	pop  r8
	pop  r9
	pop  r10
	pop  r11
	pop  r12
	pop  r13
	pop  r14
	pop  r15
	popf
	ret
