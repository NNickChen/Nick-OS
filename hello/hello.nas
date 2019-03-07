[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "hello.nas"]

		GLOBAL	_NickStartup
		
[SECTION .text]

_NickStartup:
		MOV		EDX,29
		INT		0x40
		CMP		EAX,0
		JNE		chinese
		MOV		EBX,msg
		MOV		EDX,2
		JMP		putstr
chinese:
		MOV		EBX,msg1
		MOV		EDX,2
putstr:
		INT		0X40

		MOV		EDX,4
		INT		0x40
		
		
[SECTION .data]

msg:
	DB	"hello,world!",0x0a,0
	
msg1:
	DB	"ÄãºÃ£¬ÊÀ½ç£¡",0x0a,0
	