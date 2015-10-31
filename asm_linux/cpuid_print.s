.data
msg:
	.asciz "Hello world : %s\n"
.bss
	.lcomm buffer,12
.text
.global main
main:
	movl $0,%eax
	cpuid
	movl $buffer,%edi
	movl %ebx,(%edi)
	movl %ecx,4(%edi)
	movl %edx,8(%edi)
	pushl $buffer
	pushl $msg
	call printf
	pushl $0
	call exit
