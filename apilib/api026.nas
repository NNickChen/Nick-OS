[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api026.nas"]

		GLOBAL	_api_fsize
		
[SECTION .text]

_api_fsize:
		MOV		EDX,26
		MOV		EAX,[ESP+4]
		MOV		ECX,[ESP+8]
		INT		0x40
		RET
		