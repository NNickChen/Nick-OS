#include "apilib.h"
#include <stdio.h>

int strtol(char *s, char **endp, int base);

void HariMain(void)
{
	char s[8], cmdline[60], *p;
	int max;
	api_cmdline(cmdline, 60);
	
	for(p = cmdline; *p > ' '; p++){ }
	for(; *p == ' '; p++){ }
	max = strtol(p, 0, 10);
	if(max >= 1000000){
		api_putstr0("The limit is to big\n");
		api_end();
	}
	char flag[max];
	int i, j;
	for(i = 0; i < max; i++){
		flag[i] = 0;
	}
	for(i = 2; i < max; i++){
		if(flag[i] == 0){
			sprintf(s, "%d ", i);
			api_putstr0(s);
			for(j = 2 * i; j < max; j += i){
				flag[j] = 1;
			}
		}
	}
	api_putchar(0x0a);
	api_end();
}
