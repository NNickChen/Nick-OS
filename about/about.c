#include "apilib.h"
#include <stdlib.h>

void NickStartup(void)
{
	int win, langmode = api_getlang();
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(270 * 100);
	if(api_getlang() != 0){
		win = api_openwin(buf, 270, 100, -1, "关于...", 0);
	} else {
		win = api_openwin(buf, 270, 100, -1, "about...", 0);
	}
	api_txtbox(win, 8, 28, 254, 63, 0);
	if(langmode == 0){
		api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
		api_putstrwin(win, 12, 48, 7, 30, "[version 0.1.0.20180116_alpha]");
		api_putstrwin(win, 12, 64, 7, 13, "(c) 2018 Nick");
	} else {
		api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
		api_putstrwin(win, 12, 48, 7, 30, "[版本 0.1.0.20180116_alpha]");
		api_putstrwin(win, 12, 64, 7, 28, "(c) 2018 Nick 保留所有权利。");
	}
	for(;;){
		if(api_getkey(1) == 251){
			break;
		}
	}
	exit(0);
}
