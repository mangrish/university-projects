/* slidwid.c: Simple compressing program based upon string substitution.
   Decoding function written by Alistair Moffat, alistair@cs.mu.oz.au,
   September 1999.
   Encoding function written by XXXXXX XXXXXX.

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

#define HASHSIZE 65536

typedef unsigned char uchr;

struct nlist {
	struct nlist *next;
	uchr *string;
};


struct nlist *hashtab[HASHSIZE];

unsigned hash(uchr *s);
void install(char *stph);
int lookup( uchr *stph, unsigned short *copoff,uchr *buffend);
struct nlist *make_node(uchr *stph);

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

void
encoder(unsigned *ibytes, unsigned *obytes) {
	/* This simplistic encoder uses brute force to look for copy strings.
	   If it can't find a copy, it sends four characters as literals,
	   and trys again for a copy.
	   Yours needs to
	   (a) use something smarter than brute force searching so that it
	   executes (much?) faster; and
	   (b) employ better rules for the use of literals and
	   copys, so that it gets better compression.
	   (In my laziness I have coded all copies as three-byte ones, so
	   there is one obvious possibility that you should explore.)
	*/

	uchr *buff;		/* buffer to hold one block of input file */
	uchr *stph;		/* start of current phrase */

	int blocklen;		/* length of current block */
	int litlen;		/* length of current literal */
	int coplen;		/* length of best copy found */
	unsigned short copoff;	/* offset for best copy pointer */
	uchr byte;		/* holds a byte for the output stream */

	if ((buff = malloc(BUFFSIZE)) == NULL) {
		fprintf(stderr, "%s: Unable to malloc %d bytes for buffer\n",
			pn, BUFFSIZE);
		exit(1);
	}

	*obytes = *ibytes = 0;

	/* Ok, now process the input file block by block */
	while ((blocklen = fread(buff, 1, BUFFSIZE, stdin)) > 0) {
		*ibytes += blocklen;
		stph = buff;
		while (stph < buff+blocklen) {
			coplen = lookup(stph, &copoff, buff + blocklen);

			if (coplen<MINLONGCOPY) {
				/* No copy? Then send a literal, but watch for
				   end of block */
				if (stph+MINLONGCOPY>buff+blocklen)
					litlen = (buff+blocklen) - stph;
				else
					litlen = MINLONGCOPY;

				install(stph);
				byte = PACKBYTE(LITERAL,litlen);
				putchar(byte);
				*obytes += 1;
				fwrite(stph, 1, litlen, stdout);
				*obytes += litlen;
				stph += litlen;
			} else {
				/* send a long copy and start again */
				byte = PACKBYTE(LONGCOPY,coplen);
				putchar(byte);
				*obytes += 1;
				copoff -= 1;
				fwrite(&copoff, sizeof(copoff), 1, stdout);
				*obytes += 2;
				stph += coplen;
			}
		}
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



unsigned hash( uchr *s)
{
	unsigned hashval= 0;
	int i;

	for (i = 0; i < 2; i++)
		{
			if (*(s+i) ==  '\0')
				break;
			hashval = *(s+i) * hashval;
		}
	return hashval;
}


int lookup( uchr *stph, unsigned short *copoff,uchr *buffend)
{
	struct nlist *np;
	int match;
	int coplen = 0;

	for (np = hashtab[hash(stph)]; np != NULL; np = np->next)
		match = strsim(stph, np->string, buffend);
		if (match>coplen) 
		{
			coplen = match;
			*copoff = stph- np->string;
		}

	return coplen;
}




void install( char *stph)
{
	struct nlist *np, *newn, *tmp;
	unsigned hashval;

	hashval = hash(stph);
	newn = make_node(stph);
	tmp = NULL;

	if ((np =(hashtab[hashval])) != NULL)	
	{
		tmp = np;
		np = newn;
		np->next = tmp;
	}
	else 
		np = newn;
			


	return;
}

struct nlist *make_node(uchr *stph)
{
	struct nlist *newn;

	newn = (struct nlist *) malloc(sizeof(*newn));

	if (newn ==NULL )
		fprintf( stderr, "Failure: Memory not allocated for data item.\n"); 
		exit(EXIT_FAILURE);

	newn->string = stph;
	newn->next = NULL;

	return newn;	
}

