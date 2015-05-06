#define HASHSIZE 101

struct nlist {
	struct nlist *next;
	char *string;
}


static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *string);








unsigned hash( char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 63 * hashval;
	return hashval;
}






struct nlist *lookup( char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (np->hashval == NULL)
			match = strsim(*s, np->string, buff +blocklen);
			if (match>coplen) 
			{
				coplen = match;
				copoff = stph-c;
			}
			return np;

	return NULL;
}




struct nlist *install( char *string)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(string)) == NULL)
			{
			np = (struct nlist *) malloc(sizeof(*np));
			if (np == NULL || (np->string = strdup(string)) == NULL)
				return NULL;
			hashval = hash(string);
			np->next = NULL;
			hashtab[hashval] = np;
			}
	else
		match = strsim(string,np->string , buff+blocklen);


	return np;
}


