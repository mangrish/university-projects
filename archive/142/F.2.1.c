/*
** Program : pointer3.c -- Introducing pointers, arrays, addresses.
** For     : M.U. C.S.Dept;  Programming exercise.
** Written : VRJ; June 96
*/

#include <stdio.h>

#define ASIZE  10

int main( void )
{
    int a[ASIZE] = {0,5,10,20,25,30};
    int	a        = nptr;
    int *nptr    = a + 2;
    int n        = *nptr;
    
    printf( " *nptr + 3  = %d;  *(nptr + 9) = %d; a[9] = %d\n",
              *nptr  +3 , *(nptr + 9), a[9]  );

    return 0;
}

