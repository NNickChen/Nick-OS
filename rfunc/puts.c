#include "apilib.h"

int puts(char *s)
{
	api_putstr0(s);
	api_putchar(0x0a);
	return 0;
}
