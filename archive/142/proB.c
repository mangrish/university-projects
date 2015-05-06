/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*
**  Program : PROJECT B: to prepare a cross-referncing file, to produce for an
**			 input text document, a list of the line numbers on 
**			 which each word occurs in that document. 
**  For     : 433 -142: Computing fundamentals B;
**            PROJECT B 
**  Written : Mark Angrish, Student number: 74592
**            January 1999;
**  Input   : From stdin: any text file;
**  Output  : To stdout: output of an array of structs in alphabetical order
**            with up to 100 line numbers ( 10 per line of output) and a set
**	      of statistics.
**  Note    : An inefficient but nonetheless effective sorting device was used
**	      Please have mercy on this students sole when marking!!!!!!!!!!!
*/
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define OCCURANCE_ARRAY_SIZE 100
#define WORD_CHAR_SIZE 20
#define TOTAL_STRUCT_SIZE 1500
#define BUFFER_SIZE 100

/*
**  TYPEDEFS
*/
 
typedef int			Occurance;
typedef struct word_occurance	Struct_element;


struct word_occurance 
{
char		word[ WORD_CHAR_SIZE ];
int		total_counter;
Occurance	array_lines[ OCCURANCE_ARRAY_SIZE ];
};

/*
** FUNCTION PROTOTYPES
*/

void process_words( Struct_element *, int );
int getword( char * , int , int * );
void print_array( Struct_element * , int );
void update( Struct_element *, int , char *, int );
void optional( Struct_element *, int );
void bubblesort( Struct_element *, int );
int lower_cmp(char *, char *);

/*****************************************************************************/

int main( void )
/**************/

{
    Struct_element array[ TOTAL_STRUCT_SIZE ];

    process_words( array, TOTAL_STRUCT_SIZE );
    bubblesort( array, TOTAL_STRUCT_SIZE);
    print_array( array, TOTAL_STRUCT_SIZE); 
    optional( array, TOTAL_STRUCT_SIZE);
    
    return 0;

}

/*
**process_word  : repeatedly calls getword until it encounters the EOF, and
**		  trucates the buffer if it contains more than 20 chars.
**		  it then calls update to add the word appropriately into the
**		  array.
*/

void process_words( Struct_element array[], int asize )
/*****************************************************/

{
    char buffer[ BUFFER_SIZE];
    int line_count = 1;

    while (( getword(buffer, WORD_CHAR_SIZE, &line_count) != EOF  ))
	{       
	    if (strlen( buffer ) > 20 )     
		    strncpy( buffer , buffer, WORD_CHAR_SIZE );
		    buffer[20] = '\0';

	   update( array, asize, buffer,  line_count );
	}
    return;
}

/*
** get_word  : gets a char from standard input repeatedly until a nonalpha
**	       character is encountered and stores it in a buffer array of char.
**	       It also counts the line a word appeared on using pointers.
*/

int getword( char word[] , int size , int *line_count)
/****************************************************/

{
    int c;
    int letter_count = 0;
    
    while ( !isalpha( c = getchar() ) && c != EOF )
	{
	  if ( c == '\n')
	    (*line_count)++;
	}

    while ( isalpha( c ) && c != EOF )
	{
	  word[ letter_count++]  = (char) c;
	    c = getchar();
	} 

    word[ letter_count ]  =  '\0';
    
    if  ( c == '\n' ) 
	ungetc( c, stdin);

   return c;
}

/*
** update  : workhorse function that updates the building array of structs
**	     by using various comparisons, and also terminates if the input
**	     given is larger than the array can hold.
*/

void update( Struct_element array[], int asize, char word[], int lc)
/******************************************************************/

{    
    int i;

    for ( i = 0; i < (TOTAL_STRUCT_SIZE - 1 ) && ( array[i].total_counter != 0 ); i++ )
	{
	  if (i > TOTAL_STRUCT_SIZE -1)
	    {
		fprintf( stderr,
			 "Memory allocation failed; program terminated.\n");
		exit( EXIT_FAILURE );
	    } 

	  if (strcmp(array[i].word , word) == 0) 
	    {
		if ( array[i].total_counter > OCCURANCE_ARRAY_SIZE )
		  {
		    fprintf( stderr,
			    "Memory allocation failed; program terminated.\n");
			    exit( EXIT_FAILURE );
		  }

		else if (array[i].array_lines[ (array[i].total_counter) - 1] == lc)
		  return;
             
		else  
		  array[i].array_lines[ (array[i].total_counter) ] = lc;
		  array[i].total_counter++;
		  return ;
	    }
	}
    strcpy( array[i].word , word);
    array[i].total_counter++;
    array[i].array_lines[0] = lc;

    return ;
}

/*
** bubblesort  : sorts the items into the specified alphbetical order, by means
**		 of a series of comparisons.  This sorting algorithm is 
**		 extremely inefficient, but absolutely simple to code.
*/

void bubblesort( Struct_element array[], int asize)
/***************************************************/

{
    int pass, i;
    Struct_element hold;

    for (pass = 1; (pass <= asize -1) && ( array[pass].total_counter != 0 ); pass++)
	{
	  for (i = 0; (i <= asize - 2)&&(array[i + 1].total_counter != 0); i++)
	    {
		if (lower_cmp(array[i].word,array[i+1].word ) > 0)
		  {
		    hold = array[i];
		    array[i] = array[ i+1];
		    array[i+1] = hold;
		  }
		
		if (lower_cmp(array[i].word, array[i+1].word ) == 0)
		  { 
		    if (strcmp(array[i].word, array[i+1].word ) > 0)
			{
			  hold = array[i];
                	  array[i] = array[ i+1];
                	  array[i+1] = hold;
    	   		}
		  } 
	    }
	}    
    return;
}

/*
** lower_cmp: exactly the same function as strcmp, but strings are compared in
**	      lower case.
*/

int lower_cmp(char *s, char *t)
/*****************************/
{
    for (; tolower(*s) == tolower(*t) ; s++, t++ )
	if (*s == '\0')
	  return 0;
    return tolower(*s) - tolower(*t);
}

/*
** print_array  : outputs the array of structs in the way decribed in the header
*/

void print_array( Struct_element array[] , int asize)
/***************************************************/

{
    int i,j;
    
    for (i = 0 ; i < (TOTAL_STRUCT_SIZE - 1)  && ( array[i].total_counter != 0 ); i++ )
	{
	  printf( "%-20s : ", array[i].word);

	  for ( j = 0; j < (TOTAL_STRUCT_SIZE - 1) && ( array[i].array_lines[j] != 0) ; j++ )
	    {
		if  (( j % 10) == 0 && j !=0)
		  printf( "\n%23c %4d",' ',array[i].array_lines[j]);
		else 
		  printf( " %4d",array[i].array_lines[j]);
	    }
   
	  putchar('\n');
	}
    putchar('\n');

    return;
}

/*
** optional  : prints out a set of statistics about the outputed data.
*/

void optional( Struct_element array[], int asize )
/************************************************/

{
    int count = 0, word = 0, count_lines = 0, word_length = 0, cross_refs = 0;
    int i;
    char max_word[WORD_CHAR_SIZE],
         ref_word[WORD_CHAR_SIZE];


    for (i = 0 ; (i < asize - 1)  && ( array[i].total_counter != 0 ); i++ )
	{
	  count++;
	  word += strlen( array[i].word);
	  count_lines += array[i].total_counter;
	  if (strlen( array[i].word) > word_length )
	    {
		word_length = strlen(array[i].word);
		strcpy(max_word, array[i].word);
            }
	 if (array[i].total_counter > cross_refs )
	    {
		cross_refs = array[i].total_counter;
		strcpy(ref_word, array[i].word);
	    }
 	}

    printf( "Max word size: %d\n" , word_length );
    printf( "First word of this size: %s\n", max_word );
    printf( "Average word size: %.2f\n", ((double) word / count) );
    printf( "Maximum number of X-references: %d\n", cross_refs );
    printf( "First word with this number of X-refs: %s\n", ref_word  );
    printf( "Average number of X-refs: %.2f\n\n",((double) count_lines/ count));
    printf( "Memory provided for entries: %d bytes\n", sizeof( Struct_element) * TOTAL_STRUCT_SIZE  );
   /* printf( "Memory actually used for data: %d\n",  );
    printf( "%% of provided memory used: %d%%\n", (sizeof(Struct_element) * TOTAL_STRUCT_SIZE) );
*/
    return;
}
