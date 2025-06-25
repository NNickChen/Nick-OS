int api_openwin(char *buf, int xsize, int ysize, int col_inv, char *title);
void api_end(void);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);
void api_initmalloc(void);
char *api_malloc(int size);
void api_txtbox(int win, int x0, int y0, int x1, int y1, int col);
void api_refreshwin(int win, int x0, int y0, int x1, int y1);
void api_closewin(int win);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
int api_getkey(int mode);
void api_getmouse(int win, int mode, int *x, int *y);
void api_point(int win, int x, int y, int col);

void HariMain(void)
{
	char *buf;
	int win, i, *mx, *my;
	
	api_initmalloc();
	buf = api_malloc(200 * 200);
	win = api_openwin(buf, 200, 200, -1, "draw");
	mx = (int *) api_malloc(1);
	my = (int *) api_malloc(1);
	api_txtbox(win, 8, 28, 184, 163, 7);
	for(;;){
		api_getmouse(win, 1, mx, my);
		api_point(win, *mx, *my, 0);
	}
	api_end();
}
	