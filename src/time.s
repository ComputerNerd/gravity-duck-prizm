
	.global	_time_getTicks
	.type	_time_getTicks, @function

_time_getTicks:
	mov.l syscall_address, r2
	mov.l getTicks, r0
	jmp @r2
	nop

syscall_address:
	.long 0x80020070
getTicks:
	.long 0x02C1
