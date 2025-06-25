#include <stdio.h>
#include "apilib.h"

void NickStartup(void)
{
	char *buf, s[8];
	int win, timer, i, key_to = 0;
	unsigned int hou = 0, min = 0, sec = 0;
	
	api_initmalloc();
	buf = api_malloc(150 * 50);
	if(api_getlang == 0){
		win = api_openwin(buf, 150, 50, -1, "timer", 0);
	} else {
		win = api_openwin(buf, 150, 50, -1, "¼ÆÊ±Æ÷", 0);
	}

go:	
	key_to = 0;
	hou = 0;
	min = 0;
	sec = 0;
	api_boxfilwin(win + 1, 28, 27, 115, 41, 7);
	sprintf(s, "%02d:%02d:%02d", hou, min, sec);
	api_boxfilwin(win + 1, (3 - key_to) * 26, 27, (3 - key_to) * 28 + 24, 41, 8);
	api_putstrwin(win + 1, 34, 27, 0, 8, s);
	api_refreshwin(win, 28, 27, 115, 43);
	for(;;){
		i = api_getkey(1);
		if(key_to == 0){
			if(i == 255){
				if(sec < 59){
					sec++;
				}
			}
			if(i == 254){
				if(sec > 0){
					sec--;
				}
			}
		} else if(key_to == 1){
			if(i == 255){
				if(min < 59){
					min++;
				}
			}
			if(i == 254){
				if(min > 0){
					min--;
				}
			}
		} else {
			if(i == 255){
				if(hou < 23){
					hou++;
				}
			}
			if(i == 254){
				if(hou > 0){
					hou --;
				}
			}
		}
		if(i == 253){
			if(key_to < 2){
				key_to++;
			}
		}
		if(i == 252){
			if(key_to > 0){
				key_to--;
			}
		}
		if(i == 0x0a){
			break;
		}
		if(i == 251){
			goto fin;
		}
		api_boxfilwin(win + 1, 28, 27, 115, 41, 7);
		sprintf(s, "%02d:%02d:%02d", hou, min, sec);
		api_boxfilwin(win + 1, (3 - key_to) * 26, 27, (3 - key_to) * 28 + 24, 41, 8);
		api_putstrwin(win + 1, 34, 27, 0, 8, s);
		api_refreshwin(win, 28, 27, 115, 43);
	}
	api_boxfilwin(win + 1, 28, 27, 115, 41, 7);
	sprintf(s, "%02d:%02d:%02d", hou, min, sec);
	api_putstrwin(win + 1, 34, 27, 0, 8, s);
	api_refreshwin(win, 28, 27, 115, 43);
	if(min > 0 && sec == 0){
		sec = 60;
		min--;
	}
	if(hou > 0 && min == 0){
		min = 60;
		hou--;
	}
	timer = api_alloctimer();
	api_inittimer(timer, 128);
	api_settimer(timer, 100);
	for(;;){
		if(sec <= 0 && min <= 0 && hou <= 0){
			break;
		} else if (api_getkey(1) == 128){
			sec--;
			if(sec == 0){
				if(min > 0){
					sec = 60;
					min--;
				}
				if(min == 0){
					if(hou > 0){
						min = 60;
						hou--;
					}
				}
			}
			api_boxfilwin(win + 1, 28, 27, 115, 41, 7);
			sprintf(s, "%02d:%02d:%02d", hou, min, sec);
			api_putstrwin(win + 1, 34, 27, 0, 8, s);
			api_settimer(timer, 100);
			api_refreshwin(win, 28, 27, 115, 43);
		} else if(i == 251){
			api_end();
		} else if(i == 0x0a){
			goto go;
		}
	}
	api_beep(200000);
	for(;;){
		i = api_getkey(1);
		if(i == 128){
			api_beep(0);
			api_inittimer(timer, 129);
			api_settimer(timer, 100);
		} else if(i == 129){
			api_beep(200000);
			api_inittimer(timer, 128);
			api_settimer(timer, 100);
		} else if(i == 0x0a){
			goto go;
		} else if(i == 251){
			goto fin;
		}
	}
fin:
	api_end();
}
