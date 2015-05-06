/*
 *------------------------------------------------------------
 *
 *	File..........:	event.c
 *	Author........:	Gary Eddy (gary@cs.mu.OZ.AU)
 *			(adapted from code by Zoltan Somogyi)
 *	Date..........: March 27 1996
 *	Purpose.......:	Event driven simulator
 *	Revision History: Paolo Busetta, March 1997 - minor changes.
 *                    Peter Eckersley, March 2000 - added cache code.
 *
 *	Description:
 *		Controls the sequence of events in the simulator
 *		determining the time at which events happen 
 *		relative to each other.
 *
 *		Events are placed in a linked-list which records
 *		the relative time, the process causing the event
 *		the disk track involved, and the type of event.
 *		The two types of event are requests and 
 *		completions. The first is an indication that a
 *		process requires data from the disk and the
 *		second indicates that the disk has serviced 
 *		such a request.
 *
 *		Each 'process' has a file associated with it
 *		contains a number of lines containing 2 integers.
 *		The first indicates how long the process will
 *		wait to issue it's next request. All processes 
 *		are assumed to start at time 0. 
 *		
 *
 *------------------------------------------------------------
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"event.h"
#include	"disk.h"

#define	REQUEST		0
#define	COMPLETION	1

typedef struct event_rec event;

struct event_rec {
	int	time;
	int	type;
	int	proc;
	int	track;
	ReadOrWrite rw;
	event	*next;
};

void	event_insert(event *new);

FILE	**fp; 
int	num_files;
event	*event_q;

/*
 *	init_event():
 *		initialises all of the processes involved in the 
 *		simulation, opening all files and queueing the first
 *		request from each.
 */

void
init_event(int files, char **filenames)
{
	int	i;
	event	*curr;

	event_q = NULL;
	num_files = files;
	fp = (FILE **) malloc(sizeof(FILE *)*files);
	if(fp == NULL) {
		fprintf(stderr, "Ran out of memory.\n");
		exit(1);
	}
	for(i = 0; i < files; i++) {
		fp[i] = fopen(filenames[i], "r");
		if(fp[i] == NULL) {
			fprintf(stderr, "Couldn't open simulation file: %s\n", 
				filenames[i]);
			exit(1);
		}
		curr = (event *) malloc(sizeof(event));
		if(fscanf(fp[i], "%d %d %d\n", 
				&(curr->time), &(curr->track), (int *) &(curr->rw))
			!= 3) {
			fprintf(stderr, "Malformed input file: %s\n",
				filenames[i]);
			exit(1);

		}
		if (curr->rw < 0 || curr->rw > 1) {
			fprintf( stderr, "Invalid request type %d\n", curr->rw );
			exit( EXIT_FAILURE);
		}

		curr->proc = i;
		curr->type = REQUEST;
		event_insert(curr);
	}
} /* init_event() */

/*
 *	simulation():
 *		the main loop of the simulation. While there are
 *		any events remaining on the queue extract and 
 *		process the first.
 *
 *		There are two types of events, requests and
 *		completions. The first requires only that the
 *		relavent information is passed to the disk scheduler
 *		and if the disk is idle that a new request is
 *		scheduled. Completions are slightly more complicated.
 *		A new process must be scheduled once again, but also 
 *		the time of the next request from the process
 *		associated with the completion is also calculated
 *		and added to the event queue.
 */

int
simulation( void (*schedule)(int, int *, int *), 
            void (*request)(int, int, ReadOrWrite))
{
	int	time_now = 0;
	event	*this, *new;
	int	finish, got;

	/* While there are events to be processed */
	while(event_q != NULL) {
		this = event_q;
		event_q = event_q->next;

		time_now = this->time;
		if(this->type == COMPLETION) {
			int	current_proc;

			schedule(time_now, &finish, &current_proc);
			if(current_proc != -1) {
				/*
				** queue the completion of the current
				** operation performed by the disk.
				*/
				new = (event *) malloc(sizeof(event));
				if(new == NULL) {
					fprintf(stderr, "Ran out of memory.\n");
					exit(1);
				}
				new->proc = current_proc;
				new->type = COMPLETION;
				new->time = time_now+finish;
				event_insert(new);
			}
			/* 
			** read the time and place of the next request 
			** of the process whose request just completed.
			*/
			got = fscanf(fp[this->proc], "%d %d %d", &(this->time),
				&(this->track), (int *) &(this->rw));
			if(got == EOF) {
				/* The end of the process */
				free(this);
			}
			else if(got != 3) {
				fprintf(stderr, "Malformed input: process %d\n",
					this->proc);
				exit(1);
			}
			else {
				if (this->rw < 0 || this->rw > 1) {
					fprintf(stderr, "Invalid request type %d)\n",this->rw);
					exit( EXIT_FAILURE);
				}
				this->time += time_now;
				this->type = REQUEST;
				event_insert(this);
			}
		}
		else { /* REQUEST */
			request(this->proc, this->track, this->rw);
			if(disk_idle()) {
				/* 
				** The disk is not busy. Start servicing
				** the request now.
				*/
				int working_for;
				schedule(time_now, &finish, &working_for);
				if (working_for != this->proc) {
					fprintf(stderr, 
					    "Expected process %d, got %d\n",
					    this->proc, working_for);
					exit(1);
				};
				this->type = COMPLETION;
				this->time = time_now+finish;
				event_insert(this);
			}
			else
				free(this);
		}
	}
	/* return the total time taken by the simulation */
	return time_now;
} /* simulation() */

/*
 *	event_insert():
 *		simple linked-list insertion in sorted order.
 *		items with equal times are inserted such that
 *		they are extracted in a first-come-first-served
 *		manner.
 */

void
event_insert(event *new)
{
	event	*prev, *curr;

	for (prev = NULL, curr = event_q; 
	     curr && (curr->time <= new->time); 
	     prev = curr, curr = curr->next)
	     ;
	new->next = curr;
	if(prev != NULL)
		prev->next = new;
	else
		event_q = new;
} /* event_insert() */
