/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : owners.e:
**  For     : 433 -351: Databases;
**            PROJECT 2
**  Written : Mark Angrish, Student number: 74592
**            May 2001;
**  Input   : owners [-h "Hotel Name" "Hotel City"]: either no input is given
**						or a -h option with a corresponding hotel name and city.
**  Output  : To stdout: output in the form of:
**						Report on hotel owners
**						======================
**						Regent Hotel,  Sydney:
**							Telstra, Melbourne (50.0%)
**							LindaLand, Melbourne (31.2%)
**						Queens Hotel,  London:
**			        Optus, Melbourne (10.0%)
**						L`Hotel Sur Le Seine,  Paris
**						etc...
**						with a single entry given for a -h options hotel
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include "owners.h"

/*---------------------------*/
/* LOCAL FUNCTION PROTOTYPES */
/*---------------------------*/

void usage(char *);
Hotels_record *getopthotel(char *, char *);
void print_out(Output *);

/*---------------------------*/


int main(int argc, char *argv[])
{
	char *prog_name = argv[0]; /*name of the program executable*/
	char *hotel_name; /* for the -h option, holds the hotel name*/
	char *hotel_city; /* for the -h option, holds the hotel city*/
	Owners_record *owners; /* linked list of the owners in the owners table*/
	Hotels_record *hotels; /*linked list of all the hotels in the hotels table
												  or the hotel from the -h option */
	Output *output; /*linked list of all the hotels and owners for outputting*/

	/* display the usage to the user if the program is incorrectly invoked */
	if (argc != 1 && (argc != 4 || strcmp(argv[1],"-h") != 0)) 
		usage(prog_name);

	/* if there are no command line arguments.... */
	if (argc == 1) 
		hotels = gethotels();/* get the hotels from the database..*/
	 /*.. otherwise get the command line arguments, and make them the only
		 hotel to be used */
	else {
		hotel_name = argv[2];
		hotel_city = argv[3];
		hotels = getopthotel(hotel_name, hotel_city);
	}

	owners = getowners(); /*get the owners from the owners list in the database*/
	output = build(owners, hotels);/*find all the hotels and their respective 
																	 owners */
	print_out(output); /* print the output to the user */

	return 0;
}


/*==============================================================================
** usage: displays the usage to the user if the program is incorrectly invoked.
**	input- the string of the program executable name.
**	output- usage message for correct program invokation to stderr.
**============================================================================*/

void usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s [-h \"Hotel name\" \"Hotel city\"]\n", prog_name);
	exit(EXIT_FAILURE);
}


/*==============================================================================
** getopthotel: takes the command line arguments and turns them into a 
**							Hotels_record node.
**	input- a hotel name string, and a hotel city string.
**	output- a node of type Hotels_record, with the input as its values.
**============================================================================*/

Hotels_record *getopthotel(char *hotel_name, char *hotel_city)
{
	Hotels_record *node;	

	node = malloc(sizeof(Hotels_record));

	strcpy(node->hotel_name, hotel_name);
	strcpy(node->hotel_city, hotel_city);
	node->next = NULL;

	return node;
}


/*==============================================================================
** print_out: prints out the linked list of output nodes in the format as
**						specified in the spec sheet and at the header of this file.
**	input- the linked list of output nodes (see owners.h).
**	output- to stdout: the output as demonstated in the header of this file.
**============================================================================*/

void print_out(Output *output)
{
	Output *curr_ptr ;
	Company *company_curr ;

	printf("\nReport on hotel owners\n");
	printf("======================\n");

	curr_ptr = output;

	while (curr_ptr != NULL) {
		printf("%s,  %s",curr_ptr->hotel_name, curr_ptr->hotel_city);
		if (curr_ptr->company_head == NULL)
			printf("\n");
		else {
			printf(":\n");
			company_curr = curr_ptr->company_head;
			while (company_curr != NULL) {
				printf("\t%s, %s (%.1f%\%)\n",company_curr->company_name, company_curr->company_city, 
							 company_curr->percentage);
				company_curr = company_curr->next;
			}
		}
		curr_ptr = curr_ptr->next;
	}
	printf("\n");

	return;
}
