#include "apilib.h"
#include <stdlib.h>

void NickStartup(void)
{
	int win, langmode = api_getlang();
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(250 * 100);
	if(langmode == 0){
		api_setlang(1);
	}
	win = api_openwin(buf, 250, 100, -1, "����...", 0);
	api_txtbox(win, 8, 28, 234, 63, 0);
	if(langmode == 0){
		api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
		api_putstrwin(win, 12, 48, 7, 15, "[version 0.0.9]");
		api_putstrwin(win, 12, 64, 7, 13, "(c) 2018 Nick");
	} else {
		api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
		api_putstrwin(win, 12, 48, 7, 15, "[�汾 0.0.9]");
		api_putstrwin(win, 12, 64, 7, 28, "(c) 2018 Nick ��������Ȩ����");
	}
	for(;;){
		if(api_getkey(1) == 251){
			break;
		}
	}
	exit(0);
}
