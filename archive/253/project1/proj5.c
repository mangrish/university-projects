/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*
**  Program : PROJECT A: 
**  For     : 433 -253: Algorithms and data structures;
**            PROJECT A
**  Written : Mark Angrish, Student number: 74592
**            September 1999;
**  Input   : From the command line (file); From stdin: text;
**						From command line with flag (-s stopfile file).
**  Output  : To stdout: a kwik output with word in alphabetical order in 
**					  relation to the other words in the text file
*/
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#define TOTAL_STRUCT_SIZE 50000
#define MAXSIZE 200000
#define SHOWCHARS 20
#define STOPSIZE 1000
#define MAXWORD 80 
/*
**	TYPEDEFS
*/

typedef struct word_occurance Struct_element;

extern  int strcasecmp(const char *s1, const char *s2);

struct word_occurance
{
	char	*wordptr;
	int	line_counter;
};

/*
**	FUNCTION PROTOTYPES
*/

int cmpare( char *s, char *t);
char *stopgetword( char filearray[],int size, Struct_element array[], int *starti ,int *line_count, int *sizeofthis, char *newword_ptr, char *stoparray[], int stopsize );
int process_stopwords(char *stoparray[], int stop_size, Struct_element array[],int asize, char filearray[], int size);
int getstopword(char *word, int lim, FILE *fp);
int open_stop(FILE *stopfp, char *stoparray[], int size);
int cmp(Struct_element a, Struct_element b);
void print_array(Struct_element array[], int asize, char filearray[],int tsize);
int copy_in(FILE *fp, char filearray[]);
int process_words( Struct_element array[], int asize , char filearray[], int size);
char *getword( char filename[], int size,Struct_element arrau[] , int *starti , int *line_count,int *sizeofthis, char *nextword_ptr); 
void usage( char *progname );
FILE *open_file( char *progname, char *fname, const char *mode );


int main( int argc, char *argv[] )
/********************************/
{
	FILE *fp=NULL;
	FILE *stopfp=NULL;
	char filearray[MAXSIZE];
	int size, asize;
	Struct_element array[TOTAL_STRUCT_SIZE];
	/*char *stoparray[STOPSIZE];*/

	if ( argc == 2)
	{
		fp = open_file( argv[0], argv[1], "r" );
		size = copy_in(fp,filearray);
		asize = process_words( array, TOTAL_STRUCT_SIZE, filearray ,size);
		qsort(array,asize-1, sizeof(Struct_element),(int (*) (const void *, const void *))cmp);
		print_array(array,asize,filearray,size);
	}	

	else if ( (argc == 4) && (strcmp(argv[1],"-s")==0) )
	{
		stopfp = open_file( argv[0], argv[2], "r" );	
		fp = open_file( argv[0], argv[3], "r" );
/*		size = copy_in(fp, filearray);
 		stopsize = open_stop(stopfp, stoparray, STOPSIZE);
		asize = process_stopword(stoparray,stopsize, array, TOTAL_STRUCT_SIZE, filearray,size);
    qsort(array,asize-1, sizeof(Struct_element),(int (*) (const void *, const void *))cmp);
		print_array(array,asize,filearray,size);
		*/
	}

	else if (argc == 1 )
	{
		fp=stdin;
		size = copy_in(fp, filearray);
		asize = process_words( array, TOTAL_STRUCT_SIZE , filearray,size);
		qsort(array,asize-1, sizeof(Struct_element),(int (*) (const void *, const void *))cmp);
		print_array(array,asize,filearray,size);
	}

	else 
		usage(argv[0]);

	fclose(fp);
	fclose(stopfp);

	return 0;
}

/*
**	usage:	if not the right command line arguement given, display usage
*/

void usage( char *progname )
/**************************/
{
	fprintf( stderr, "Usage: %s (stdin) | filename \n", progname );

	exit( EXIT_FAILURE );
}

/*
**	open_file:	Open "fname" in the read mode.  If "fname" cant be opened, 
**	report this, and exit fail.
*/

FILE *open_file( char *progname, char *fname, const char *mode )
/**************************************************************/
{
	FILE *fp;

	fp = fopen( fname, mode );

	if (fp == NULL )
	{
		fprintf( stderr, "%s: ERROR: File \"%s\" not opened;  "
							"program execution terminated.\n", progname, fname );

		exit( EXIT_FAILURE );
	}

	return fp;
}

/*
**	copy_in:	copys the input from the where the fp is pointing to, to an array
**						of char(filearray).
*/

int copy_in( FILE *fp, char filearray[])
/**************************************/
{
	int i;
	int c;

	for (i=0;((c= getc(fp))!=EOF);i++) 
	{
			filearray[i]= c;
	}

	filearray[i]='\0';
	
	return i;
}

/*
**	process_words:	repeatedly calls the getword function and also holds the 
**	values of the line_count and returns the total size of the array of structs
**	being built(array).
*/

int process_words( Struct_element array[], int asize, char filearray[],int size )
/*******************************************************************************/

{
    int line_count = 1;
		int sizeofthis = 0;
		int starti=0;
		int total=0;
		char *newword_ptr;
		newword_ptr = filearray;

    while (( (newword_ptr= getword(filearray,size, array, &starti, &line_count, &sizeofthis,newword_ptr)) != '\0'  ))
    {
			 total++;
			 if (starti==size)
			 break;
    }

    return total;
}

/*
**	getword:	finds the start of a new word and tells the Struct_element.wordpt
**	to point to it, whilst also assigning the line number it appeared on.
*/

char *getword( char filearray[],int size, Struct_element array[], int *starti , int *line_count, int *sizeofthis, char *newword_ptr )
/***********************************************************************************************************************************/
{
   int c,i;

	for (i=(*starti); i<size; i++, (*starti)++)
	{
   if ( !isalnum( c = filearray[i] ) && (c != '\0')  )
   {
    if  ( c == '\n' )
		{ 
			filearray[i]=' ';
			(*line_count)++;
		}

		else if ( !isprint(c) )
			filearray[i]=' ';

		newword_ptr++;
		
   }

   if ( isalnum( c ) && (c != '\0') )
   {
			array[*sizeofthis].wordptr=(newword_ptr);
			array[*sizeofthis].line_counter=(*line_count);
			(*sizeofthis)++;
			break;
   }
	}

	while (isalnum(c=(filearray[i])) && (c !='\0'))
	{
		newword_ptr++;
		i++;
		(*starti)++;
	}

  return newword_ptr;
}

/*
**	cmp: the cmp function for qsort, uses lib function strcasecmp to compare.
*/

int cmp( Struct_element a, Struct_element b)
/******************************************/
{
	return strcasecmp( a.wordptr, b.wordptr);
}

/*
**	print_array:	prints the output as in the project specs, 	
*/

void print_array(Struct_element array[], int asize, char filearray[],int tsize)
/*****************************************************************************/
{
	int i,j,k;
	
	for (i=0; i<(asize-1) ; i++)
	{
		printf("%d:", array[i].line_counter);
	  
		for (j=SHOWCHARS; j>=1; j--)
		{
			if ( &*(array[i].wordptr - j) < &filearray[0] )
					printf (">");
			else
				printf("%c", *(array[i].wordptr - j));
		}
		
		printf("*");
		
		for (k=0; k<=SHOWCHARS-1; k++)
		{
			if (&*(array[i].wordptr +k) > &filearray[tsize] )
				printf ("<");
			else
				printf("%c", *(array[i].wordptr +k) );
		}
		
		if (&*(array[i].wordptr + k) > &filearray[tsize] )
			printf ("<");

		printf("\n");
	}
	return;
}


int open_stop(FILE *stopfp, char *stoparray[], int size)
/******************************************************/
{
	int stopsize = 0;
	char word[MAXWORD];
	int i=0;
	int c;

	while ((c=getstopword(word, MAXWORD, stopfp)) != EOF )
	{
		stoparray[i] = word;
		stopsize++;
		i++;
	}
	return stopsize;
}

int getstopword(char *word, int lim, FILE *fp)
/********************************************/
{
	int c;
	int letter_count = 0;

	while (!isalnum(c =getc(fp)) && c!= EOF)
		;
	while (isalpha(c) && c !=EOF)
	{
		if (letter_count >= lim)
		{
			word[letter_count] ='\0';
			return c;
		}
		word[letter_count++] = (char) c;
		c = getc(fp);
	}
	word[letter_count] = '\0';

	return c;
}
/*
int process_stopwords(char *stoparray[], int stopsize, Struct_element array[], int asize, char filearray[], int size)
{
  int line_count = 1;
  int sizeofthis = 0;
  int starti=0;
  int total=0;
  char *newword_ptr;
  newword_ptr = filearray;

  while ( (newword_ptr= stopgetword(filearray,size, array, &starti, &line_count,&sizeofthis, newword_ptr, stoparray, stopsize)) != '\0' )
  {
    total++;
    if (starti==size)
    break;
  }
	return total;
}

char *stopgetword( char filearray[],int size, Struct_element array[], int *starti ,int *line_count, int *sizeofthis, char *newword_ptr,char *stoparray[], int stopsize )
{
	int c,i;
	char *val;

  for (i=(*starti); i<size; i++, (*starti)++)
  {
    if ( !isalnum( c = filearray[i] ) && (c != '\0')  )
	  {
			if  ( c == '\n' )
 	    {
		    filearray[i]=' ';
		    (*line_count)++;
			}

		  else if ( !isprint(c) )
		     filearray[i]=' ';

		  newword_ptr++;

		}

  	if ( isalnum( c ) && (c != '\0') )
  	{
			if ( (val = bsearch(newword_ptr, stoparray, stopsize,sizeof(char),(int (*) (const void *, const void *))cmpare)) != NULL )
				break;
			
			else	
			{
				array[*sizeofthis].wordptr=(newword_ptr);
     		array[*sizeofthis].line_counter=(*line_count);
     		(*sizeofthis)++;
				break;
			}															   
		}
  }

  while (isalnum(c=(filearray[i])) && (c !='\0'))
  {
    newword_ptr++;
    i++;
   (*starti)++;
	}

  return newword_ptr;
}

int cmpare( char **s, char **t)
{
	for (; tolower (*s) == tolower(*t) ; s++, t++ )
		if (*s == '/0')
			return 0;
	return tolower(*s) - tolower(*t);
}
*/
