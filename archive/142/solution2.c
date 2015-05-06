/*
** Program : projcbst.c 
** For     : 433--142 Project B, Summer Semester, 1998-99
** Written : VRJ; 23 Sep 1997
**
** See Project Specification for full details and requirements.
**
** BST Version.
**
** Input   : From file named in command-line: Text whose words
**             are to be cross-referenced.
** Output  : To a named file: Cross-reference listing of all words.
**
** This program is based on `projc1.c', but uses a BST as the
**   main data structure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

                                          /* Symbolic constants  */
#define   WORDBUFFSIZE  500
#define   NAME_ASIZE     20               /* For output filename */
#define   EODATA          0               /* Flags               */
#define   NEXTDATA        1

#define   NCOLS          10


typedef struct ln_node    LNnode;
typedef struct entry_node Enode;

struct ln_node {
    int     lnum;
    LNnode *next;
} ;

struct entry_node {
    Enode  *ltree;
    char   *word;
    LNnode *ln_head;
    LNnode *ln_last;
    Enode  *rtree;
} ;

void    usage( char *progname );
FILE   *open_file( char *filename, const char *mode );
Enode  *build_references( FILE *ifp, Enode *root, 
                          char *wordbuff, int buffsize );
Enode  *bst_insert( Enode *root, char *wordbuff, int linenum );
int     getword( FILE *ifp, char *wordbuff, int buffsize, int *linenum );

char   *make_wordnode( char *wordbuff );
LNnode *make_lnnode( int linenum );
Enode  *make_entrynode( char *word, int linenum );


FILE   *get_outfile( void );
void    print_references( FILE *ofp, Enode *root );
void    print_one_entry( FILE *ofp, Enode *curr );

void    print_stats( FILE *ofp, Enode *root );
int     eval_numentries( Enode *root );
char   *eval_maxword( Enode *root );
int     eval_depth( Enode *root );

void    dealloc_mem( Enode *root );


int     word_compare( char *word1, char *word2 );
void    to_locase( char *lo_word, char *word );


int main( int argc, char *argv[] )
/********************************/
{
    FILE   *ifp, *ofp;
    char    wordbuff[WORDBUFFSIZE];
    Enode  *root = NULL;

    if ( argc != 2 )
        usage( argv[0] );

    ifp = open_file( argv[1], "r" );
    root = build_references( ifp, root, wordbuff, WORDBUFFSIZE );
    fclose( ifp );

    ofp = get_outfile( );
    print_references( ofp, root );
    print_stats( ofp, root );
    fclose( ofp );
    dealloc_mem( root );

    return 0;
}


void  usage( char *progname )
/***************************/
{
    fprintf( stderr, "Usage: %s \"source_file\"\n", progname );

    exit( EXIT_FAILURE );
}


FILE *open_file( char *filename, const char *mode )
/*************************************************/
{
    FILE *fp;

    fp = fopen( filename, mode );    
    if ( fp == NULL ) {
        fprintf( stderr, "File \"%s\" not opened;  "
                         "program terminated.\n", filename );
        exit( EXIT_FAILURE );
    }

    return fp;
}


Enode *build_references( FILE *ifp, Enode *root, 
                         char *wordbuff, int buffsize )
/*****************************************************/
{
    int    linenum = 1;                /* Line number for current word  */

    while ( getword( ifp, wordbuff, buffsize, &linenum ) ) 
        root = bst_insert( root, wordbuff, linenum );

    return root;
}


        /*
        ** If a new word, make a new entry and insert in appropriate
        **   position in BST.
        ** If not a new word, make a new line number node if it is 
        **   a new line number; otherwise, do nothing.
        */
Enode  *bst_insert( Enode *root, char *wordbuff, int linenum )
/************************************************************/
{
    int comparison;                    /* Result of comparing two words */

    if ( root == NULL ) 
        root = make_entrynode( wordbuff, linenum );
    else {
        comparison =  word_compare( root->word, wordbuff );
        if ( comparison > 0 )
            root->ltree = bst_insert( root->ltree, wordbuff, linenum );
        else if ( comparison == 0 ) {
            if ( root->ln_last->lnum != linenum ) {
                root->ln_last->next = make_lnnode( linenum );
                root->ln_last       = root->ln_last->next;
            }
        }
        else
            root->rtree = bst_insert( root->rtree, wordbuff, linenum );
    }

    return root;
}


int    getword( FILE *fp, char *wordbuff, int buffsize, int *linenum )
/*******************************************************************/
{
    int c;

    while ( ( c = fgetc( fp ) ) != EOF && !isalpha( c ) )
        if ( c == '\n' )
            ++*linenum;
    if ( c == EOF ) {
        return EODATA;
    }

    while ( c != EOF && isalpha( c ) ) {
        if ( --buffsize > 0 )
            *wordbuff++ = c;
        c = fgetc( fp );
    }
    *wordbuff = '\0';

    ungetc( c, fp );

    return NEXTDATA;
}




char  *make_wordnode( char *wordbuff )
/************************************/
{
    char *word;

    word = malloc( (strlen( wordbuff) + 1 ) * sizeof( char ));

    strcpy( word, wordbuff );

    return word;
}


LNnode *make_lnnode( int linenum )
/********************************/
{
    LNnode *new_lnnode;

    new_lnnode       = malloc( sizeof( LNnode ) );
    new_lnnode->lnum = linenum;
    new_lnnode->next = NULL;

    return new_lnnode;
}

Enode *make_entrynode( char *wordbuff, int linenum )
/**************************************************/
{
    Enode *new_entrynode;

    new_entrynode          = malloc( sizeof( Enode ) );
    new_entrynode->ltree   = NULL;
    new_entrynode->word    = make_wordnode( wordbuff );
    new_entrynode->ln_head = new_entrynode->ln_last 
                           = make_lnnode( linenum );
    new_entrynode->rtree   = NULL;

    return new_entrynode;
}
   



FILE *get_outfile( void )
/***********************/
{
    char filename[NAME_ASIZE];
    FILE *ofp;

    printf( "Name of output file: ");
    scanf( "%s", filename );
    ofp = open_file( filename, "w" );

    return ofp;
}


void   print_references( FILE *fp, Enode *root )
/**********************************************/
{
    if ( root != NULL ) {
        print_references( fp, root->ltree );
        print_one_entry( fp, root );
        print_references( fp, root->rtree );
    }

    return;
}


        /* Print one reference entry. */
void   print_one_entry( FILE *fp, Enode *entry )
/**********************************************/
{
    LNnode *curr;
    int     i = 0;

    fprintf( fp, "%-20s: ", entry->word );
    
    for ( curr = entry->ln_head ; curr != NULL ; curr = curr->next ) {
        fprintf( fp, "%3d ", curr->lnum );
        if ( ( i+1 ) % NCOLS == 0 && curr->next != NULL )
            fprintf( fp, "\n%22s", "" );
        i++;
    }
    fprintf( fp, "\n" );

    return;
}

void   print_stats( FILE *fp, Enode *root )
/*****************************************/
{
    char *maxword;

    fprintf( fp, "\n" );
    fprintf( fp, "Number of cross_reference entries   : %8d words\n", 
             eval_numentries( root ) );
    maxword = eval_maxword( root );
    fprintf( fp, "Maximum word size                   : %3d\n", 
             ( int ) strlen( maxword ) );
    fprintf( fp, "First word of this size             : %s\n", 
             maxword );
    printf( "Tree depth = %d\n", eval_depth( root ) );

/*
    Enode  *curr;
    LNnode *currln;
    int     memory_provided            = 0,
            memory_used                = 0;
    char    maxword[WORDBUFFSIZE]      = { '\0' };
    char    max_xrefword[WORDBUFFSIZE] = { '\0' };
    int     wordsize,
            max_wordsize  = 0,
            totalchars    = 0,
            totalentries  = 0;
    int     n_xrefs,
            max_xrefs     = 0,
            total_xrefs   = 0;

    for ( curr = root ; curr != NULL ; curr = curr->ltree ) {
        totalentries++;
        wordsize = strlen( curr->word );
        if ( wordsize > max_wordsize ) {
            max_wordsize = wordsize;
            strncpy( maxword, curr->word, WORDBUFFSIZE );
        }
        totalchars += wordsize;

        n_xrefs = 0;
        for ( currln = curr->ln_head ; currln != NULL ; currln = currln->next )
            n_xrefs++;
        if ( n_xrefs > max_xrefs ) {
            max_xrefs = n_xrefs;
            strncpy( max_xrefword, curr->word, WORDBUFFSIZE-1 );
        }
        total_xrefs     += n_xrefs;

        memory_used     += wordsize * sizeof( char ) +
                           n_xrefs  * sizeof( int );
        memory_provided += ( wordsize + 1 ) * sizeof( char ) +
                           n_xrefs * sizeof( LNnode )        +
                           sizeof( Enode );
    }

    fprintf( fp, "Average word size                   : %5.1f characters.\n", 
             ( double ) totalchars / totalentries );
    fprintf( fp, "Maximum number of x-refs            : %3d\n", max_xrefs );
    fprintf( fp, "First word with this number of xrefs: %s\n", max_xrefword );
    fprintf( fp, "Average number of x-refs            : %5.1f\n",
             ( double ) total_xrefs / totalentries );
    fprintf( fp, "\n" );
    fprintf( fp, "Memory provided for entries         : %8d bytes\n",
             memory_provided );
    fprintf( fp, "Memory actually used for data       : %8d bytes\n",
             memory_used );
    fprintf( fp, "Percentage of provided memory used  : %8.2f %%\n",
             100.0 * memory_used / memory_provided );
*/


    return;
}

int     eval_numentries( Enode *root )
/************************************/
{
    if (root == NULL )
        return 0;
    else
        return 1 + eval_numentries( root->ltree ) 
                 + eval_numentries( root->rtree ) ;
}

char   *eval_maxword( Enode *root )
/********************************/
{
    char *maxword, *maxword_ltree, *maxword_rtree;

    if ( root == NULL )
        return NULL;

    maxword = root->word;

    if ( root->ltree == NULL && root->rtree == NULL )
        return maxword;
    if ( root->ltree == NULL ) {
        maxword_rtree = eval_maxword( root->rtree );
        if ( strlen( maxword ) < strlen( maxword_rtree ) )
            maxword = maxword_rtree;
    }
    else if ( root->rtree == NULL ) {
        maxword_ltree = eval_maxword( root->ltree );
        if ( strlen( maxword ) < strlen( maxword_ltree ) )
            maxword = maxword_ltree;
    }
    else {
        maxword_ltree = eval_maxword( root->ltree );
        maxword_rtree = eval_maxword( root->rtree );
        if ( strlen( maxword ) < strlen( maxword_ltree ) )
            maxword = maxword_ltree;
        if ( strlen( maxword ) < strlen( maxword_rtree ) )
            maxword = maxword_rtree;
    }

    return maxword;
}

int     eval_depth( Enode *root )
/*******************************/
{
    int depth_ltree, depth_rtree;

    if ( root == NULL )
        return 0;
    else {
        depth_ltree = eval_depth( root->ltree );
        depth_rtree = eval_depth( root->rtree );
        if ( depth_ltree > depth_rtree )
            return 1 + depth_ltree;
        else
            return 1 + depth_rtree;
    }
}

     
void  dealloc_mem( Enode *root )
/*****************************/
{
    LNnode *currln, *tmpln;

    if ( root != NULL ) {
        dealloc_mem( root->ltree );
        dealloc_mem( root->rtree );
        free (root->word );
        for ( currln = root->ln_head ; currln != NULL ; currln = tmpln ) {
            tmpln = currln->next;
            free( currln );
        }
        free( root );
    }

    return;
}


        /*
        ** Utility functions.
        ** -----------------
        */

        /* `lo_word[]' is `word[]' with all letters converted 
             to lowercase.                                              */
void  to_locase( char *lo_word, char *word )
/******************************************/
{
    int i = 0;

    while ( ( lo_word[i] = tolower( word[i] ) ) )
        i++;

    return;
}


        
        /*
        ** Library function `strcmp()' is used for comparisons.
        ** Firstly, compare with all letters converted to lower-case
        **   If two words are equal on this basis, then use a
        **   case-sensitive comparison.
        ** Eg: `tap' precedes `To' precedes `to' 
        **           precedes `Top' precedes `top'.
        */
int   word_compare( char word1[], char word2[] )
/**********************************************/
{
    int  compare;
    char lo_word1[WORDBUFFSIZE],  /* To store lower-case versions       */ 
         lo_word2[WORDBUFFSIZE];

    to_locase( lo_word1, word1 );
    to_locase( lo_word2, word2 );
    compare = strcmp( lo_word1, lo_word2 );
    if ( compare == 0 )
        compare = strcmp( word1, word2 );

    return compare;
}




/************************************************************************/



