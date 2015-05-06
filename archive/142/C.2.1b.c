#include <stdio.h>


void get_size2( int * );
void print_output( int size );


int main(void)
{
    int size;

    get_size2(&size);
    print_output( size );

    return 0;
}


void get_size2( int *size )
{

    printf( " Enter size:  " ) ;
    scanf( "%d", size );
    printf( "\n");

    return ;

}

void print_output( int size )
{
    
    int row,col;

    for (row = 1; row <= size ;row++) 
    {
	for (col = 1; col <= row ; col ++)
	{
	 printf( "*" );
	}
    printf( "\n" );
    }
    
return ;
}

