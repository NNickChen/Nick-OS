[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api029.nas"]

		GLOBAL	_api_getlang
		
[SECTION .text]

_api_getlang:
		MOV		EDX,29
		INT		0x40
		RET