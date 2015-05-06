#include <stdio.h>

char grade( void);
void show_limits( char grade );
main( void )
{
   switch (grade)   {

    case 'A' :
	printf( "80 <= mark < 100\n");
	break;

    case 'B' :
	printf( "70 <= mark < 80\n");
	break;

    case 'C' :
	printf( "60 <= mark < 70\n");
	break;

    case 'D' :
	printf( "50 <= mark < 60\n");
	break;

    case 'F' :
	printf( "0 <= mark < 50\n");
	break;

    default :
	printf( "Error: Not a valid grade.\n" );
}

return ;
} 
