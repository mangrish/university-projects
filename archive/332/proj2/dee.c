#include <stdio.h>
#include <stdlib.h>

static  int intcompare(int *i, int *j)
{
	if (*i > *j)
		return (1);
	if (*i < *j)
		return (-1);
	return (0);
																								     }

int main()
{
	int a[10];
	int i;
	a[0] = 9;
	a[1] = 8;
	a[2] = 7;
	a[3] = 6;
	a[4] = 5;
	a[5] = 4;
	a[6] = 3;
	a[7] = 2;
	a[8] = 1;
	a[9] = 0;

	qsort((char *) a, 10, sizeof(int), intcompare);
	for (i=0; i<10; i++) printf(" %d",a[i]);
		printf("\n");
	return 0;
}

