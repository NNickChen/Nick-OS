#include "apilib.h"

void free(void *addr)
{
	char *q = addr;
	int size;
	if(q != 0){
		q -= 4;
		size = *((int *) q);
		api_free(q, size + 4);	
	}
	return;
}
