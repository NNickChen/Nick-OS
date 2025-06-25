#include "apilib.h"

void *malloc(int size)
{
	char *p = api_malloc(size + 4);
	
	if(p != 0){
		*((int *) p) = size;
		p += 4;
	}
	return p;
}
