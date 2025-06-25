#include "apilib.h"

void HariMain(void)
{
	char *buf;
	int win, i, *mx, *my;
	
	api_initmalloc();
	buf = api_malloc(200 * 200);
	win = api_openwin(buf, 200, 200, -1, "draw");
	mx = (int *) api_malloc(1);
	my = (int *) api_malloc(1);
	api_txtbox(win, 8, 28, 184, 163, 7);
	for(;;){
		api_getmouse(win, 1, mx, my);
		api_point(win, *mx, *my, 0);
	}
	api_end();
}
	