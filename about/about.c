#include "apilib.h"


void HariMain(void)
{
	int win, langmode = api_getlang();;
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(250 * 100);
	if(langmode == 0){
		win = api_openwin(buf, 250, 100, -1, "About...");
	} else {
		win = api_openwin(buf, 250, 100, -1, "关于...");
	}
	api_txtbox(win, 8, 28, 234, 63, 0);
	if(langmode == 0){
		api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
		api_putstrwin(win, 12, 48, 7, 15, "[version 0.0.7]");
		api_putstrwin(win, 12, 64, 7, 13, "(c) 2018 Nick");
	} else {
		api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
		api_putstrwin(win, 12, 48, 7, 15, "[版本 0.0.7]");
		api_putstrwin(win, 12, 64, 7, 28, "(c) 2018 Nick 保留所有权利。");
	}
	for(;;){
		if(api_getkey == 251){
			break;
		}
	}
	api_end();
}
