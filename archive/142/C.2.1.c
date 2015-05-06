#include <stdio.h>


int get_size( void );
void print_output( int size );


int main(void)
{
    int size;

    size = get_size();
    print_output( size );

    return 0;
}


int get_size( void )
{
    int s;

    printf( " Enter size:  " ) ;
    scanf( "%d", &s );
    printf( "\n");

    return s;

}

void print_output( int size )
{
    
    int row,col;

    row = 1;

    while ( row <= size ) {
	col = 1;
	while ( col <= row )
	{
	 printf( "*" );
	 col ++;
	}
    printf( "\n" );
    row++;
}
    return ;
}

