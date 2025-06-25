[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api017.nas"]

		GLOBAL	_api_getmouse
		
[SECTION .text]

_api_getmouse:
		PUSH	EBX
		PUSH	ESI
		MOV		EDX,17
		MOV		EBX,[ESP+12]
		MOV		EAX,[ESP+16]
		MOV		ECX,[ESP+20]
		MOV		ESI,[ESP+24]
		INT		0x40
		POP		ESI
		POP		EBX
		RET