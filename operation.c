#include	"bootpack.h"

unsigned int involution(unsigned int base, unsigned int exponent)
{
	unsigned int a, result = base;
	for(a = 2; a <= exponent; a ++){
		result*=base;
	}
	return result;
}