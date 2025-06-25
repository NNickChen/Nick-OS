[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api022.nas"]

		GLOBAL _api_beep
		
[SECTION .text]

_api_beep:
		MOV		EDX,22
		MOV		EAX,[ESP+4]
		INT		0x40
		RET