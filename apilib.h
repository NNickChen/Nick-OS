#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15
void api_putchar(int c);
void api_putstr0(char *s);
void api_putstr1(char *s, int len);
void api_end(void);
int api_openwin(char *buf, int xsize, int ysize, int col_inv, char *title, int mode);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
void api_txtbox(int win, int x0, int y0, int x1, int y1, int col);
void api_initmalloc(void);
char *api_malloc(int size);
void api_free(char *addr, int size);
void api_point(int win, int x, int y, int col);
void api_refreshwin(int win, int x0, int y0, int x1, int y1);
void api_linewin(int win, int x0, int y0, int x1, int y1, int col);
void api_closewin(int win);
int api_getkey(int mode);
int api_getmouse(int win, int mode);
int api_alloctimer(void);
void api_inittimer(int timer, int data);
void api_settimer(int timer, int time);
void api_freetimer(int timer);
void api_beep(int tone);
int api_fopen(char *fname);
void api_fclose(int fhandle);
void api_fseek(int handle, int offset, int mode);
int api_fsize(int fhandle, int mode);
int api_fread(char *buf, int maxsize, int fhandle);
int api_cmdline(char *buf, int limit);
int api_getlang(void);
void api_setlang(int newmode);
