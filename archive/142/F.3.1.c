#include <stdio.h>

#define  VSIZE 100

int getvals( int *, int );
void printvals( int *, int );


int main( void )
{
   int values[ VSIZE ];
   int total = 0;

   total = getvals( values, VSIZE );
   printvals( values, total );
 
   return 0;
}


int getvals( int values[], int  vsize)
{
   int i = 0,
       c ;

      while ((c = scanf( "%d", &values[i] ) == 1) && c != EOF )
        i++;

   return i;
}

void printvals( int array[], int totsize )
{
   
   int j;

   for (j = totsize; j ==  0; j-- )
     printf( " %d", array[j]);

   return;
}
 
