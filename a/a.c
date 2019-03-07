#include "apilib.h"
#include <stdio.h>

void NickStartup(void)
{
	for(;;){
		putchar('A');
	}
	api_end();
}
