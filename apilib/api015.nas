[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api015.nas"]

		GLOBAL	_api_closewin
		
[SECTION .text]

_api_closewin:
		PUSH	EBX
		MOV		EDX,15
		MOV		EBX,[ESP+8]
		INT		0x40
		POP		EBX
		RET