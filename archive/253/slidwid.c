/* slidwid.c: Simple compressing program based upon string substitution.
   Decoding function written by Alistair Moffat, alistair@cs.mu.oz.au,
   September 1999.
   Encoding function written by MARK ANGRISH( student numbe: 74592 login:mangr).

   In the encoder the input file is parsed into two types of strings:
   
   -- literal strings that are transmitted directly to the decoder, because
      they cannot be (economically) matched in the previous text; and
   -- copy strings, which are offsets back into the previous text to indicate
      where that string has occurred previously.

   In the compressed file these two types of strings are represented as

   -- literal strings:
      a first byte with '00' in the two high-order bits, followed by
      an unsigned 6-bit integer n in the range of 0 to 63, indicating
      that the next n+1 bytes in the compressed message should be taken
      as literal characters and passed directly to the output string
   -- near copy strings:
      a first byte with '01' in the two high-order bits, followed by
      an unsigned 6-bit integer n in the range of 0 to 63, this byte
      followed by a second byte storing an 8-bit unsigned integer f,
      indicating that the next n+1 bytes of output should be copied
      from locations c-f-1, where c is the output current byte location
      in the output file.
   -- mid copy strings:
      a first byte with '10' in the two high-order bits, followed by
      an unsigned 6-bit integer n in the range of 0 to 63, this byte
      followed by a second byte storing an 8-bit unsigned integer f,
      indicating that the next n+1 bytes of output should be copied
      from locations c-f-257, where c is the output current byte location
      in the output file.
   -- long copy strings:
      a first byte with '11' in the two high-order bits, followed by
      an unsigned 6-bit integer n in the range of 0 to 63, this byte
      followed by two further bytes storing a 16-bit unsigned integer f,
      indicating that the next n+1 bytes of output should be copied
      from locations c-f-1, where c is the output current byte location
      in the output file.

   For a more detailed explanation, read the code for function decoder().

   Both encoder and decoder read from either stdin or a named file, and
   write to stdout.
*/


#include <stdio.h>
#include <stdlib.h>

void encoder(unsigned *ibytes, unsigned *obytes);
void decoder(unsigned *ibytes, unsigned *obytes);

char *pn;    /* Pointer to the name of the program */

int
main(int argc, char **argv) {
	int encoding;
	unsigned ibytes, obytes;
	pn = argv[0];
	if (argc==1) {
		fprintf(stderr, "Usage: %s [-e|-d] [filename]\n", pn);
		exit(1);
	}
	encoding = (strcmp(argv[1], "-e") == 0);
	if (argc>2) {
		/* process the optional filename */
		if (freopen(argv[argc-1], "r", stdin) == NULL) {
			fprintf(stderr, "%s: cannot open file %s\n",
			    pn, argv[argc-1]);
			exit(1);
		}
	}

	/* Ok, now ready to do the deed */
	if (encoding) {
		encoder(&ibytes, &obytes);
		fprintf(stderr, "%s: %6u input bytes\n", pn, ibytes);
		fprintf(stderr, "%s: %6u output bytes\n", pn, obytes);
		fprintf(stderr, "%s: %6.2f bits/byte compression\n",
			pn, 8*(float)obytes/ibytes);
	} else {
		decoder(&ibytes, &obytes);
		fprintf(stderr, "%s: %6u output bytes\n", pn, obytes);
	}

	return 0;
}

/* ======================================================== */

#define BUFFSIZE 65536
#define MAXCOPY 64
#define MAXLITERAL 64

#define LITERAL 0
#define NEARCOPY 1
#define MIDCOPY 2
#define LONGCOPY 3

#define MIDOFFSET 256

#define FLAGBITS(x)	(((uchr)(x))>>6)
#define LENBITS(x)	((((uchr)(x))&(0x3f))+1)
#define PACKBYTE(flag,len) \
			((uchr)((((flag)&0x3)<<6)+((len)-1)))

#define MINLONGCOPY 4 
#define MINSHORTCOPY 3

#define HASHSIZE	65537 


typedef unsigned char uchr;

	/* the struct of a pointer to the current phase(stph) and to the next element in the
		 list
	*/	 
struct nlist {
	struct nlist *next;
	uchr *string;
};


	/* the array of pointers to structs; globally defined
	*/	 
struct nlist *hashtab[HASHSIZE] = {NULL};

	/* function declarations
	*/	 
unsigned hash(uchr *s);
void install(char *stph, unsigned hashval);
int lookup( uchr *stph, unsigned short *copoff,uchr *buffend);
struct nlist *make_node(uchr *stph);
void freemem( void);

int
strsim(uchr *p1, uchr *p2, uchr *lim) {
	/* Report how many bytes strings at p1 and p2 have in common,
	   with p2 not to go past lim, and match to not exceed MAXCOPY
	*/
	int same = 0;
	while ((*p1==*p2) && (p2!=lim) && (same<MAXCOPY)) {
		p1++; p2++;
		same++;
	}
	return(same);
}

/* encoder:	encodes the given file, block by block, by hashing the current value of the phase pointer, 
	 					then is placed into the hashtable, and is tehn packed into a byte, depending on the size of copoff
*/	 
void encoder(unsigned *ibytes, unsigned *obytes) 
{

	uchr *buff;		/* buffer to hold one block of input file */
	uchr *stph;		/* start of current phrase */

	int blocklen;		/* length of current block */
	int litlen;		/* length of current literal */
	int coplen;		/* length of best copy found */
	unsigned short copoff;	/* offset for best copy pointer */
	uchr byte;		/* holds a byte for the output stream */

	if ((buff = malloc(BUFFSIZE )) == NULL) {
		fprintf(stderr, "%s: Unable to malloc %d bytes for buffer\n",
			pn, BUFFSIZE );
		exit(1);
	}

	*obytes = *ibytes = 0;

	/* Ok, now process the input file block by block */
	while ((blocklen = fread(buff, 1, BUFFSIZE, stdin)) > 0) {
		*ibytes += blocklen;
		stph = buff;
		while (stph < buff+blocklen) 
		{
			/* coplen is now equal to the value the lookup function returns */
			coplen = lookup(stph, &copoff, buff + blocklen);

			if (coplen == 0) 
			{
				/* No copy? Then send a literal, but watch for
				   end of block */
				if (stph+MINLONGCOPY>buff+blocklen)
					litlen = (buff+blocklen) - stph;
				else
					litlen = MINLONGCOPY;

				byte = PACKBYTE(LITERAL,litlen);
				putchar(byte);
				*obytes += 1;
				fwrite(stph, 1, litlen, stdout);
				*obytes += litlen;
				stph += litlen;
			} 
			else if (copoff <=  MIDOFFSET)/* if coplen is less than or equal to MIDOFFSET then its a near copy */
			{
				byte = PACKBYTE(NEARCOPY,coplen);
				putchar(byte);
				*obytes += 1;
				putchar(copoff- 1);
				*obytes += 1;
				stph += coplen;
			}
			
			else if (copoff >=  2*MIDOFFSET)/* if coplen is greater than 2*MIDOFFSET then it must be a long copy*/ 
			{
				/* send a long copy and start again */
				byte = PACKBYTE(LONGCOPY,coplen);
				putchar(byte);
				*obytes += 1;
				copoff -= 1;
				fwrite(&copoff, sizeof(copoff), 1, stdout);
				*obytes += 2;
				stph += coplen;
			}
			else/* else it must be between MIDOFFSET and 2*MIDOFFSET so it must be a	midcopy */ 
			{
				byte = PACKBYTE(MIDCOPY,coplen );
				putchar(byte);
				*obytes += 1;
				putchar(copoff - 1);
				*obytes += 1;
				stph += coplen ;
			}
		
		}
		freemem();
	}
	return;
}


/* ======================================================== */

void
decoder(unsigned *ibytes, unsigned *obytes) {
	uchr *buff;		/* buffer to hold one block of input file */
	unsigned short copoff; 	/* offset for copy pointer */
	unsigned curr=0;	/* the current location in the buffer */
	unsigned from;		/* location to copy from */
	int readbyte;		/* need an integer to read bytes */
	int flag, len;		/* components of input byte */
	int extra;		/* Amount of extra offset to start of copy */

	if ((buff = malloc(BUFFSIZE)) == NULL) {
		fprintf(stderr, "%s: Unable to malloc %d bytes for buffer\n",
			pn, BUFFSIZE);
		exit(1);
	}

	*obytes = *ibytes = 0;

	while ((readbyte = getchar()) != EOF) {
		*ibytes += 1;
		flag = FLAGBITS(readbyte);
		len = LENBITS(readbyte);

		if (flag == LITERAL) {
			/* A literal */
			while (len) {
				buff[curr] = getchar();
				putchar(buff[curr]);
				*obytes += 1;
				curr++;
				if (curr==BUFFSIZE) curr=0;
				len--;
			}
		} else {
			/* Not a literal, must be a copy */
			if (flag == LONGCOPY) {
				fread(&copoff, sizeof(copoff), 1, stdin);  
				*ibytes += 2;
			} else {
				copoff = getchar();
				*ibytes += 1;
			}

			/* Calculate buffer indexes */
			if (flag == MIDCOPY) 
				extra = MIDOFFSET+1;
			else
				extra = 1;
			/* Calculate start location in circular buffer */
			if (copoff>=curr)
				from = curr + BUFFSIZE - copoff - extra; 
			else
				from = curr - copoff - extra;

			/* copy buffer */
			while (len) {
				putchar(buff[from]);
				*obytes += 1;
				buff[curr] = buff[from];
				curr++;
				if (curr==BUFFSIZE) curr=0;
				from++;
				if (from==BUFFSIZE) from=0;
				len--;
			}
		}
	}
	return;
}

/*===============================================================*/

/*  hash:	hashing fucntion that converts a string of 2 characters into an unsigned
					value, which can be used to insert  into a hash table.  it also checks if
					the end of file has occured to avoid hashing on that as well.
*/	 

unsigned hash( uchr *s)
{
	unsigned hashval= 0;
	int i;

	for (i = 0; i < 2; i++)
		{
			if (*(s+i) == '\0' )
				break;
			hashval = (256*hashval + *(s+i)) % HASHSIZE ;
		}
	return hashval;
}

/*	lookup:	lookup uses the hashvalue calculated, and then goes to that spot in the
						hashtable and checks if hashtab[hashval] is NULL.  If it iss, then stph
						is made to be a new node and inserrted into the hashtable, else if it isnt
						NULL, then stph is strsim'ed for every item in the linked list, until the
						greatest match is found, then the values of copoff and coplen are returned
						and then stph is also installed into the front of the linked list in the
						hashtable.
*/	 

int lookup( uchr *stph, unsigned short *copoff,uchr *buffend)
{
	struct nlist *np;
	int match;
	int coplen = 0;
	unsigned hashval;

	hashval = hash(stph);

	for (np = hashtab[hashval]; np != NULL ; np = np->next)
	{
		match = strsim(stph, np->string, buffend);
		if (match>coplen) 
		{
			coplen = match;
			*copoff = stph - np->string;
		}
	}
	install(stph, hashval);
	return coplen;
}

/*	install:	makes a new node (via makenode) then goes to the correct place in the
							hashtable and if the hashtable points to NULL, then it places the new 
							node as the only element in the linked list, else, if hashtable doesnt 
							point to to NULL, then it gets inserted into the front of the linked \
							list
*/	 


void install( char *stph, unsigned hashval)
{
	struct nlist *newn, *tmp;

	newn = make_node(stph);
	tmp = NULL;

	if (hashtab[hashval] != NULL)	
	{
		tmp = hashtab[hashval]; 
		hashtab[hashval] = newn;
		newn->next = tmp;
	}
	else 
		hashtab[hashval] = newn;
			


	return;
}

/*	make_node:	makes a new node for a linked list, if memeory isnt allocated, it 
								exits to stderr, with an error mesage. 
*/

struct nlist *make_node(uchr *stph)
{
	struct nlist *newn;

	newn = (struct nlist *) malloc(sizeof(struct nlist *));

	if (newn ==NULL )
	{
		fprintf( stderr, "Failure: Memory not allocated for data item.\n"); 
		exit(EXIT_FAILURE);
	}
	newn->string = stph;
	newn->next = NULL;

	return newn;	
}


/*	freemem:	frees the linked lists in the hashtable after one cylce through the 
							buffer (note:modified from  the 142 notes).
*/

void freemem( void )
{
	int i;
	struct nlist *tmp;

	for (i=0; i < HASHSIZE ; i++ )
	{
			for ( ; hashtab[i] != NULL ; hashtab[i]= tmp )
			{
				tmp = hashtab[i]->next;
				free(hashtab[i]);
			}
	}
	return;
}
