/*
**	A program to generate test data for project 1.
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	"std.h"

int	maxproc   =   4;
int	maxtrack  = 128;
int	maxdelay  = 100;
int	minspread =  10;
int	maxspread =  50;
int	requests  =  50;
int	variation =   5;

extern	int	rnd(int);
extern	int	getpid(void);
extern	void	usage(void);

extern	char	*optarg;
extern	int	optind;

void
main(int argc, char **argv)
{
	int	c, p;

	while ((c = getopt(argc, argv, "cd:l:n:r:t:u:v:")) != EOF)
	{
		switch (c)
		{

	when 'c':	srandom((unsigned int) getpid());

	when 'd':	if (sscanf(optarg, "%d", &maxdelay) != 1)
				usage();

	when 'l':	if (sscanf(optarg, "%d", &minspread) != 1)
				usage();

	when 'n':	if (sscanf(optarg, "%d", &maxproc) != 1)
				usage();

	when 'r':	if (sscanf(optarg, "%d", &requests) != 1)
				usage();

	when 't':	if (sscanf(optarg, "%d", &maxtrack) != 1)
				usage();

	when 'u':	if (sscanf(optarg, "%d", &maxspread) != 1)
				usage();

	when 'v':	if (sscanf(optarg, "%d", &variation) != 1)
				usage();

	otherwise:	usage();

		}
	}

	if (optind < argc)
		usage();

	for (p = 0; p < maxproc; p++)
	{
		int	rw, r, m, s, d, t;
		FILE	*fp;
		char	buf[128];

		sprintf(buf, "proc%d", p);
		fp = fopen(buf, "w");
		if (fp == (FILE *) NULL)
		{
			perror(buf);
			exit(1);
		}

		r = requests - variation + rnd(2 * variation + 1);
		m = rnd(maxtrack);
		s = minspread + rnd(maxspread - minspread + 1);

		printf("process %d: # of requests %3d, middle %3d, spread %2d\n", p, r, m, s);

		while (r-- > 0)
		{
			d = rnd(maxdelay);
			t = m - s + rnd(2 * s + 1);
			while (t < 0 || t >= maxtrack)
				t = m - s + rnd(2 * s + 1);
			rw = rnd(2);

			fprintf(fp, "%d %d %d\n", d, t, rw);
		}

		fclose(fp);
	}

	exit(0);
}

void
usage(void)
{
	fprintf(stderr, "Usage: gen1 [-c] [-dlnrstuv num]\n");
	exit(1);
}

int
rnd(reg int n)
{
	reg	int	r;

	r = random() % n;
	if (r < 0)
		return -r;

	return r;
}
