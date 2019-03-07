#include "apilib.h"
#include <stdlib.h>

void NickStartup(void)
{
	int win;
	char buf[130 * 180];
	
	api_initmalloc();
	if(api_getlang() == 0){
		win = api_openwin(buf, 130, 180, 255, "notrec", 1);
	} else {
		win = api_openwin(buf, 130, 180, 255, "·Ç¾ØÐÎ´°¿Ú", 1);
	}
	api_boxfilwin(win + 1, 0, 0, 60, 30, 255);
	api_boxfilwin(win + 1, 70, 0, 130, 30, 255);
	api_boxfilwin(win + 1, 30, 80, 100, 100, 255);
	api_boxfilwin(win + 1, 0, 110, 40, 130, 255);
	api_boxfilwin(win + 1, 80, 110, 130, 180, 255);
	api_refreshwin(win, 0, 0, 130, 180);
	
	for(;;){
		if(api_getkey(1) == 251){
			break;
		}
	}
	exit(1);
}
