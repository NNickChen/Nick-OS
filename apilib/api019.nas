[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api019.nas"]

		GLOBAL	_api_inittimer
		
[SECTION .text]

_api_inittimer:
		PUSH	EBX
		MOV		EDX,19
		MOV		EBX,[ESP+8]
		MOV		EAX,[ESP+12]
		INT 	0x40
		POP 	EBX
		RET