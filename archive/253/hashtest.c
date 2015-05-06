/*
    433253 -- Algorithms and Data Structures

    Hash Functions for Strings

    Skeleton code by: mjl

    -------------------------------------
    Directions:
    
    Write different hash functions and observe the statistics that result.
    the aim is to produce a function which gives an even distribution
    for most input types.

    Try very different functions...anything which maps
         char[]  -> int 
      ie strings -> [0..HASH_SIZE]
    is an acceptable candidate function.  Be creative.
    
    Try /usr/dict/words as a sample file.
    Also try different hash_table sizes eg powers of 2, powers of 10,
    primes etc. 

    Questions:

      The original parameters and hash function seem reasonable.
      So why does the function always return 10?

      What is the effect of modifying HASH_TABLE_SIZE to 30 or 40?
      Explain the output.
      What about more realistic values like 1024? 65536?

      What is the effect of stripping newlines from the strings with size=32?

      What is the effect of a different multiplier, eg 31 rather than 32,
      with and without newlines.
      Determine what multipliers seem to work well and which are very poor.
      Can you find a test to tell what multiplier/size combinations work well?

      What is the effect of a different hash table size and modulus, eg 33
      rather than 32.

      Try making up completely different mappings for the hash function.
      See if you can make one that distributes the strings randomly across
      the buckets no matter what value HASH_TABLE_SIZE has

    ---------------------------------------
      
    compilation:
      gnuc hash.c -lm
    
    usage:
      hash < textfile
    where, if the statistics reported are to be sensible, each line
    of the the textfile should be unique.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#define HASH_TABLE_SIZE  31 

#define MAX_STRING       1000
#define GROUPS           40      /* adjust this to vary the output */
				 /* you should be able to fit 40 lines */
				 /* on a tall screen */

unsigned int hash_func1( char* s);


int main( void ){
  unsigned int hash_table[HASH_TABLE_SIZE] = {0};
  char line[MAX_STRING];
  unsigned int result;
  unsigned int start, finish, total, i;
  unsigned int grouping_size,j;
  unsigned int min, max, min_elem, max_elem;
  unsigned int nstrings=0;
  unsigned counter;
  unsigned scale;
  
  
  /* read in the lines and evaluate the hash function */
  while( fgets( line, MAX_STRING, stdin ) != NULL){

    /*uncomment next line to remove the newlines*/
    /* line[ strlen(line) -1 ] = '\0'; */

    result = hash_func1( line );
    hash_table[ result ]++;
    nstrings++;
  }
        
  /* statistics */

  grouping_size = ceil((double)HASH_TABLE_SIZE / GROUPS);
  scale = 1 + nstrings/GROUPS/10;
  /* calculated so that average group of buckets should show 10 *s  in graph */
  
  for( i = 0; i < GROUPS; i++ ){
    total = 0;
    start = i * grouping_size;
    if(start >= HASH_TABLE_SIZE)
      continue;
    finish = (i+1) * grouping_size-1;
    if(finish >= HASH_TABLE_SIZE)
      finish = HASH_TABLE_SIZE - 1;
    
    for( j = start; j <= finish ; j++){
      total += hash_table[j];
    }
    printf("%4d->%4d : ", start, finish);
    counter = total/scale;
    for (counter=total/scale; counter; counter--)
      printf("*");
    printf("\n");
  }

  /* find minimum and maximum */
  min_elem = 0;
  min = hash_table[0];
  max_elem = 0;
  max = hash_table[0];
  for(i=1; i < HASH_TABLE_SIZE; i++){
    if( hash_table[i] < min){
      min_elem = i;
      min = hash_table[i];
    }
    if( hash_table[i] > max){
      max_elem = i;
      max = hash_table[i];
    }
  }

  printf("Number of strings processed:  %d\n", nstrings);
  printf("Each * represents about %d strings\n", scale);
  printf("Minimum entries in a bucket:  bucket %d has %d items\n",
    min_elem, min);
  printf("Maximum entries in a bucket:  bucket %d has %d items\n",
    max_elem, max);
  printf("Average strings per bucket should be: %.1f\n", 
  	(float)nstrings/HASH_TABLE_SIZE);
  
  return 0;
}

/*
 * Example Hash Function -- From the 253 Lecture Notes, slide #292
 */
unsigned int hash_func1(char* s)
{
  unsigned int multiplier = 32;
  unsigned int modulus = HASH_TABLE_SIZE;
  unsigned int result = 0;
  
  while( *s != '\0' ){
    result = (result*multiplier + (unsigned int) *s ) % modulus;
    s++;
  }

  return result;
}
