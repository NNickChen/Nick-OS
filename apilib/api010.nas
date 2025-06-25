[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api010"]

		GLOBAL	_api_free
		
[SECTION .text]

_api_free:
		PUSH	EBX
		MOV		EDX,10
		MOV		EBX,[CS:0X0020]
		MOV		ECX,[ESP+8]
		MOV		EAX,[ESP+12]
		INT		0x40
		POP		EBX
		RET