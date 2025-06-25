[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api021.nas"]

		GLOBAL	_api_freetimer
		
[SECTION .text]

_api_freetimer:
		PUSH	EBX
		MOV		EBX,[ESP+8]
		MOV		EDX,21
		INT		0x40
		POP		EBX
		RET