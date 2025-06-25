#include "apilib.h"

void free(void *addr)
{
	char *q = addr;
	int size;
	if(q != 0){
		q -= 16;
		size = *((int *) q);
		api_free(q, size + 16);	
	}
	return;
}
