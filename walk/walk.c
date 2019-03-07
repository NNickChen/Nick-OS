#include <stdio.h>
#include "apilib.h"

struct key {
	int right, left, space, esc;
};
void wait(int i, int timer, struct key *key);
void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen);

void NickStartup(void)
{
	int win, timer, x = 80, y = 50, i, mx, my, btn;
	char *buf;
	struct key key;
	
	timer = api_alloctimer();
	api_inittimer(timer, 128);
	api_initmalloc();
	buf = api_malloc(160 * 100);
	win = api_openwin(buf, 160, 100, -1, "walk", 0);
	mx = (int *) api_malloc(1);
	my = (int *) api_malloc(1);
	api_txtbox(win, 8, 28, 144, 63, 0);
	api_putstrwin(win, x, y, 3, 1, "*");
	for(;;){
		i = api_getkey(1);
		api_putstrwin(win, x, y, 0, 1, "*");
		if(i == 'w' && y > 30){y -= 8;}
		if(i == 'a' && x > 10){x -= 8;}
		if(i == 's' && y < 70){y += 8;}
		if(i == 'd' && x < 140){x += 8;}
		if(i == 251){
			api_putstrwin(win, 8, 30, COL8_FFFFFF, 20, "确定要关闭此程序吗");
			make_button8(win, 20, 50, 60, 80, "确定", 0, 4);
			make_button8(win, 100, 50, 140, 80, "取消", 0, 4);
			for(;;){
				mx = api_getmouse(win, 0);
				my = api_getmouse(win, 1);
				btn = api_getmouse(win, 2);
				if(19 <= mx && mx <= 61 && 49 <= my && my <= 81 && btn == 1){
					make_button8(win, 20, 50, 60, 80, "确定", 1, 4);
					wait(20, timer, &key);
					make_button8(win, 20, 50, 60, 80, "确定", 0, 4);
					break;
				} else if(99 <= mx && mx <= 139 && 49 <= my && my <= 81 && btn == 1){
					make_button8(win, 100, 50, 140, 80, "取消", 1, 4);
					wait(20, timer, &key);
					make_button8(win, 100, 50, 140, 80, "取消", 0, 4);
					api_txtbox(win, 8, 28, 144, 63, 0);
					goto go;
				} 
			}
			break;
		}
go:
		api_putstrwin(win, x, y, 3, 1, "*");
	}
	api_end();
}

void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen)
{
	if(act == 0){
		api_boxfilwin(win, x0 - 1, y0 - 1, x1 + 1, y1 + 1, COL8_848484);
		api_boxfilwin(win, x0 - 1, y0 - 1, x1, y1, COL8_FFFFFF);
		api_boxfilwin(win, x0, y0, x1, y1, COL8_C6C6C6);
	} else {
		api_boxfilwin(win, x0 - 1, y0 - 1, x1, y1, COL8_848484);
		api_boxfilwin(win, x0, y0, x1 + 1, y1 + 1, COL8_FFFFFF);
		api_boxfilwin(win, x0, y0, x1, y1, COL8_C6C6C6);
	}
	api_putstrwin(win, x0 + 5, y0 + 10, 0, strlen, s);
	return;
}

void wait(int i, int timer, struct key *key)
{
	int j;
	if (i > 0) {
		api_settimer(timer, i);
		i = 128;
	} else {
		i = 0x0a; /* Enter */
	}
	for (;;) {
		j = api_getkey(1);
		if (i == j) {
			break;
		}
		if (i == 251){
			key->esc = 1;
		}
		if (j == 253) {
			key->left = 1;
		}
		if (j == 252) {
			key->right = 1;
		}
		if (j == ' ') {
			key->space = 1;
		}
	}
	return;
}
