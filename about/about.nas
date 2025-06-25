[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 32]
	EXTERN	_api_getlang
	EXTERN	_api_initmalloc
	EXTERN	_api_malloc
	EXTERN	_api_openwin
	EXTERN	_api_txtbox
	EXTERN	_api_putstrwin
	EXTERN	_api_getkey
	EXTERN	_exit
[FILE "about.c"]
[SECTION .data]
LC0:
	DB	0xB9,0xD8,0xD3,0xDA,"...",0x00
LC2:
	DB	"Nick OS",0x00
LC3:
	DB	"[version 0.1.0.20180116_alpha]",0x00
LC4:
	DB	"(c) 2018 Nick",0x00
LC5:
	DB	"[",0xB0,0xE6,0xB1,0xBE," 0.1.0.20180116_alpha]",0x00
LC6:
	DB	"(c) 2018 Nick ",0xB1,0xA3,0xC1,0xF4,0xCB,0xF9,0xD3,0xD0,0xC8,0xA8,0xC0,0xFB,0xA1,0xA3,0x00
LC1:
	DB	"about...",0x00
[SECTION .text]
	GLOBAL	_NickStartup
_NickStartup:
	PUSH	EBP
	MOV	EBP,ESP
	PUSH	ESI
	PUSH	EBX
	CALL	_api_getlang
	MOV	ESI,EAX
	CALL	_api_initmalloc
	PUSH	27000
	CALL	_api_malloc
	MOV	EBX,EAX
	CALL	_api_getlang
	TEST	EAX,EAX
	POP	ECX
	JE	L2
	PUSH	0
	PUSH	LC0
L10:
	PUSH	-1
	PUSH	100
	PUSH	270
	PUSH	EBX
	CALL	_api_openwin
	ADD	ESP,24
	MOV	EBX,EAX
	PUSH	0
	PUSH	63
	PUSH	254
	PUSH	28
	PUSH	8
	PUSH	EAX
	CALL	_api_txtbox
	ADD	ESP,24
	TEST	ESI,ESI
	JNE	L4
	PUSH	LC2
	PUSH	7
	PUSH	7
	PUSH	32
	PUSH	12
	PUSH	EBX
	CALL	_api_putstrwin
	PUSH	LC3
	PUSH	30
	PUSH	7
	PUSH	48
	PUSH	12
	PUSH	EBX
	CALL	_api_putstrwin
	ADD	ESP,48
	PUSH	LC4
	PUSH	13
L11:
	PUSH	7
	PUSH	64
	PUSH	12
	PUSH	EBX
	CALL	_api_putstrwin
	ADD	ESP,24
L6:
	PUSH	1
	CALL	_api_getkey
	POP	EDX
	CMP	EAX,251
	JNE	L6
	JMP	L12
L4:
	PUSH	LC2
	PUSH	7
	PUSH	7
	PUSH	32
	PUSH	12
	PUSH	EBX
	CALL	_api_putstrwin
	PUSH	LC5
	PUSH	30
	PUSH	7
	PUSH	48
	PUSH	12
	PUSH	EBX
	CALL	_api_putstrwin
	ADD	ESP,48
	PUSH	LC6
	PUSH	28
	JMP	L11
L2:
	PUSH	0
	PUSH	LC1
	JMP	L10
L12:
	PUSH	0
	CALL	_exit
