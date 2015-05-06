#include <stdio.h>


int main( void)
{
	int min, max, val;

	if ( scanf( "%d", &val) == 1) {

	  max = min = val;

	  while (scanf( "%d", &val)  == 1)
		if (val < min)
		   min = val;
		else if (val > max)
		   max = val;
}
	printf( "%d %d\n", min, max);

return 0;
}    
