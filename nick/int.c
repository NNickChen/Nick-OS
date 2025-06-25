#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
{
	io_out8(PIC0_IMR,  0xff  );
	io_out8(PIC1_IMR,  0xff  ); 

	io_out8(PIC0_ICW1, 0x11  );
	io_out8(PIC0_ICW2, 0x20  );
	io_out8(PIC0_ICW3, 1 << 2);
	io_out8(PIC0_ICW4, 0x01  ); 

	io_out8(PIC1_ICW1, 0x11  ); 
	io_out8(PIC1_ICW2, 0x28  ); 
	io_out8(PIC1_ICW3, 2     ); 
	io_out8(PIC1_ICW4, 0x01  );

	io_out8(PIC0_IMR,  0xfb  );
	io_out8(PIC1_IMR,  0xff  ); 

	return;
}

void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67);
	return;
}

int *inthandler0d(int *esp)
{
	char s[30];
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	if(task->langmode != 0){
		cons_putstr0(cons, "\nINT 0D :\n 一般保护异常。\n");
	} else {
		cons_putstr0(cons, "\nINT 0D :\n General Protected Exception.\n");
	}
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}

int *inthandler0c(int *esp)
{
	char s[30];
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	if(task->langmode != 0){
		cons_putstr0(cons, "\nINT 0C :\n 栈异常。\n");
	} else {
		cons_putstr0(cons, "\nINT 0C :\n Stack Exception.\n");
	}
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}

int *inthandler00(int *esp)
{
	char s[30];
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	if(task->langmode != 0){
		cons_putstr0(cons, "\nINT 00 :\n 除‘0’异常。\n");
	} else {
		cons_putstr0(cons, "\nINT 00 :\n Devide '0' Exception.\n");
	}
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}
