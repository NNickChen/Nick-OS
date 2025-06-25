[FORMAT	"WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api006.nas"]

		GLOBAL	_api_putstrwin
		
[SECTION .text]

_api_putstrwin:
		PUSH	EBX
		PUSH	ESI
		PUSH	EDI
		PUSH	EBP
		MOV		EDX,6
		MOV		EBX,[ESP+20]
		MOV		ESI,[ESP+24]
		MOV		EDI,[ESP+28]
		MOV		EAX,[ESP+32]
		MOV		ECX,[ESP+36]
		MOV		EBP,[ESP+40]
		INT		0x40
		POP		EBP
		POP		EDI
		POP		ESI
		POP		EBX
		RET