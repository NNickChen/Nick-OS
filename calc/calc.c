#include "apilib.h"
#include <stdio.h>		/* sprintf */
#include <stdlib.h>		/* strtol */

#define INVALID		-0x7fffffff /* 出错码 */

char *skipspace(char *p);
int getnum(char **pp, int priority);
void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen); /* 绘制按钮 */

void NickStartup(void)
{
	int win, i, c, d, e, f, h, j, result, k;
	char winbuf[150 * 200], calcline[150], s[15], *p, *g;
	
	win = api_openwin(winbuf, 150, 200, 255, "calc", 1);
	make_button8(win, 120, 80, 140, 120, "C", 0, 1);
restart:
	api_txtbox(win, 8, 8, 133, 20, 255);
	api_boxfilwin(win, 10, 34, 140, 52, 8);
	for(c = 0; c < 150; c++){
		calcline[c] = ' ';
	}
	c = 0;
	d = 0;
	e = 0;
	h = 0;
	j = 0;
	i = 0;
	k = 0;
	api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, "0");
	
	/* 获取算式 */
	for(;;){
		api_boxfilwin(win, 10, 50, 140, 66, COL8_C6C6C6);
		api_putstrwin(win, 10, 50, 0, 15, calcline);
		k = api_getkey(0);
		if(k != -1){
			i = k;
		} else {
			i = -1;
		}
		if(h == 1){
			h = 0;
			if(i == '0'){
				api_boxfilwin(win, 10, 50, 130, 70, COL8_C6C6C6);
				i = -1;
				goto restart;
			}
		}
		if(i == 251){
			api_end();
		} else if('0' <= i && i <= '9' && c < 150){
			if(j == 1){
				d = c;
				e = 0;
				api_boxfilwin(win, 10, 10, 140, 26, 255);
				j = 0;
			}
			calcline[c] = i;
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if(i == 'x' && c < 150){
			calcline[c] = i;
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if(i == '(' && c < 150){
			if(j == 1){
				d = c;
				e = 0;
				api_boxfilwin(win, 10, 10, 140, 26, 255);
				j = 0;
			}
			calcline[c] = i;
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if(i == ')' && c < 150){
			calcline[c] = i;
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if('a' <= i && i <= 'f' && c < 150){
			calcline[c] = i;
			c++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if('A' <= i && i <= 'F' && c < 150){
			calcline[c] = i;
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if(i == '+' || i == '-' || i == '*' || i == '/' || i =='|' || i == '&' || i == '>' || i == '<' || i == '^' || i == '%'){
			calcline[c] = i;
			j = 1;
			c++;
		} else if(i == 0x5c && c < 150){
			if(j == 1){
				d = c;
				e = 0;
				api_boxfilwin(win, 10, 10, 140, 26, 255);
				j = 0;
			}
			calcline[c] = '-';
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if(i == '~' && c < 150){
			if(j == 1){
				d = c;
				e = 0;
				api_boxfilwin(win, 10, 10, 140, 26, 255);
				j = 0;
			}
			calcline[c] = '~';
			c ++;
			api_boxfilwin(win, 10, 10, 140, 26, 255);
			if(d < 15){
				api_putstrwin(win, 140 - (8 * (e + 1)), 10, 7, 10, calcline + d);
				e++;
			} 
		} else if(i == '='){
			p = calcline;
			result = getnum(&p, 9);
			if(result != INVALID){
				api_boxfilwin(win, 10, 10, 140, 26, 255);
				sprintf(s, "%d", result);
				f = 0;
				for(g = s; *g != 0x00; g++){
					f++;
				}
				api_putstrwin(win, 140 - (8 * f), 10, 7, 30, s);
				for(c = 0; c < 150; c++){
					calcline[c] = ' ';
				}
				c = 0;
				for(g = s; *g != 0x00; g++){
					calcline[c] = *g;
					c++;
				}
				c = f;
				sprintf(s, "= 0x%x", result);
				f = 0;
				for(g = s; *g != 0x00; g++){
					f++;
				}
				api_boxfilwin(win, 10, 35, 145, 35 + 14, COL8_C6C6C6);
				api_putstrwin(win, 140 - (8 * f), 35, 0, 30, s);
			}
			h = 1;
		}
	}
}


char *skipspace(char *p)
{
	for (; *p == ' '; p++) { }	
	return p;
}

int getnum(char **pp, int priority)
{
	char *p = *pp;
	int i = INVALID, j;
	p = skipspace(p);

	
	if (*p == '+') {
		p = skipspace(p + 1);
		i = getnum(&p, 0);
	} else if (*p == '-') {
		p = skipspace(p + 1);
		i = getnum(&p, 0);
		if (i != INVALID) {
			i = - i;
		}
	} else if (*p == '~') {
		p = skipspace(p + 1);
		i = getnum(&p, 0);
		if (i != INVALID) {
			i = ~i;
		}
	} else if (*p == '(') {	
		p = skipspace(p + 1);
		i = getnum(&p, 9);
		if (*p == ')') {
			p = skipspace(p + 1);
		} else {
			i = INVALID;
		}
	} else if ('0' <= *p && *p <= '9') { 
		i = strtol(p, &p, 0);
	} else { 
		i = INVALID;
	}

	
	for (;;) {
		if (i == INVALID) {
			break;
		}
		p = skipspace(p);
		if (*p == '+' && priority > 2) {
			p = skipspace(p + 1);
			j = getnum(&p, 2);
			if (j != INVALID) {
				i += j;
			} else {
				i = INVALID;
			}
		} else if (*p == '-' && priority > 2) {
			p = skipspace(p + 1);
			j = getnum(&p, 2);
			if (j != INVALID) {
				i -= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '*' && priority > 1) {
			p = skipspace(p + 1);
			j = getnum(&p, 1);
			if (j != INVALID) {
				i *= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '/' && priority > 1) {
			p = skipspace(p + 1);
			j = getnum(&p, 1);
			if (j != INVALID && j != 0) {
				i /= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '%' && priority > 1) {
			p = skipspace(p + 1);
			j = getnum(&p, 1);
			if (j != INVALID && j != 0) {
				i %= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '<' && p[1] == '<' && priority > 3) {
			p = skipspace(p + 2);
			j = getnum(&p, 3);
			if (j != INVALID && j != 0) {
				i <<= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '>' && p[1] == '>' && priority > 3) {
			p = skipspace(p + 2);
			j = getnum(&p, 3);
			if (j != INVALID && j != 0) {
				i >>= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '&' && priority > 4) {
			p = skipspace(p + 1);
			j = getnum(&p, 4);
			if (j != INVALID) {
				i &= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '^' && priority > 5) {
			p = skipspace(p + 1);
			j = getnum(&p, 5);
			if (j != INVALID) {
				i ^= j;
			} else {
				i = INVALID;
			}
		} else if (*p == '|' && priority > 6) {
			p = skipspace(p + 1);
			j = getnum(&p, 6);
			if (j != INVALID) {
				i |= j;
			} else {
				i = INVALID;
			}
		} else {
			break;
		}
	}
	p = skipspace(p);
	*pp = p;
	return i;
}

void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen) /* 绘制按钮 */
{
	if(act == 0){
		api_boxfilwin(win, x0 - 1, y0 - 1, x1 + 1, y1 + 1, COL8_848484);
		api_boxfilwin(win, x0 - 1, y0 - 1, x1, y1, COL8_FFFFFF);
		api_boxfilwin(win, x0, y0, x1, y1, COL8_C6C6C6);
	} else {
		api_boxfilwin(win, x0 - 1, y0 - 1, x1, y1, COL8_848484);
		api_boxfilwin(win, x0, y0, x1 + 1, y1 + 1, COL8_FFFFFF);
		api_boxfilwin(win, x0, y0, x1, y1, COL8_C6C6C6);
	}
	api_putstrwin(win, x0 + 5, y0 + 10, 0, strlen, s);
	return;
}