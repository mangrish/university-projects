/*
** Program : projb1.c 
** For     : 433--142 Project B, Summer Semester, 1998-99
** Written : VRJ; 8/9/1997
**
** See Project Specification for full details and requirements.
**
** Input   : From stdin:  Text whose words are to be cross-referenced.
** Output  : To stdout :  Cross-referencing of all different words
**
** In this program, the cross-referenced data is stored in an array
**   of structs of type `Entry'.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


                                  /* Sizes for arrays                   */
                                  /* ----------------                   */
#define WORD_ASIZE      21        /* Size for word-array                */
#define ENTRIES_ASIZE 1500        /* Size for array of ref. entries     */
#define LNSIZE         100        /* Size for array of line numbers     */

                                  /* Other constants                    */
                                  /* ----------------------------       */
#define EOINPUT          0        /* Line number designating input end  */
#define NCOLS           10        /* Number of cols for line-numbers    */


                                  /* Type definitions                   */
                                  /* ----------------                   */
typedef struct reference_entry  Entry;

                                  /* Structure templates                */
                                  /* -------------------                */
struct reference_entry {
    char word[WORD_ASIZE];
    int  line_nums[LNSIZE];
    int  n_linenums;
};

                                  /* Function prototypes                */
                                  /* -------------------                */
int   build_references( Entry xrefs[], int maxentries );
int   getword( char word[], int word_asize, int linenum );
int   search_references( Entry xrefs[], int numentries, char word[] );
void  add_entry( Entry xrefs[], int numentries, 
                 char word[], int linenum );
void  add_linenum( Entry xrefs[], int entrynum, int linenum );

void  print_references( Entry xrefs[], int numentries );
void  print_one_entry( Entry thisentry );
void  print_stats( Entry xrefs[], int numentries );

void  my_qsort( Entry xrefs[], int left, int right );
int   partition( Entry xrefs[], int left, int right );
void  swap( Entry xrefs[], int i, int j );
int   word_compare( char word1[], char word2[] );
void  to_locase( char lo_word[], char word[] );


        /*
        ** Main function.
        ** -------------
        */
int main( void )
{
    Entry  xrefs[ENTRIES_ASIZE];
    int    numentries;

    numentries = build_references( xrefs, ENTRIES_ASIZE );
    my_qsort( xrefs, 0, numentries-1 );
    print_references( xrefs, numentries );
    print_stats( xrefs, numentries );

    return 0;
}


        /*
        ** Build the references.
        ** --------------------
        ** Read text from stdin, break into words, add/update entries
        **   in references.
        ** Return final number of references entries.
        */
int   build_references( Entry xrefs[], int maxentries )
{
    int  numentries = 0,          /* Number of reference entries        */
         entrynum,                /* Current entry number               */
         linenum = 1;             /* Line number for the current word   */
    char word[WORD_ASIZE];        /* To store current input word        */

    linenum = getword( word, WORD_ASIZE, linenum );
    while ( linenum > EOINPUT && numentries < maxentries ) {
        entrynum = search_references( xrefs, numentries, word );
        if ( entrynum == numentries ) {
            add_entry( xrefs, numentries, word, linenum );
            numentries++;
        }
        else
            add_linenum( xrefs, entrynum, linenum );
        linenum = getword( word, WORD_ASIZE, linenum );
    }

    if ( numentries == maxentries && 
           ( linenum = getword( word, WORD_ASIZE, linenum ) ) > 0 ) {
        fprintf( stderr, "Program termination:  Number of word-entries "
                         "exceeds limit of %d.\n", maxentries );
        exit( EXIT_FAILURE );
    }

    return numentries;
}

        /*
        ** Get next word from stdin, store in `word'.
        ** A `word' is any string of alpha chars.
        ** Return the input line number from which that word is read.
        ** At end of input, return the not-a-linenumber value, EOINPUT. 
        */
int   getword( char word[], int word_asize, int linenum )
{
    int c,                        /* Current input character            */
        i = 0;                    /* Counter for word-array             */

                                  /* Skip over all non-alpha chars      */
    while ( ( c = getchar() ) != EOF && !isalpha( c ) )
        if ( c == '\n' )
            linenum++ ;
    if ( c == EOF )               /* End of input; return linenum = -1  */
        return EOINPUT;

                                  /* Get and store the next word        */
    while ( c != EOF && isalpha( c ) ) {
                                  /* Store up to `word_asize - 1' chars,
                                       discard the rest                 */
        if ( --word_asize > 0 )      
            word[i++] = c;
        c = getchar();
    }
    word[i] = '\0';
                                  /* Put the char that terminates this
                                       word back on to the input stream 
                                       (so that we count line-numbers
                                       properly).                       */
    ungetc( c, stdin );    

    return linenum;
}


        /*
        ** Find and return array-location for next word.
        ** If word is not already in xrefs[], return `numentries'.
        */
int   search_references( Entry xrefs[], int numentries, char word[] )
{
    int i;

    for ( i = 0 ; 
          i < numentries && strcmp( xrefs[i].word, word ) != 0; 
          i++ )
        ;

    return i;
}


        /*
        ** Add a complete new entry (word and line-number) to
        **   the references array.
        */
void  add_entry( Entry xrefs[], int numentries, 
                 char word[], int linenum )
{
    Entry newentry;

    strcpy( newentry.word, word );
    newentry.line_nums[0] = linenum;
    newentry.n_linenums   = 1;

    xrefs[numentries] = newentry;

    return;
}
    
        /*
        ** Amend existing reference entry if the new line number 
        **   differs from the last-entered line number.
        */
void  add_linenum( Entry xrefs[], int entrynum, int linenum )
{
    int n = xrefs[entrynum].n_linenums;

    if ( n == LNSIZE ) {
        fprintf( stderr, "Program termination:\n" );
        fprintf( stderr, "   For word \"%s\":  ", xrefs[entrynum].word );
        fprintf( stderr, "number of cross-references exceeds limit of %d.\n",
                         LNSIZE );
        exit( EXIT_FAILURE );
    }
    else if ( xrefs[entrynum].line_nums[n-1] != linenum ) {
        xrefs[entrynum].line_nums[n] = linenum;
        xrefs[entrynum].n_linenums++;
    }

    return;
}


        /*
        ** Print references entries to stdout.
        ** -----------------------------------
        */
void  print_references( Entry xrefs[], int numentries )
{
    int i;

    for ( i = 0 ; i < numentries ; i++ ) 
        print_one_entry( xrefs[i] );

    return;
}


void  print_one_entry( Entry thisentry )
{
    int i,
        n  = thisentry.n_linenums,
        fw = WORD_ASIZE - 1;                   /* Field width for word */

    printf( "%-*s: ", fw, thisentry.word );
    for ( i = 0 ; i < n ; i++ ) {
        printf( "%3d ", thisentry.line_nums[i] );
        if ( (i+1) % NCOLS == 0 && i != n-1 ) 
            printf( "\n%*s", fw+2, "" );
    }
    printf( "\n" );

    return;
}

        /*
        ** Print statistics
        ** ----------------
        */
void  print_stats( Entry xrefs[], int totalentries )
{
    int  memory_provided,
         memory_used              = 0;
    char max_word[WORD_ASIZE]     = { '\0' };
    char max_xrefword[WORD_ASIZE] = { '\0' };
    int  wordsize, 
         max_wordsize = 0, 
         totalchars   = 0;
    int  n_xrefs, 
         max_xrefs    = 0, 
         total_xrefs  = 0;
    int i;


    if ( totalentries == 0 ) {
        printf( "No words in input file; no cross-referencing.\n" );
        return;
    }
    for ( i = 0 ; i < totalentries ; i++ ) { 
        wordsize =  strlen( xrefs[i].word );
        if ( wordsize > max_wordsize ) {
            max_wordsize = wordsize;
            strcpy( max_word, xrefs[i].word );
        }
        totalchars   += wordsize;

        n_xrefs  = xrefs[i].n_linenums;
        if ( n_xrefs > max_xrefs ) {
            max_xrefs = n_xrefs;
            strcpy( max_xrefword, xrefs[i].word );
        }
        total_xrefs += n_xrefs;

        memory_used += wordsize * sizeof( char ) + 
                       n_xrefs * sizeof( int );
    }
    memory_provided =  ENTRIES_ASIZE * sizeof( Entry );

    printf( "\n" );
    printf( "Maximum word size                   : %3d\n", max_wordsize );
    printf( "First word of this size             : %s\n", max_word );
    printf( "Average word size                   : %5.1f characters.\n", 
             ( double ) totalchars / totalentries );
    printf( "Maximum number of x-refs            : %3d\n", max_xrefs );
    printf( "First word with this number of xrefs: %s\n", max_xrefword );
    printf( "Average number of x-refs            : %5.1f\n",
             ( double ) total_xrefs / totalentries );
    printf( "\n" );
    printf( "Number of cross_reference entries   : %8d words\n", 
             totalentries );
    printf( "Memory provided for entries         : %8d bytes\n",
             memory_provided );
    printf( "Memory actually used for data       : %8d bytes\n",
             memory_used );
    printf( "Percentage of provided memory used  : %8.2f %%\n",
             100.0 * memory_used / memory_provided );

    return;
}


        /*
        ** Utility functions.
        ** -----------------
        */

        /* `lo_word[]' is `word[]' with all letters converted 
             to lowercase.                                              */
void  to_locase( char lo_word[], char word[] )
{
    int i = 0;

    while ( ( lo_word[i] = tolower( word[i] ) ) )
        i++;

    return;
}

        /* my_qsort - Sort the reference entries in alphabetical
                        order of the `word' item.     
                      Using my version rather than library function.    */
void  my_qsort( Entry xrefs[], int left, int right )
{
    int i;

    if ( right > left ) {
        i = partition( xrefs, left, right );
        my_qsort( xrefs, left, i-1 );
        my_qsort( xrefs, i+1, right );
    }

    return;
}

        /*
        ** Partition the references array for my_qsort.
        ** This function is not called unless there 2 or more elements
        **   to be partitioned.
        ** (Based on Kernighan & Ritchie's version, K&R p.87.)
        */
int   partition( Entry xrefs[], int left, int right )
{
    int pindex = left,                 /* Current index-position of last
                                          element that is `<=' pivot
                                            value at index 0.           */
        i;                             /* Array/loop index              */

    swap( xrefs, left, (left + right) / 2 );

    for ( i = left+1 ; i <= right ; i++ )
        if ( word_compare( xrefs[i].word, xrefs[left].word ) <= 0 ) {
            pindex++;
            swap( xrefs, pindex, i );
        }
    swap( xrefs, left, pindex );

    return pindex;
}


        /* Swap -- interchange two entry records in the references array */
void  swap( Entry xrefs[], int i, int j )
{
    Entry tmp;

    tmp      = xrefs[i];
    xrefs[i] = xrefs[j];
    xrefs[j] = tmp;

    return;
}

        /*
        ** Library function `strcmp()' is used for comparisons.
        ** Firstly, compare with all letters converted to lower-case
        **   If two words are equal on this basis, then use a
        **   case-sensitive comparison.
        ** Eg: `tap' precedes `Tip' precedes `To' precedes `to' precedes `top'.
        */
int   word_compare( char word1[], char word2[] )
{
    int  compare;
    char lo_word1[WORD_ASIZE],    /* To store lower-case versions       */ 
         lo_word2[WORD_ASIZE];

    to_locase( lo_word1, word1 );
    to_locase( lo_word2, word2 );
    compare = strcmp( lo_word1, lo_word2 );
    if ( compare == 0 )
        compare = strcmp( word1, word2 );

    return compare;
}

/************************************************************************/

