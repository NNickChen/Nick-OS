int api_openwin(char *buf, int xsize, int ysize, int col_inv, char *title);
void api_end();
int api_getkey(int mode);
void api_initmalloc(void);
char *api_malloc(int size);
void api_txtbox(int win, int x0, int y0, int x1, int y1, int col);
int api_timeralloc(void);
void api_inittimer(int timer, int data);
void api_settimer(int timer, int time);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);

void HariMain(void)
{
	int win, timer, cursor_c = 7, cursor_x = 8, i;
	char *buf, s[2];
	
	buf = api_malloc(144 * 52);
	win = api_openwin(buf, 144, 52, -1, "textbox");
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
			} else {
				if(cursor_x < 128){
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