[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api025.nas"]

		GLOBAL	_api_fseek
		
[SECTION .text]

_api_fseek:
		PUSH	EBX
		MOV		EDX,25
		MOV		EAX,[ESP+8]
		MOV		ECX,[ESP+16]
		MOV		EBX,[ESP+12]
		INT		0x40
		POP		EBX
		RET
		