int api_openwin(char *buf, int xsize, int ysize, int col_vin, char *title);
void api_fillbox(int win, int x0, int y0, int x1, int y1, int col);
void api_winstr(int win, int x, int y, int col, int len, char *str);
void api_end(void);
void api_memman_init(void);
char *api_memman_alloc(int size);

char buf[150 * 50];

void HariMain(void)
{
	int win;
	
	win = api_openwin(buf, 150, 50, -1, "hello");
	api_fillbox(win, 8, 36, 141, 43, 3);
	api_winstr(win, 28, 28, 0, 12, "hello,world!");
	api_end();
}
