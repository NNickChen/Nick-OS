[FORMAT "WCOFF"]				
[INSTRSET "i486p"]				
[BITS 32]						
[FILE "a_nask.nas"]		

		GLOBAL 	_api_putchar, _api_putstr0
		GLOBAL	_api_end, _api_openwin
		GLOBAL	_api_boxfilwin, _api_putstrwin, _api_point
		GLOBAL	_api_initmalloc, _api_malloc, _api_free
		GLOBAL	_api_refreshwin, _api_txtbox, _api_linewin, _api_closewin
		GLOBAL	_api_getkey, _api_getmouse
		GLOBAL	_api_alloctimer, _api_inittimer, _api_settimer, _api_freetimer
		GLOBAL	_api_beep
		
[SECTION .text]

_api_putchar:
		MOV 	EDX,1
		MOV		EBX,1
		MOV 	AL,[ESP+4]
		INT		0x40
		RET
		
_api_putstr0:
		PUSH	EBX
		MOV		EDX,2
		MOV		EBX,[ESP+8]
		INT		0x40
		POP		EBX
		RET

_api_end:
		MOV 	EDX,4
		INT		0x40
		
_api_openwin:
		PUSH	EDI
		PUSH	ESI
		PUSH	EBX
		MOV		EDX,5
		MOV		EBX,[ESP+16]
		MOV		ESI,[ESP+20]
		MOV		EDI,[ESP+24]
		MOV  	EAX,[ESP+28]
		MOV		ECX,[ESP+32]
		INT		0x40
		POP		EBX
		POP		ESI
		POP		EDI
		RET
		
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
		
_api_initmalloc:
		PUSH	EBX
		MOV		EDX,8
		MOV		EBX,[CS:0x0020]		; malloc領域の番地
		MOV		EAX,EBX
		ADD		EAX,32*1024			; 32KBを足す
		MOV		ECX,[CS:0x0000]		; データセグメントの大きさ
		SUB		ECX,EAX
		INT		0x40
		POP		EBX
		RET
		
_api_malloc:
		PUSH	EBX
		MOV		EDX,9
		MOV		EBX,[CS:0x0020]
		MOV		ECX,[ESP+8]			; size
		INT		0x40
		POP		EBX
		RET
		
_api_free:
		PUSH	EBX
		MOV		EDX,10
		MOV		EBX,[CS:0X0020]
		MOV		ECX,[ESP+8]
		MOV		EAX,[ESP+12]
		INT		0x40
		POP		EBX
		RET
		
_api_point:
		PUSH	EDI
		PUSH	ESI
		PUSH	EBX
		MOV		EDX,11
		MOV		EBX,[ESP+16]	; win
		MOV		ESI,[ESP+20]	; x
		MOV		EDI,[ESP+24]	; y
		MOV		EAX,[ESP+28]	; col
		INT		0x40
		POP		EBX
		POP		ESI
		POP		EDI
		RET
		
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
		
_api_txtbox:
		PUSH	EBX
		PUSH	ESI
		PUSH	EDI
		PUSH	EBP
		MOV		EDX,13
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
		
_api_linewin:
		PUSH	EBX
		PUSH	ESI
		PUSH	EDI
		PUSH	EBP
		MOV		EDX,14
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
		
_api_closewin:
		PUSH	EBX
		MOV		EDX,15
		MOV		EBX,[ESP+8]
		INT		0x40
		POP		EBX
		RET
		
_api_getkey:
		MOV		EDX,16
		MOV		EAX,[ESP+4]
		INT		0x40
		RET
		
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

_api_alloctimer:
		MOV		EDX,18
		INT 	0x40
		RET
		
_api_inittimer:
		PUSH	EBX
		MOV		EDX,19
		MOV		EBX,[ESP+8]
		MOV		EAX,[ESP+12]
		INT 	0x40
		POP 	EBX
		RET
		
_api_settimer:
		PUSH	EBX
		MOV		EDX,20
		MOV		EBX,[ESP+8]
		MOV		EAX,[ESP+12]
		INT		0x40
		POP		EBX
		RET
		
_api_freetimer:
		PUSH	EBX
		MOV		EBX,[ESP+8]
		MOV		EDX,21
		INT		0x40
		POP		EBX
		RET

_api_beep:
		MOV		EDX,22
		MOV		EAX,[ESP+4]
		INT		0x40
		RET
		
		