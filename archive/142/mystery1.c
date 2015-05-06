/*
** Program : mystery1.c -- Illustrates recursion, where the recursive
**              call is to `main()'.
** Written : Jan 97 -- Based on Deitel & Deitel, Ex. 5.46.
** Input   : Text read from standard input (no prompts for input).
** Output  : To be evaluated by students.
*/

#include <stdio.h>

int main( void )
{
    int c;

    if ( ( c = getchar() ) != EOF ) {
        printf( "%c", c );
	main();
    }

    return 0;
}

