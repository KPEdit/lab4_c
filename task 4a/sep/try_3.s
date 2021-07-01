	.file	"try_3.c"
	.text
	.globl	_nullTable
	.def	_nullTable;	.scl	2;	.type	32;	.endef
_nullTable:
LFB13:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	L2
L5:
	movl	$0, -8(%ebp)
	jmp	L3
L4:
	movl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	imull	$100, %edx, %ecx
	movl	-4(%ebp), %edx
	addl	%ecx, %edx
	movl	$0, (%eax,%edx,4)
	addl	$1, -8(%ebp)
L3:
	cmpl	$1, -8(%ebp)
	jle	L4
	addl	$1, -4(%ebp)
L2:
	cmpl	$99, -4(%ebp)
	jle	L5
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE13:
	.def	___main;	.scl	2;	.type	32;	.endef
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$816, %esp
	call	___main
	leal	16(%esp), %eax
	movl	%eax, (%esp)
	call	_nullTable
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE14:
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
