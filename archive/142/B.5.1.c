#include <stdio.h>
#include <ctype.h>


int main()

{ int c;
  
  int alpha = 0,
      digit = 0,
      punct = 0,
      space = 0;

   while  (( c = getchar() ) != EOF )  {
      
 
      if  (isalpha( c ))
      alpha++;

      else if  (isdigit( c ) ) 
      digit++;

      else if  (ispunct( c ) )
      punct++;
 
      else if  (isspace( c ) )
      space++;

}

printf( " %d %d %d %d \n", alpha, digit, punct, space );

return 0;
}

      
