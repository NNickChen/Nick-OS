[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api007.nas"]

		GLOBAL	_api_boxfilwin
		
[SECTION .text]

_api_boxfilwin:
		PUSH	EBX
		PUSH	ESI
		PUSH	EDI
		PUSH	EBP
		MOV		EDX,7
		MOV		EBX,[ESP+20]
		MOV		EAX,[ESP+24]
		MOV		ECX,[ESP+28]
		MOV		ESI,[ESP+32]
		MOV		EDI,[ESP+36]
		MOV		EBP,[ESP+40]
		INT		0x40
		POP		EBP
		POP		EDI
		POP		ESI
		POP		EBX
		RET