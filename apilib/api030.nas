[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api030.nas"]

		GLOBAL	_api_setlang
		
[SECTION .text]

_api_setlang:
		MOV		EDX,30
		MOV		EAX,[ESP+4]
		INT		0x40
		RET
		