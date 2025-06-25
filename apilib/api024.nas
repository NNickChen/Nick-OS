[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api024.nas"]

		GLOBAL	_api_fclose
		
[SECTION .text]

_api_fclose:
		MOV		EDX,24
		MOV		EAX,[ESP+4]
		INT		0x40
		RET
		