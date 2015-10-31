#
#simon <nicksimon109445@gmail.com>
#2015-05-29 12:54
#print process vendor
#
.data
msg:
	.asciz "Hello World 'xxxxxxxxxxxx'\n"
len:
	.long . - msg
.text
.code32
.global main
main:
	movl $msg,%edi
	movl $0,%eax
	cpuid
	movl %ebx,13(%edi)
	movl %edx,17(%edi)
	movl %ecx,21(%edi)
	movl $4,%eax
	movl $1,%ebx
	movl $msg,%ecx
	movl len,%edx
	int $0x80
	movl $1,%eax
	movl $0,%ebx
	int $0x80
