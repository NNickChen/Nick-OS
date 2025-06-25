[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api020.nas"]

		GLOBAL	_api_settimer
		
[SECTION .text]

_api_settimer:
		PUSH	EBX
		MOV		EDX,20
		MOV		EBX,[ESP+8]
		MOV		EAX,[ESP+12]
		INT		0x40
		POP		EBX
		RET