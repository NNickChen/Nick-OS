[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api016.nas"]

		GLOBAL	_api_getkey
		
[SECTION .text]

_api_getkey:
		MOV		EDX,16
		MOV		EAX,[ESP+4]
		INT		0x40
		RET