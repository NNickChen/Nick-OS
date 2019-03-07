#include "apilib.h"
#include <stdlib.h>

void NickStartup(void)
{
	int win;
	char buf[144 * 52];
	
	
	win = api_openwin(buf, 144, 52, -1, "hello", 0);
	api_boxfilwin(win, 8, 36, 141, 43, 3);
	api_putstrwin(win, 28, 28, 0, 12, "hello,world!");
	for(;;){
		if(api_getkey(1) == 0x0a){
			break;
		}
	}
	free(buf);
	api_end();
}
