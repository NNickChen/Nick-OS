#include "apilib.h"
#include <stdlib.h>

void NickStartup(void)
{
	char *buf;
	int win, mx, my, btn, langmode = api_getlang();
	
	api_initmalloc();
	buf = malloc(200 * 200);
	if(langmode == 0){
		win = api_openwin(buf, 200, 200, -1, "draw", 0);
	} else {
		win = api_openwin(buf, 200, 200, -1, "»­Í¼", 0);
	}
	api_txtbox(win, 8, 28, 184, 163, 7);
	for(;;){
		mx = api_getmouse(win, 0);
		my = api_getmouse(win, 1);
		btn = api_getmouse(win, 2);
		if(8 <= mx && mx <= 190 && 28 <= my && my <= 190 && btn == 1){
			api_point(win, mx, my, 0);
		}
	}
	api_end();
}
	