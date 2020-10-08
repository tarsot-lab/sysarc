.data
	msg:	.string "cognito, ergo sum\n"
	tam:	.int . - msg

.text
	.globl _start

_start:
	mov $4, %eax   # write
	mov $1, %ebx   # salida estandar
	mov $msg, %ecx # cadena
	mov tam, %edx  # longitud
	int $0x80      # llamada al sistema

	mov $1, %eax   # exit
	xor %ebx, %ebx # 0
	int $0x80      # llamada al sistema
