/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : columnate.c: 
**  For     : 433 -254: Software Design;
**            PROJECT 1
**  Written : Mark Angrish, Student number: 74592
**            April 2000;
**  Input   : From stdin: text;
**  Output  : To stdout: Columnated version of the input text, neatly formatted.
**	Questions: 				  
**	Q1.Which module have you chosen to make the main module of this program? Why?
**			the main module for this program was the input module..as it is easier to
**			implement.
**
**
**	Q2.Which input grammar rule(s) did you need to change, and why? what rules did
**	you replace it or them with? 
**			the grammer for body was changed as it had left recursion, while half
**			line needed no changing as it was implemented accordingly
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/*
Input Grammar

input:  nonprintable*, body, end
body:   paragraph+
paragraph:  word+, paragraph-terminator
word:  printable+, whitespace*, [ '\n', whitespace* ]
paragraph-terminator:  ('\n' | '\f'), nonprintable*
whitespace:  space | '\t'
nonprintable:  whitespace | '\n' | '\f'
printable:  any character that is not nonprintable and not end

Output Grammar

output:  page*, end
page:  header, line+, '\f'
header:  '\n\n\n\n\n'
line:  half-line, [ space+, half-line ], '\n'
half-line:  space+ | (word, (space, word)*, space*)
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define ROW_MAX 120
#define COLUMNMAX 32

/*GLOBAL VARIABLES*/
int c;	/*c=getchar*/
int row	=	0;	/* the row counter*/
int column	=	0;	/* the current number of spaces in each column*/
char buffer[ROW_MAX-1][COLUMNMAX+1];	/* the 2D array for storing the output*/
char tempword[COLUMNMAX+1];	/* buffer for if a word does not fit in the last*/
														/* element in the 2D array.*/

/*FUNCTION PROTOTYPES*/

void fill_buffer(void);
int getword(char *);
void print_buffer(void);
void insert_word(char *);
void blank_pad(char *);
void paragraph_terminator(char *word);



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int main(void)
/************/
{
	int i;
	c = getchar();

	while (c!=EOF && isspace(c))	/* skip leading whitespace*/
		c= getchar();

	while (c!=EOF)	/* while its not the EOF, fill the 2D array and print it*/
	{								/* one output page at a time*/
		fill_buffer();
		print_buffer();	
		row = 0;	/*reinitialise*/
		column=0;
		for (i=0;i<=ROW_MAX-1;i++)	/* reinitialise 2D array*/
		{
			strcpy(buffer[i],"");
		}
	}
	
	return 0;
}



/* fill_buffer: repeatedly gets a new word, and inserts it into the elements of 
**							the 2D array, by calling subfunctions to do so, until it is 
**							full.
*/

void fill_buffer(void)
/********************/
{
	char word[COLUMNMAX];	/* buffer to get a word from stdin*/
	
	while (c!=EOF && row<=ROW_MAX-1)
	{
		if (strlen(tempword)>0)	/*if there is a leftover word from the last output*/
		{												/*page then make it the 1st word in the new output*/
														/*page, and then reinitialise it.*/
			insert_word(tempword);
			strcpy(tempword,"");
		}

		getword(word);	/*get a word from stdin*/
		insert_word(word);	/*insert the word into the 2D array*/

		if (strlen(word)==0) /*if the length of the word is zero, than it must be*/
		{										 /* a paragraph terminator, so pad with blanks for 2*/ 
			blank_pad(word);	 /* rows increment the row, and reset the column counter*/
			row++;
			column=0;
			blank_pad(word);
			row++;
		}

		if ((COLUMNMAX - column) < 0 && strlen(word) !=0)	
		{																			/*else if the column will become over*/
			blank_pad(word);										/*flowed with the next word, than*/
			buffer[row][COLUMNMAX + 1] = '\0';  /*dont insert the word in that element*/
			column =0;													/* but make it the 1st in the next*/
			if (row == (ROW_MAX-1))							/*element of the 2D array.*/
			{																		
				strcpy(tempword,word);
			}
			row++;
			insert_word(word);	
		}
	}	

	if (c ==EOF)	/* if the EOF is encountered, than pad the remaining element*/
	{							/* of the 2D array with blanks*/
		blank_pad(word);
	}	
	return;
}



/* getword: gets a word from stdin and puts it in a word buffer, than passes the word back to
**					fill_buffer.
*/

int getword(char word[])
/**********************/
{
  int letter_count = 0;/*number of letters counted in the word so far*/
   
	ungetc(c,stdin); /*just incase we picked up a \n or \f at the end of the last word*/
  do				
	{
		c = getchar();									/*in the leading whitespace, if a \n or a \f is */
		if (c=='\n'|| c=='\f')					/*encountered than look if it is a paragraph teminator*/
		{
			paragraph_terminator(word);
			if (strlen(word)==0 )
			{
				return c;
			}
		}
		
	}while ( !ispunct(c) && !isalnum(c)  && c != EOF );

  while ( ispunct(c) ||  (isalnum( c ) && c != EOF) )/*now get the the characters that make the*/
	{																									 /*word and return it to fill_buffer*/
 	 word[ letter_count++]  = (char) c;
   c = getchar();
	} 
  
	word[ letter_count ]  =  '\0';
    
  if  ( c == '\n' || c =='\f' ) /* if the \n or \f are encountered unget the word or the next*/
		ungetc( c, stdin);					/*pass through get word*/

return c;
}


/*	paragraph-terminator: if whitespace being passed is a paragraph terminator than return the
**												word to be empty, inicating there is no word only a paragraph end
*/

void paragraph_terminator(char *word)
/***********************************/
{
			c=getchar();
			while ( !ispunct(c) && !isalnum(c)  && c != EOF ) /*during whitespace, keep getting */
			{																									/*until another \n is found*/
				c=getchar();
				if (c=='\n')
				{
					while ( !ispunct(c) && !isalnum(c)  && c != EOF )/*just skip over any other w/s*/
					{
						c=getchar();
					}
					strcpy(word,"");
					return ;
				}
			}

	return ;
}



/* insert_word: inserts the buffered word into the the 2D array if there is
**							space to do so, and inserts a space is it can.
*/

void insert_word(char word[])
/***************************/
{
  column = strlen(word) + column;	
	
  if (COLUMNMAX- column >= 0 )     
	{
		strcat( buffer[row], word );					/*copys the word into a buffer element*/
		if ((COLUMNMAX- (column + 1)) >= 0 )	/*and if an extra space will fit, it*/
		{																			/*is also inserted on the end*/
			strcat( buffer[row] , " " );
			column++;
		}
	}
	return;
}



/* blank_pad: pads the remaining buffer line with spaces. 
*/

void blank_pad(char word[])
/*************************/
{
	int i;

	for (i=(column -strlen(word)); i <= COLUMNMAX; i++)
	{													/*while the length of the column is not bigger than*/
		buffer[row][i] =' ';		/*32 than pad with blanks*/
	}
	
	return;
}



/* print_buffer: prints the formatted output page to stdout.
*/

void print_buffer( void )
/***********************/
{
	int i,j,k;
	int divider= (row/2);	/*divides the total number of lines into 2.*/
	
	printf("\n\n\n\n\n");/*the header*/
	for (i=0; i<(row/2) ;i++)/*this prints out the entire line..than adds a \n*/
	{
		for (j= 0; j<COLUMNMAX; j++)	/*prints out 1 half of a line*/
		{
			printf("%c",buffer[i][j]);
		}
		printf("        ");	/*the gutter*/
		for (k= 0; k<COLUMNMAX; k++)	/*prints out the other half of a line*/
		{
			printf("%c",buffer[divider+i][k]);
		}
		putchar('\n');
	}
	putchar('\f');	/*the footer*/
	return;
}

