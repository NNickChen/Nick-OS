#include "apilib.h"

void HariMain(void)
{
	int win, timer, cursor_c = 7, cursor_x = 8, i;
	char *buf, s[2];
	
	api_initmalloc();
	buf = api_malloc(144 * 52);
	if(api_getlang() == 0){
		win = api_openwin(buf, 144, 52, -1, "textbox");
	} else {
		win = api_openwin(buf, 144, 52, -1, "ÎÄ±¾ÊäÈë¿ò");
	}
	api_txtbox(win, 8, 28, 128, 16, 7);
	
	timer = api_alloctimer();
	api_inittimer(timer, 0);
	api_settimer(timer, 50);
	for(;;){
		i = api_getkey(1);
		if(i <= 1){
			if(i != 0){
				cursor_c = 7;
				api_inittimer(timer, 0);
			} else {
				cursor_c = 0;
				api_inittimer(timer, 1);
			}
			api_settimer(timer, 50);
		} else {
			if(i == 8){
				if(cursor_x > 8){
					api_boxfilwin(win, cursor_x, 28, cursor_x, 43, 7);
					api_boxfilwin(win, cursor_x - 8, 28, cursor_x, 43, 7);
					cursor_x -= 8;
				}
			} else if(i == 251){
				api_end();
			} else if(i <= 250){
				if(cursor_x < 128 ){
					s[0] = i;
					s[1] = 0;
					api_boxfilwin(win, cursor_x, 28, cursor_x, 43, 7);
					api_putstrwin(win, cursor_x, 28, 0, 1, s);
					cursor_x += 8;
				}
			}
		}
		api_boxfilwin(win, cursor_x, 28, cursor_x, 43, cursor_c);
	}
}
