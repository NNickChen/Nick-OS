[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "naskfunc.nas"]

		GLOBAL	_write_reg_ecx
		
[SECTION .text]

_write_reg:
		MOV		EDX,[ESP+4]
		CMP		ECX,0
		JS		err
		MOV		ECX,EDX
		MOV		EAX,ECX
		RET
err:
		MOV		EAX,-1
		RET
		