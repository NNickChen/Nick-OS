[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api027.nas"]

		GLOBAL	_api_fread
		
[SECTION .text]

_api_fread:
		MOV		EDX,27
		PUSH	EBX
		MOV		EAX,[ESP+16]
		MOV		ECX,[ESP+12]
		MOV		EBX,[ESP+8]
		INT		0x40
		POP		EBX
		RET
		