int api_openwin(char *buf, int xsize, int ysize, int col_inv, char *title);
void api_end(void);
void api_initmalloc(void);
char *api_malloc(int size);
void api_txtbox(int win, int x0, int y0, int x1, int y1, int col);
void api_refreshwin(int win, int x0, int y0, int x1, int y1);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);
void api_closewin(int win);
void api_getmouse(int win, int mode, int *x, int *y);


void HariMain(void)
{
	int win, *x, *y;
	char *buf;
	
	api_initmalloc();
	buf = api_malloc(150 * 100);
	x = (int *) api_malloc(1);
	y = (int *)	api_malloc(1);
	win = api_openwin(buf, 150, 100, -1, "About...");
	api_txtbox(win, 8, 28, 134, 63, 0);
	api_putstrwin(win, 12, 32, 7, 7, "Nick OS");
	api_putstrwin(win, 12, 48, 7, 15, "[version 0.0.4]");
	api_putstrwin(win, 12, 64, 7, 13, "(c) 2017 Nick");
	api_getmouse(win, 1, x, y);
	if(0 <= *x && *x <= 150 && 0 <= *y && *y <= 100){
		api_closewin(win);
		api_end();
	}
}
