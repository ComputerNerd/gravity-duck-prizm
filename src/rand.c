#include "rand.h"

static unsigned int lastrandom = 0x12345678;
	
void srand(unsigned int seed)
{
	lastrandom = seed;
}

int rand(void)
{
	lastrandom = 0x41C64E6D*lastrandom + 0x3039;
	return lastrandom >> 16;
}
