#include "apilib.h"


void HariMain(void)
{
	int win, *x, *y;
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(150 * 100);
	x = (int *) api_malloc(1);
	y = (int *)	api_malloc(1);
	win = api_openwin(buf, 150, 100, -1, "About...");
	api_txtbox(win, 8, 28, 134, 63, 0);
	api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
	api_putstrwin(win, 12, 48, 7, 15, "[version 0.0.6]");
	api_putstrwin(win, 12, 64, 7, 13, "(c) 2018 Nick");
	api_getmouse(win, 1, x, y);
	if(0 <= *x && *x <= 150 && 0 <= *y && *y <= 100){
		api_closewin(win);
		api_end();
	}
}
