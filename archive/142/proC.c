/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*
**  Program : PROJECT C: to prepare a cross-referncing file, to produce for an
**                       input text document, a list of the line numbers on
**                       which each word occurs in that document, using linked
**			 lists, and binary search trees.
**  For     : 433 -142: Computing fundamentals B;
**            PROJECT C
**  Written : Mark Angrish, Student number: 74592
**            February 1999;
**  Input   : From stdin: any text file;
**  Output  : To stdout: output of a BST of pointers to structs in alphabetical
**	      order, with no restrictions on the amount of words or the number
**	      of line numbers encountered.
**  Note    : The list of statistics was not included in project C. 
*/
/*---------------------------------------------------------------------------*/
/*****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

/*
**	TYPE DEFINITIONS
*/

typedef struct Structure             Data;
typedef struct tnode                 Tnode;
typedef struct lnode                 Lnode;

/*
**	STRUCTURE DECLARATIONS
*/

struct Structure {
    char    *word;
    Lnode *lineptr;
} ;

struct lnode {
    int line_number;
    Lnode *next;
};

struct tnode {
    Tnode *tleft;
    Data  *record;
    Tnode *tright;
} ;

/*
**	FUNCTION PROTOTYPES
*/

Tnode *make_node( char * , int );
Tnode *addtree(Tnode *, Tnode *,  int  );
Lnode *make_lnode( int  );
void treeprint( Tnode *);
int getword(char *, int, int *);
int lower_cmp(char *, char *);
Lnode *addlist( Lnode *, Lnode *  );
void free_treemem( Tnode * );
Tnode *caller( void );

/*****************************************************************************/

int main( void )
/**************/
{
   Tnode *troot;

   troot = caller();
   treeprint(troot);
   free_treemem(troot);

   return 0;
}


/*
**  caller  : calls make_node and addtree, so long as getword does not = EOF.
**	      It returns the  tree.
*/


Tnode *caller( void )
/*******************/
{
   Tnode *root = NULL;
   Tnode *newn = NULL; 
   char word[MAXWORD];
   int line_count = 1;

   while (getword( word, MAXWORD, &line_count) != EOF)
     {
	newn = make_node(word, line_count); 
	root = addtree(root, newn, line_count);
     }

   return root;
}

/*
** get_word  : gets a char from standard input repeatedly until a nonalpha
**             character is encountered and stores it in a buffer array of char.
**             It also counts the line a word appeared on using pointers.
*/

int getword( char *word , int size , int *line_count)
/***************************************************/
{
   int c;
   int letter_count = 0;

   while ( !isalpha( c = getchar() ) && c != EOF )
     {
	if  ( c == '\n' )
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
**  make_node  :  make new tree node to point to struct. it allocates memory
**		  for the new node itself, the actual size of the word, and
**		  calls make_lnode to allocated memory for the linked list of 
**		  line numbers.  It terminates the program if mem not allocated. 
*/


Tnode *make_node( char *w , int line_count)
/*****************************************/
{
   Tnode *newn;

   newn = malloc( sizeof( Tnode) );
   newn->record = malloc( sizeof( Data ));
 
   if ( newn == NULL)
     {
	fprintf( stderr, "Failure: Memory not allocaed for data item.\n");
	exit(EXIT_FAILURE);
     } 

   newn->tleft = NULL;

   if (strlen( w ) > 50 )
     newn->record->word  = (char *) malloc( 51 * sizeof( char));
   else
     newn->record->word  = (char *) malloc( (strlen(w) + 1) * (sizeof( char)) );
   
   if (newn->record->word != NULL)
    {
     if (strlen( w ) > 50 )
       {
         w[50] = '\0'; 
         strcpy(newn->record->word, w);
       }
     else
       strcpy(newn->record->word, w);
    }
   newn->record->lineptr = make_lnode( line_count ); 
   newn->tright = NULL;
 
   return newn;
}


/*
**  make_lnode  : makes a new node for the linked list of line numbers.
**		  It returns the address of the new node, and if memory
**		  is not allocated, exit-fail.
*/

Lnode *make_lnode( int line_count )
/*********************************/
{
   Lnode *newln;

   newln = malloc(sizeof(Lnode) );

   if ( newln == NULL)
     { 
	fprintf( stderr, "Failure: Memory not allocaed for data item.\n");
	exit(EXIT_FAILURE);
     }

   newln->line_number = line_count;
   newln->next = NULL;

   return newln;
}


/*
**  addtree  : insert a new treenode into its proper place in the BST.
**	       Duplicate values are found and their linked list of line numbers
**	       is added on to the end.  This is done recursivly.
*/

Tnode *addtree(Tnode *root, Tnode *newn, int line_count)
/******************************************************/
{
   int cond;

   if (root == NULL )
     root = newn; 
 
   else if ((cond = lower_cmp(newn->record->word, root->record->word)) == 0)
     {
	if ((strcmp(newn->record->word, root->record->word)) == 0 )	
	   addlist( root->record->lineptr, newn->record->lineptr);
     }

   else if (cond < 0 )
	root->tleft = addtree(root->tleft, newn, line_count);

   else
	root->tright = addtree(root->tright, newn, line_count);

   return root;
}


/*
**  addlist  : each new line number is linked in to the end of the so-far-built
**	       list.  This also happens to be in sorted order.  This function
**	       returns the head of the completed list.
*/

Lnode *addlist( Lnode *root, Lnode *newn )
/******************************/
{
   Lnode *head, *last;

   head = last = root;

   while  (last->next != NULL )
      last = last->next;

   last->next = newn;

   return head;
}


/*
** lower_cmp: exactly the same function as strcmp, but strings are compared in
**            lower case.
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
**  treeprint  : Print contents of the binary tree, using an in-order traverse
*/

void treeprint( Tnode *p)
/******************************/
{
   int n = 0;

   if ( p != NULL )
     {
	treeprint(p->tleft);
        printf(" %-50s :",  p->record->word );
        while (p->record->lineptr != NULL) 
	   { 
	     if ((n % 10 ) == 0 && n != 0 )
		printf("\n%54c%4d", ' ',p->record->lineptr->line_number );
             else 
		printf( " %4d", p->record->lineptr->line_number);
	     n++;
	    p->record->lineptr =  p->record->lineptr->next;
           }
        putchar( '\n');
        treeprint( p->tright);
     }
   return;
}


/*
**  free_treemem:  Free the memory allocated for each node in the BST.  It uses
**		   a post_order traversal to first free the linked list of 
**		   line numbers, than the actual word itself, followed by 
**		   freeing the actual node itself.
*/

void free_treemem( Tnode *tptr )
/******************************/
{
   Lnode *tmp;

   if ( tptr != NULL )
     {
	free_treemem(tptr->tleft);
	free_treemem(tptr->tright);
	for (; tptr->record->lineptr != NULL ; tptr->record->lineptr = tmp )
	   {
	     tmp = tptr->record->lineptr->next;
	     free( tptr->record->lineptr );
	   }
	free(tptr->record->word);
	free( tptr );
     }
   return;
}
