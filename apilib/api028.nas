[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_cmdline
		
[SECTION .text]

_api_cmdline:
		PUSH	EBX
		MOV		EDX,28
		MOV		EBX,[ESP+8]
		MOV		ECX,[ESP+12]
		INT		0x40
		POP		EBX
		RET
		