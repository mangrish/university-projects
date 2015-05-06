#include <stdio.h>

FILE *open_file();
void get_file(FILE *fp);


int main( int argc, char *argv[])
{
	FILE *fp = NULL;
	
	if (argc != 2)
	{
		printf("USAGE: ugroups username\n");
		exit(1);
	}
	
	UserName = argv[1];
	fp = open_file();
	get_file(fp)
}

FILE *open_file()
{
	FILE *fp;

	fp = fopen("/etc/group"  , "r" );

  if (fp == NULL )
  {
		fprintf( stderr, "%s: ERROR: File \"%s\" not opened;  "
                 "program execution terminated.\n", progname, fname );

    exit( EXIT_FAILURE );
  }

  return fp;
}

void get_file(FILE *fp)
{
	int curr_char= getc(fp);	
	
	while (curr_char!= EOF )
		get_line(curr_char);

	return;
}

void get_line()
{
	
