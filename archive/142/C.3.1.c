#include <stdio.h>


int factorial( int );

int main(void)
{
    int num, done;

    printf( "Enter value: \n");
    scanf( "%d", &num);

    done = factorial( num );
   
    printf( "%d factorial is: %d\n", num, done );
   
    return 0;
}

int factorial( int n )
{

  if (n >= 1)
    return  n * factorial( n -1 ); 

   else
    return 1; 
    
    
}

