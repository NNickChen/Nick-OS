#include "apilib.h"
unsigned char rgb2pal(int r, int g, int b, int x, int y);

void NickStartup(void)
{
	int win, x, y, r, g, b;
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(144 * 164);
	if(api_getlang() == 0){
		win = api_openwin(buf, 144, 164, -1, "color", 0);
	} else {
		win = api_openwin(buf, 144, 164, -1, "������ɫ", 0);
	}
	
	for(y = 0; y < 128; y++){
		for(x = 0; x < 128; x++){
			r = x * 2;
			g = y * 2;
			b = 0;
			buf[(x + 8) + (y + 28) * 144] = rgb2pal(r, g, b, x, y);
		}
	}
	api_refreshwin(win, 8, 28, 144, 164);
	for(;;){
		if(api_getkey(1) == 251){
			break;
		}
	}
	api_end();
}

unsigned char rgb2pal(int r, int g, int b, int x, int y)
{
	static int table[4] = { 3, 1, 0, 2 };
	int i;
	x &= 1; 
	y &= 1;
	i = table[x + y * 2];	
	r = (r * 21) / 256;	
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4;	
	g = (g + i) / 4;
	b = (b + i) / 4;
	return 16 + r + g * 6 + b * 36;
}
