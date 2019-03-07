#include <stdio.h>
#include <stdlib.h>
#include "apilib.h"

void NickStartup(void)
{
	char *buf, s[12];
	int win, timer, sec = 0, min = 0, hou = 0, i, langmode = api_getlang();
	
	api_initmalloc();
	buf = api_malloc(150 * 50);
	if(langmode == 0){
		win = api_openwin(buf, 150, 50, -1, "stop watch", 0);
	} else {
		win = api_openwin(buf, 150, 50, -1, "秒表", 0);
	}
	timer = api_alloctimer();
	api_inittimer(timer, 128);
	api_settimer(timer, 100);
	sprintf(s, "%5d:%02d:%02d", hou, min, sec);         
	api_boxfilwin(win, 28, 27, 115, 41, 7 /*白色*/);         
	api_putstrwin(win, 28, 27, 0 /*黑色*/, 11, s); 
	for(;;){
		i = api_getkey(1);
		if(i == 128){
			sec++;
			if(sec >= 60){
				sec = 0;
				min++;
				if(min >= 60){
					min = 0;
					sec = 0;
					hou++;
				}
			}
			  sprintf(s, "%5d:%02d:%02d", hou, min, sec);         
			  api_boxfilwin(win, 28, 27, 115, 41, 7 /*白色*/);         
			  api_putstrwin(win, 28, 27, 0 /*黑色*/, 11, s); 
			api_settimer(timer, 100);
		} else {
			if(i == 0x0a){
				break;
			}
		}
	}
	for(;;){
		if(api_getkey(1) == 251){
			break;
		}
	}
	exit(1);
}
