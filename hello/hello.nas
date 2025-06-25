[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "hello5.nas"]

		GLOBAL	_NickStartup
		
[SECTION .text]

_NickStartup:
		MOV		EBX,msg
		MOV		EDX,2
		INT		0X40
		MOV		EDX,4
		INT		0x40
		
		
[SECTION .data]

msg:
	DB	"ÄãºÃ£¡",0x0a,0
	