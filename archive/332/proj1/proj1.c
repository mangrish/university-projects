/*
 *------------------------------------------------------------
 *
 *	File..........:	proj1.c
 *	Author........:	Gary Eddy (gary@cs.mu.OZ.AU)
 *			(adapted from code by Zoltan Somogyi)
 *	Date..........: March 27 1996
 *	Purpose.......:	Driver unit for 433-332 project 1 
 *	Revision History: Paolo Busetta, March 1997 - Updated prototypes
 *
 *	Description:
 *		drives a disk head scheduling simulation
 *		program.
 *
 *------------------------------------------------------------
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	"event.h"
#include	"disk.h"

#define	DISK_SIZE	128
#define	START_COST	10
#define	TRACK_COST	1
#define CACHE_SIZE 0

/*	Function declarations:	*/
int	main(int argc, char **argv);
void	usage(char *name);

/*	Solaris and IRIX are inconsistent about where srandom() is defined */
/* int	srandom(unsigned seed); */

/*	Extern declarations: */
extern	int	optind;
extern	char	*optarg;

/*
 *	main():
 *		does general initialisation for the program including 
 *		the processing of options, for which getopt is used.
 *		The default is to use first-come-first-served as the
 *		scheduling algorithm but the disk_init(), schedule()
 *		and request() function pointers are initialised to
 *		whichever algorithm is to be used.
 *
 *		The 'penalty' variable passed to disk_init() is necessary
 *		for the vscan algorithm only.
 */

int
main(int argc, char **argv)
{
	int	c, end_time;
	double	penalty = 0.0;
	int	disk_head = 0;
	int	start_cost = START_COST, track_cost = TRACK_COST;
	int	disk_size = DISK_SIZE;
	int cache_size = CACHE_SIZE;
	void	(*schedule)(int, int *, int*), (*request)(int, int, ReadOrWrite);
	void	(*disk_init)(int, int, int, int, double, int);


	disk_init = fcfs_init;
	schedule = fcfs_schedule;
	request = fcfs_request;

	while ((c = getopt(argc, argv, "i:p:a:s:t:d:c:")) != EOF)
	{
		switch ( c )
		{
			case 's':
				start_cost = atoi(optarg);
				break;
			case 't':
				track_cost = atoi(optarg);
				break;
			case 'a':
				if(strcmp(optarg, "cscan") == 0) {
					disk_init = cscan_init;
					schedule = cscan_schedule;
					request = cscan_request;
				}
				else if(strcmp(optarg, "vscan") == 0) {
					disk_init = vscan_init;
					schedule = vscan_schedule;
					request = vscan_request;
				}
				else if(strcmp(optarg, "sstf") == 0) {
					disk_init = sstf_init;
					schedule = sstf_schedule;
					request = sstf_request;
				}
				else if(strcmp(optarg, "fcfs") != 0) {
					fprintf(stderr, "Unknown algorithm: %s\n", optarg);
					exit(1);
				}
				break;
			case 'p':
				if(sscanf(optarg, "%lf", &penalty) == 0) {
					fprintf(stderr, "Invalid penalty.\n");
					exit(1);
				}
				break;
			case 'i':
				disk_head = atoi(optarg);
				break;
			case 'd':
				disk_size = atoi(optarg);
				break;
			case 'c':
				cache_size = atoi(optarg);
				break;
			default:
				usage(argv[0]);
				break;
		} /* switch */
	} /* while */

	/* There must be at least one data file to process */
	if(optind >= argc)
		usage(argv[0]);
	
	/* seed random number generator random() */
	srandom(getpid());
	disk_init(start_cost, track_cost, disk_size, cache_size, penalty, disk_head);
	init_event(argc-optind, argv+optind);
	end_time = simulation(schedule, request);
	print_results(end_time);
	return 0;
} /* main() */

/*
 *	usage():
 *		print out correct invocation of the program.
 */

void
usage(char *name)
{
	char	*ptr;

	/* want basename of program */

	ptr = strrchr(name, '/');
	if(ptr == NULL)
		ptr = name;
	else
		ptr++;

	fprintf(stderr, "usage: %s [-s start_cost] [-t track_cost] %s%s\n", 
		ptr, "[-a algorithm] [-p penalty] [-c cache_size]\n", 
		"       [-i start_track] sim_file1 sim_file2 ...");
	exit(1);
} /* usage() */

