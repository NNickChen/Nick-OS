#include "apilib.h"
#include <stdio.h>

int puts(char *s)
{
	api_putstr0(s);
	return 0;
}
