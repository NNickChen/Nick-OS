#include "apilib.h"
#include <stdlib.h>

void NickStartup(void)
{
	char *buf;
	int win, i, x, y, langmode = api_getlang();;
	
	api_initmalloc();
	buf = api_malloc(150 * 100);
	if(langmode == 0){
		win = api_openwin(buf, 150, 100, -1, "stars", 0);
	} else {
		win = api_openwin(buf, 150, 100, -1, "ÐÇ¿Õ", 0);
	}
	api_txtbox(win, 8, 28, 134, 63, 0);
	for(i = 0; i < 50; i++){
		x = (rand() % 137) + 6;
		y = (rand() % 67) + 26;
		api_point(win + 1, x, y, 3);
	}
	api_refreshwin(win, 6, 26, 143, 93);
	for(;;){
		if(api_getkey(1) == 0x0a){
			break;
		}
	}
	api_closewin(win);
	api_end();
}
