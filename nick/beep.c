#include "bootpack.h"

void beep_on(void)
{
	int i = io_in8(0x61);
	io_out8(0x61, (i | 0x03) & 0x0f);
	return;
}

void beep_settone(int tone)
{
	io_out8(0x43, 0xb6);
	int i = 1193180000 / tone;
	io_out8(0x42, i & 0xff);
	io_out8(0x42, i >> 8);
	return;
}

void beep_off(void)
{
	int i = io_in8(0x61);
	io_out8(0x61, i & 0xd);
	return;
}
