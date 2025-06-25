void api_end(void);
int api_openwin(char *buf, int xsize, int ysize, int col_inv, char *title);

char buf[150 * 50];

void HariMain(void)
{
	int win;
	win = api_openwin(buf, 150, 50, -1, "hello");
	api_end();
}
