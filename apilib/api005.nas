[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api005.nas"]

		GLOBAL	_api_openwin
		
[SECTION .text]

_api_openwin:
		PUSH	EDI
		PUSH	ESI
		PUSH	EBX
		PUSH	EBP
		MOV		EDX,5
		MOV		EBX,[ESP+20]
		MOV		ESI,[ESP+24]
		MOV		EDI,[ESP+28]
		MOV  	EAX,[ESP+32]
		MOV		ECX,[ESP+36]
		MOV		EBP,[ESP+40]
		INT		0x40
		POP		EBP
		POP		EBX
		POP		ESI
		POP		EDI
		RET