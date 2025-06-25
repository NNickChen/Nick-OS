#include "apilib.h"

int rand(void);

void HariMain(void)
{
	char *buf;
	int win, i, x, y, langmode = api_getlang();;
	
	api_initmalloc();
	buf = api_malloc(150 * 100);
	if(langmode == 0){
		win = api_openwin(buf, 150, 100, -1, "stars");
	} else {
		win = api_openwin(buf, 150, 100, -1, "ÐÇ¿Õ");
	}
	api_boxfilwin(win + 1, 6, 26, 143, 93, 0);
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
