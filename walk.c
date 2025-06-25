#include <stdio.h>
#include "apilib.h"

void HariMain(void)
{
	int win, x = 80, y = 50, i, *mx, *my;
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(160 * 100);
	win = api_openwin(buf, 160, 100, -1, "walk");
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
			api_putstrwin(win, 8, 30, 7, 17, "close the window?");
			api_boxfilwin(win, 30, 50, 60, 80, 8);
			api_putstrwin(win, 35, 60, 0, 3, "Yes");
			api_boxfilwin(win, 100, 50, 130, 80, 8);
			api_putstrwin(win, 110, 60, 0, 3, "No");
go:
			api_getmouse(win, 1, mx, my);
			if(30 <= *mx && *mx <= 60 && 50 <= *my && *my <= 80){
				break;
			} else if(100 <= *mx && *mx <= 130 && 50 <= *my && *my <= 80){
				api_txtbox(win, 8, 28, 144, 63, 0);
			} else {
				goto go;
			}
		}
		api_putstrwin(win, x, y, 3, 1, "*");
	}
	api_end();
}
