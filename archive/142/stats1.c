/*
** Program : stats2.c -- Calculates some simple statistics.
** For     : exercises 
** Written : Mark Angrish.
** Input   : From stdin: 3 integer numbers.
** Output  : To stdout : The mean, and the range, of those input values.
*/

#include <stdio.h>

int max3( int num1, int num2, int num3 );   /* Function prototype       */
int min3( int num1, int num2, int num3 );

int main( void )
/**************/
{
    int n1, n2, n3, range;                  /* Input values             */
    double mean;

    printf( "Enter 3 integer numbers: " );
    scanf( "%d %d %d", &n1, &n2, &n3 );

    mean = (double) (n1 + n2 + n3)/3;
    range = max3( n1, n2, n3 ) - min3( n1, n2, n3 );

    printf( "Mean    = %7.3f\n", mean);
    printf( "Range   = %3d\n", range);

    return 0;
}

        /*
        ** Returns the largest of three numbers.
        */
int max3( int num1, int num2, int num3 )
/**************************************/
{
    int max;

    max = num1;
    if ( num2 > max )
        max = num2;
    if ( num3 > max )
        max = num3;

    return max;
}


int min3( int num1, int num2, int num3 )
/**************************************/
{
    int min;
   
    min = num1;
    if ( num2 < min )
       min = num2;
    if ( num3 < min )
       min = num3;

    return min;
}
