[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api012.nas"]

		GLOBAL	_api_refreshwin
		
[SECTION .text]

_api_refreshwin:
		PUSH	EBX
		PUSH	ESI
		PUSH	EDI
		MOV		EDX,12
		MOV		EBX,[ESP+16]
		MOV		EAX,[ESP+20]
		MOV		ECX,[ESP+24]
		MOV		ESI,[ESP+28]
		MOV		EDI,[ESP+32]
		INT		0x40
		POP		EDI
		POP		ESI
		POP		EBX
		RET