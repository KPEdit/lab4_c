	.file	"try_3.c"
	.text
	.globl	_menu_size
	.section .rdata,"dr"
	.align 2
_menu_size:
	.word	5
	.globl	_menu
LC0:
	.ascii "Exit\0"
LC1:
	.ascii "Find Element\0"
LC2:
	.ascii "Add Element\0"
LC3:
	.ascii "Delete Element\0"
LC4:
	.ascii "Print Element\0"
	.data
	.align 32
_menu:
	.long	_Exit
	.long	LC0
	.long	_findElement
	.long	LC1
	.long	_addElement
	.long	LC2
	.long	_delElement
	.long	LC3
	.long	_printElement
	.long	LC4
	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB20:
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
LFE20:
	.globl	_nullTable
	.def	_nullTable;	.scl	2;	.type	32;	.endef
_nullTable:
LFB21:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	L4
L7:
	movl	$0, -8(%ebp)
	jmp	L5
L6:
	movl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	imull	$100, %edx, %ecx
	movl	-4(%ebp), %edx
	addl	%ecx, %edx
	movl	$0, (%eax,%edx,4)
	addl	$1, -8(%ebp)
L5:
	cmpl	$1, -8(%ebp)
	jle	L6
	addl	$1, -4(%ebp)
L4:
	cmpl	$99, -4(%ebp)
	jle	L7
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE21:
	.globl	_checkKeys
	.def	_checkKeys;	.scl	2;	.type	32;	.endef
_checkKeys:
LFB22:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jne	L10
	movl	8(%ebp), %eax
	movl	4(%eax), %edx
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	cmpl	%eax, %edx
	jne	L10
	movl	$1, %eax
	jmp	L12
L10:
	movl	$0, %eax
L12:
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE22:
	.section .rdata,"dr"
LC5:
	.ascii "\0"
LC6:
	.ascii "%s\0"
LC7:
	.ascii "%du\0"
LC8:
	.ascii "%*[^\12]\0"
LC9:
	.ascii "Incorrect type.\12\0"
	.text
	.globl	_getKey
	.def	_getKey;	.scl	2;	.type	32;	.endef
_getKey:
LFB23:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$LC5, -12(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC6, (%esp)
	call	_printf
L17:
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC6, (%esp)
	call	_printf
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC7, (%esp)
	call	_scanf
	movw	%ax, -14(%ebp)
	cmpw	$0, -14(%ebp)
	jns	L14
	movl	$0, %eax
	jmp	L15
L14:
	cmpw	$0, -14(%ebp)
	jne	L16
	movl	$LC8, (%esp)
	call	_scanf
	movl	$LC9, -12(%ebp)
L16:
	cmpw	$0, -14(%ebp)
	je	L17
	movl	$1, %eax
L15:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE23:
	.section .rdata,"dr"
LC10:
	.ascii "%80[^\12]\0"
LC11:
	.ascii "%*c\0"
	.text
	.globl	_getString
	.def	_getString;	.scl	2;	.type	32;	.endef
_getString:
LFB24:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$120, %esp
	movl	$0, -12(%ebp)
	movl	$1, (%esp)
	call	_malloc
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movb	$0, (%eax)
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_puts
L23:
	leal	-99(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC10, (%esp)
	call	_scanf
	movw	%ax, -18(%ebp)
	cmpw	$0, -18(%ebp)
	jns	L19
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, -16(%ebp)
	movl	$0, %eax
	jmp	L24
L19:
	cmpw	$0, -18(%ebp)
	jne	L21
	movl	$LC11, (%esp)
	call	_scanf
	jmp	L22
L21:
	leal	-99(%ebp), %eax
	movl	%eax, (%esp)
	call	_strlen
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$1, %eax
	movl	%eax, 4(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_realloc
	movl	%eax, -16(%ebp)
	leal	-99(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_strcat
L22:
	cmpw	$0, -18(%ebp)
	jg	L23
	movl	8(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax)
	movl	$1, %eax
L24:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE24:
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
	.def	_Exit;	.scl	2;	.type	32;	.endef
	.def	_findElement;	.scl	2;	.type	32;	.endef
	.def	_addElement;	.scl	2;	.type	32;	.endef
	.def	_delElement;	.scl	2;	.type	32;	.endef
	.def	_printElement;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_scanf;	.scl	2;	.type	32;	.endef
	.def	_malloc;	.scl	2;	.type	32;	.endef
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_free;	.scl	2;	.type	32;	.endef
	.def	_strlen;	.scl	2;	.type	32;	.endef
	.def	_realloc;	.scl	2;	.type	32;	.endef
	.def	_strcat;	.scl	2;	.type	32;	.endef
