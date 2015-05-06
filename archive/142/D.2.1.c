/*
** Program : breakfast.c
** For     : M.U. C.S.Dept; 433--142 Programming exercise
** Written : VRJ
**
** A program to un-scramble.  It DOES compile and run!
*/

#include <stdio.h>

#define NOTVALID 1

int main(void)
{
    double num;
    int counter = 0;

    while(scanf("%lf",&num) == 1)
	if (num != NOTVALID)
        ++counter;

return 0;
}

