/*
 *------------------------------------------------------------
 *
 *	File..........:	disk.c
 *	Author........:	Gary Eddy (gary@cs.mu.OZ.AU)
 *	Date..........: March 27 1996
 *	Purpose.......:	Disk scheduling algorithms.
 *	Revision History: Paolo Busetta, March 1997 - Minor changes
 *	                  Peter Eckersley, March 2000 - Added cache code
 *			  David Sanders, April 2000 - Added algorithm code
 *			  
 *	Description:
 *		implements 4 disk scheduling algorithms; sstf, fcfs, scan
 *		and vscan. The interface with the driver consists of the
 *		functions for each init() for any initialisation required by
 *		the algorithm, request() to receive new requests and
 *		schedule() to indicate how long the disk will be busy with a
 *		request and which process made the request.
 *
 *		Also to algorithm independant functions which indicate disk
 *		status, disk_idle() and to print statistics of the simulation,
 *		print_results().
 *
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"disk.h"
#include	"std.h"

#define		MIDDLE_TRACK 50

typedef struct disk_rec disk;

/* Additional */
typedef	enum {Up, Down}	Direction;

struct disk_rec {
	int		proc;
	int		track;
	disk		*next;
	ReadOrWrite	type;
};


static	int 	cache_size;
static	int	num_reads, num_writes;
static	int	num_cache_hits, num_cache_misses;
static	int	num_requests, num_seeks, num_tracks;
static	int	start_cost, track_cost;
static	int	idle;
static	int	curr_track;
static	int	time_waiting;
static	disk	*disk_q_head, *disk_q_tail;
static	int	last_busy;

/* Additional */
static Direction direction;
static	double	vscan_penalty;
static	int	diskSize;


/*
 *	disk_idle():
 *		return disk status. It _IS_ too small to be a function
 *		but is defined this way so that all disk information
 *		can be kept in this file. 
 *		Do not change this function.
 */

int
disk_idle()
{
	return idle;
} /* disk_idle() */

/*
 *	print_results():
 *		display the results of the simulation when it is complete.
 *		Do not change this function.
 */

void
print_results(int end_time)
{
	printf("time at completion:\t\t%d\n", end_time);
	printf("time spent idle:\t\t%d\n", time_waiting);
	printf("number of requests:\t\t%d\n", num_requests);
	printf("number of reads/writes:\t\t%d/%d\n", num_reads, num_writes);
	printf("number of cache hits/misses:\t%d/%d\n",
	       num_cache_hits, num_cache_misses);
	printf("number of seeks executed:\t%d\n", num_seeks);
	printf("number of tracks travelled:\t%d\n", num_tracks);
	printf("average time/request:\t\t");
	printf("%.3f\n", 
	     (num_requests > 0) ? ((double) end_time)/num_requests : 0.0 );
	printf("average seeks/request:\t\t");
	printf("%.3f\n", 
	     (num_requests > 0) ? ((double) num_seeks)/num_requests : 0.0 );
	printf("average tracks/seek:\t\t");
	printf("%.3f\n", 
	     (num_seeks > 0) ? ((double) num_tracks)/num_seeks : 0.0 );
} /* print_results() */

/*
 * These are the nodes for the Least Recently Used (LRU) queue the cache uses
 * to keep track of its contents and decide which tracks should be flushed
 * to free space
 */

typedef struct cached_track {
	struct cached_track *next, *prev;
	int track;
} Cache_entry;

static Cache_entry *cache_head = NULL;
static Cache_entry *cache_tail = NULL;
static int num_cache_entries = 0;

/*
 * cache_allocate():
 * helper function to allocate LRU queue entries
 */

Cache_entry *cache_allocate( int track )
{
	Cache_entry *entry;
	entry = malloc( sizeof(Cache_entry) );
	if (entry == NULL) {
		fprintf( stderr, "Memory allocation error\n" );
		exit( EXIT_FAILURE );
	}
	entry->track = track;
	entry->next = NULL;
	entry->prev = NULL;
	return entry;
}
/*
 * is_in_cache():
 * returns TRUE if a track is in the cache, FALSE if it is not.
 * If the track is in the cache, it will be moved to the front of the LRU queue
 * so that it will not be flushed in the near future.
 */

int is_in_cache(int track)
{
	Cache_entry *curr;
	
	for (curr = cache_tail; curr != NULL; curr=curr->next) 
		if (curr->track == track) {
			if (curr->next == NULL)
				return TRUE;  /*Track is already at the front*/

			curr->next->prev = curr->prev;
			cache_head->next = curr;
			if (curr->prev != NULL)
				curr->prev->next = curr->next;
			else 
				cache_tail = curr->next;
			curr->next = NULL;
			curr->prev = cache_head;
			cache_head = curr;
			return TRUE;
		}
	return FALSE;
}

/*
 * add_to_cache():
 * place this track at the front of the cache's LRU queue, removing the
 * Least Recently Used track if necessary
 */

void add_to_cache(int track)
{
	if (num_cache_entries == 0) {
		if (cache_size > 0) {
			cache_head = cache_tail = cache_allocate( track );
			num_cache_entries++;
			return;
		} else
			return;
	}

	cache_head->next = cache_allocate( track );
	cache_head->next->prev = cache_head;
	cache_head = cache_head->next;

	if (num_cache_entries == cache_size) {
		Cache_entry *temp;
		temp = cache_tail;
		cache_tail = cache_tail->next;
		cache_tail->prev = NULL;
		free(temp);
	} else
		num_cache_entries++;
}


/********************************************************************
 * Code for first come first served.
 ********************************************************************
 *
 * Uses a simple linked-list to queue requests.
 *
 */

/*
 *	fcfs_init():
 *		initialise the state of the disk.
 */

void
fcfs_init(int startup, int pertrack, int disk_size, int cachesize,
	double penalty, int disk_head)
{
	num_tracks = 0;
	num_seeks = 0;
	num_requests = 0;
	num_cache_hits = num_cache_misses = 0;
	num_reads = num_writes = 0;
	time_waiting = last_busy = 0;
	idle = 1;
	start_cost = startup;
	track_cost = pertrack;
	curr_track = disk_head;
	cache_size = cachesize;
	disk_q_head = disk_q_tail = NULL;
	return;
} /* fcfs_init() */

/*
 *	fcfs_request():
 *		simple first-come-first-served scheduling. Just inserts
 *		the new request at the end of a linked-list. Maintains
 *		pointers to the head and tail of the list so that
 *		insertion AND deletion are both constant time opererations.
 */

void
fcfs_request(int proc, int track, ReadOrWrite type)
{
	disk	*new;

	num_requests++;
	if (type == Read)
		num_reads++;
	else
		num_writes++;
	new = (disk *) malloc(sizeof(disk));
	if(new == NULL) {
		fprintf(stderr, "Ran out of memory.\n");
		exit(1);
	}
	new->proc = proc;
	new->track = track;
	new->type = type;
	if(disk_q_head == NULL) {
		disk_q_head = disk_q_tail = new;
		new->next = NULL;
	}
	else {
		disk_q_tail = disk_q_tail->next = new;
		new->next = NULL;
	}

} /* fcfs_request() */

/*
 *	fcfs_schedule():
 *		determines the next request to be serviced. In this case
 *		just takes the head of the linked-list built up by 
 *		fcfs_request(). Also maintains the amount of time the disk
 *		has spent idle, the number of tracks seeked over, the time
 *		spent rotating the disk and the number of seeks performed.
 *
 *		When invoked, the current_time variable must contain the 
 *		current simulated time.
 *
 *		Returns the time the next request will take via the 
 *		delay variable and which process is involved via the
 *		nextproc variable.
 */

void
fcfs_schedule(int current_time, int *delay, int *nextproc)
{
	disk	*this;
	int	mustmove = 0;
	int	in_cache = FALSE;

	this = disk_q_head;
	/* There are no requests left and nothing for the disk to do */
	if(this == NULL) {
		idle = 1;
		last_busy = current_time;
		*nextproc = -1;
		return;
	}
	/* How long has the disk been idle? */
	if(idle == 1) {
		time_waiting += (current_time-last_busy);
	}
	idle = 0;
	disk_q_head = disk_q_head->next;

	/* is the data in the cache ? */
	if ((in_cache = is_in_cache(this->track))) {
		if (this->type == Read) 
			num_cache_hits++;
	} else {
		add_to_cache(this->track);
		if (this->type == Read)
			num_cache_misses++;
	}

	/* decide if we have to move the disk head */
	/* note that we are not doing write-behind caching */
	if ((this->type == Write) || !in_cache) 
		mustmove = abs(curr_track-this->track);
	num_tracks += mustmove;
	if(mustmove > 0) {
		num_seeks++;
		*delay = mustmove*track_cost + start_cost;
		curr_track = this->track;
	} else 
		*delay = 0;
	/* (Getting data from the cache or the current track takes "no time") */
	*nextproc = this->proc;
	free(this);
} /* fcfs_schedule() */

/********************************************************************
 * Code for the `sstf' algorithm
 ********************************************************************/
/*
 *	This algorithm goes through the queue and finds the request with 
 *	the shortest seek time.
 *	This algorithm uses a queue the same way as fcfs does, so
 *	that when more than one request has the shortest seek time, it uses
 *	the one that came first.
 */

/*
 *	sstf_init():
 *		initialise the state of the disk.
 *		Seeing as this initializes the disk the same as fcfs_init does
 *		it just calls fcfs_init (for reduced program size).
 */

void
sstf_init(int startup, int pertrack, int disk_size, int cachesize, 
	double penalty, int disk_head)
{
	fcfs_init(startup, pertrack, disk_size, cachesize, penalty, disk_head);
	return;

} /* sstf_init() */

/*
 *	sstf_schedule():
 *		This algorithm finds the shortest seek-time by going through
 *		the request queue and comparing each seek-time with the shortest
 *		seek-time so far.
 */

void
sstf_schedule(int current_time, int *delay, int *nextproc)
{
	disk *this, *prev, *ideal, *prev_ideal;
	int shortest_seek;
	int mustmove = 0;
	int in_cache = FALSE;

	this = disk_q_head;

	/* if there are no more requests */
	if ( this == NULL ) {
		idle = 1;	
		last_busy = current_time;
		*nextproc = -1;
		return;
	}

	/* How long has the disk been idle? */
	if ( idle == 1 )
		time_waiting += (current_time-last_busy);
	idle = 0;
	
	prev = NULL;
	prev_ideal = NULL;
	shortest_seek = abs(curr_track - this->track);
	ideal = this;
	prev = this;
	this = this->next;
	while ( this != NULL ) {
		if ( abs(curr_track - this->track) < shortest_seek ) {
			shortest_seek = abs(curr_track - this->track);
			ideal = this;
			prev_ideal = prev;
		}
		prev = this;
		this = this->next;
	}

	/* is the data in the cache? */
	if ((in_cache = is_in_cache(ideal->track))) {
		if ( ideal->type == Read )
			num_cache_hits++;
	} else {
		add_to_cache( ideal->track );
		if ( ideal->type == Read )
			num_cache_misses++;
	}

	/* decide if we have to move the disk head */
	/* note that we are not doing write-behind caching*/
	if (( ideal->type == Write ) || !in_cache )
		mustmove = shortest_seek;
	num_tracks += mustmove;
	if ( mustmove > 0 ) {
		num_seeks++;
		*delay = mustmove*track_cost + start_cost;

		/* set the direction variable correctly */
		if ( (ideal->track - curr_track) > 0 ) direction = Up;
		else direction = Down;

		curr_track = ideal->track;
	} else
		*delay = 0;

	*nextproc = ideal->proc;

	/* if only one in list */
	if ( prev_ideal == NULL && ideal->next == NULL )
		disk_q_head = disk_q_tail = NULL;
	/* if in middle */
	else if ( prev_ideal != NULL && ideal->next != NULL ) 
		prev_ideal->next = ideal->next;
	/* if at head */
	else if ( prev_ideal == NULL )
		disk_q_head = ideal->next;
	/* if at tail */
	else if ( ideal->next == NULL ) {
		disk_q_tail = prev_ideal;
		prev_ideal->next = NULL;
	}

	free(ideal);
		
	return;
} /* sstf_schedule() */

/*
 *	sstf_request():
 *		The request data structure was implemented the same as fcfs.
 */

void
sstf_request(int proc, int track, ReadOrWrite type)
{	
	fcfs_request( proc, track, type );
	return;
} /* sstf_request() */

/********************************************************************
 * Code for the `scan' algorithm.
 ********************************************************************/
/*
 *	This algorithm goes through the request queue and at first only
 *	considers requests with the shortest seek-time 'in front' of the 
 *	disk head. If there are no requests in front of the disk head,
 *	it then turns around and finds the closest request 'behind'.
 */

/*
 *	scan_init():
 *		initialise the state of the disk by 
 *		calling fcfs_init() 
 */

void
scan_init(int startup, int pertrack, int disk_size, int cachesize, 
	double penalty, int disk_head)
{		
	fcfs_init(startup, pertrack, disk_size, cachesize, penalty, disk_head);
	return;
} /* scan_init() */

/*
 *	scan_schedule():
 *		It works like this:
 *		1. If the disk head is idle it just finds the closest request
 *		2. It goes through and finds the closest request 
 *		   that is 'in front' of the disk head
 *		3. but if there is no requests in front it just finds the 
 *		   closest request and changes direction
 */

void
scan_schedule(int current_time, int *delay, int *nextproc)
{
        disk    *this, *ideal = NULL, *prev = NULL, *prev_ideal = NULL;
        int     mustmove = 0;
        int     in_cache = FALSE;

        this = disk_q_head;
        /* There are no requests left and nothing for the disk to do */
        if(this == NULL) {
                idle = 1;
                last_busy = current_time;
                *nextproc = -1;
                return;
        }

	if ( idle == 1 ) {
		/* find the closest request */	
		sstf_schedule( current_time, delay, nextproc );
		return;
	}

	/* if first request is a candidate then record it */
	if ( (direction == Up && (this->track >= curr_track)) ||
	     (direction == Down && (this->track < curr_track)) ) {
		ideal = this;
	}
	prev = this;
	this = this->next;
	while ( this != NULL ) { 

		/* if a candidate hasn't been recorded, */
		/* and 'this' is in front then record it */
		if ( (ideal == NULL) && 
		((direction == Up && (this->track >= curr_track))
		|| (direction == Down && (this->track < curr_track)))) {
			prev_ideal = prev;
			ideal = this;
		}
		/* if 'this' is better, record it */
		else if ( ideal != NULL && 
		((direction == Up && (ideal->track > this->track)&&
		(this->track >= curr_track)) || (direction == Down && 
		(ideal->track < this->track) && (this->track 
		< curr_track)))) {
			prev_ideal = prev;
			ideal = this;
		}
		
		/* move to next item in the list */
		prev = this;
		this = this->next;
	}

	/* did not find request in same direction */
	if (ideal == NULL) { 
		/* find the closest request */
		sstf_schedule( current_time, delay, nextproc );
		return;
	}	if ( (direction == Up && (this->track >= curr_track)) ||
	     (direction == Down && (this->track < curr_track)) ) {
		ideal = this;
	}
	prev = this;
	this = this->next;


        /* is the data in the cache ? */
        if ((in_cache = is_in_cache(ideal->track))) {
                if (ideal->type == Read)
                        num_cache_hits++;
        } else {
                add_to_cache(ideal->track);
                if (ideal->type == Read)
                        num_cache_misses++;
        }

        /* decide if we have to move the disk head */
        /* note that we are not doing write-behind caching */
        if ((ideal->type == Write) || !in_cache)
                mustmove = abs(curr_track - ideal->track);
        num_tracks += mustmove;
        if(mustmove > 0) {
                num_seeks++;
                *delay = mustmove*track_cost + start_cost;

		/* this sets the direction variable correctly */
		if ( (ideal->track - curr_track) > 0 ) direction = UP;
		else direction = Down;

		curr_track = ideal->track;
        } else
                *delay = 0;
        /* (Getting data from the cache or the current track takes "no time") */
        *nextproc = ideal->proc;

	/* if only one in list */
	if ( prev_ideal == NULL && ideal->next == NULL )
		disk_q_head = disk_q_tail = NULL;
	/* if in middle */
	else if ( prev_ideal != NULL && ideal->next != NULL ) 
		prev_ideal->next = ideal->next;
	/* if at head */
	else if ( prev_ideal == NULL )
		disk_q_head = ideal->next;
	/* if at tail */
	else if ( ideal->next == NULL ) {
		disk_q_tail = prev_ideal;
		prev_ideal->next = NULL;
	}

	free(ideal);

	return;
} /* scan_schedule() */

/*
 *	scan_request():
 *		just calls fcfs_request() to implement the queue
 */

void
scan_request(int proc, int track, ReadOrWrite type)
{
	fcfs_request( proc, track, type );
	return;
} /* scan_request() */

/********************************************************************
 * Code for vscan.
 ********************************************************************/
/*
 *	vscan goes through the request queue and finds the closest 
 *	requests in front and behind the disk head. After that it compares
 *	the two requests and chooses the request with to lowest seek 'cost'.
 */

/*
 *	vscan_init():
 *		initialise the state of the disk by 
 *		calling fcfs_init() but also initializes the vscan penalty
 *		and the disk size.
 */

void
vscan_init(int startup, int pertrack, int disk_size, int cachesize, 
	double penalty, int disk_head)
{	
	fcfs_init(startup, pertrack, disk_size, cachesize, penalty, disk_head);
	vscan_penalty = penalty;
	diskSize = disk_size;

	return;
} /* vscan_init() */

/*
 *	vscan_schedule():
 *		vscan works like this: If goes through the queue and finds the
 *		closest requests in front of and behind the disk head.  It then 
 *		compares the two request costs and chooses the best one. If 
 *		there is nothing in front, then it changes direction and uses 
 *		the closest request behind.  If there is nothing behind it just
 *		uses the closest request in front.  
 */

void
vscan_schedule(int current_time, int *delay, int *nextproc)
{       
        disk    *this, *ideal = NULL, *prev = NULL, *prev_ideal = NULL;
	disk	*front = NULL, *behind = NULL; 
	disk	*prev_front = NULL, *prev_behind = NULL;
        int     mustmove = 0;
        int     in_cache = FALSE;
	int	behind_seek;
	int	front_seek;

        this = disk_q_head;
        /* There are no requests left and nothing for the disk to do */
        if(this == NULL) {
                idle = 1;
                last_busy = current_time;
                *nextproc = -1;
                return;
        }

	if ( idle == 1 ) {
		/* find the closest request */	
		sstf_schedule( current_time, delay, nextproc );
		return;
	}

	/* see if first request is a valid candidate */
	if ( (direction == Up && (this->track >= curr_track)) ||
	     (direction == Down && (this->track < curr_track)) ) {
		front = this;
		front_seek = abs(this->track - curr_track);
	}
	else {
		behind = this;
		behind_seek = abs(this->track - curr_track);
	}

	prev = this;
	this = this->next;
	while ( this != NULL ) { 

		/* if 'this' is in front and it is a valid candidate */
		if ( ((direction == Up && (this->track >= curr_track)) ||
		     (direction == Down && (this->track < curr_track))) && 
		     ( (abs(this->track - curr_track) < front_seek) || 
		     front == NULL ) ) {
			/* then record it */
			prev_front = prev;
			front = this;
			front_seek = abs(this->track - curr_track);
		/* else if it is behind and a valid candidate */
		} else if ( abs(this->track - curr_track) < behind_seek 
			|| behind == NULL ){
			/* then record it */
			prev_behind = prev;
			behind = this;
			behind_seek = abs(this->track - curr_track);
		}

		/* move onto next item in the list */
		prev = this;
		this = this->next;
	}

	/* if nothing behind or in front - can't compare so just assign */
	if ( behind == NULL || front == NULL )
		/* if nothing behind */
		if ( behind == NULL ) {
			prev_ideal = prev_front;
			ideal = front;
		}
		/* if nothing in front */
		else { 
			prev_ideal = prev_behind;
			ideal = behind;
		}
	else {
		/* now to find which has the lower cost */
		if ( (behind_seek + vscan_penalty*diskSize) 
			< front_seek ) {
			ideal = behind;
				prev_ideal = prev_behind;
		} else {
			ideal = front;
			prev_ideal = prev_front;
		}
	}

        /* is the data in the cache ? */
        if ((in_cache = is_in_cache(ideal->track))) {
                if (ideal->type == Read)
                        num_cache_hits++;
        } else {
                add_to_cache(ideal->track);
                if (ideal->type == Read)
                        num_cache_misses++;
        }

        /* decide if we have to move the disk head */
        /* note that we are not doing write-behind caching */
        if ((ideal->type == Write) || !in_cache)
                mustmove = abs(curr_track - ideal->track);
        num_tracks += mustmove;
        if(mustmove > 0) {
                num_seeks++;
                *delay = mustmove*track_cost + start_cost;

		/* set the direction variable correctly */
		if ( (ideal->track - curr_track) > 0 ) direction = Up;
		else direction = Down;

		curr_track = ideal->track;
        } else
                *delay = 0;
        /* (Getting data from the cache or the current track takes "no time") */
        *nextproc = ideal->proc;

	/* if only one in list */
	if ( prev_ideal == NULL && ideal->next == NULL )
		disk_q_head = disk_q_tail = NULL;
	/* if in middle */
	else if ( prev_ideal != NULL && ideal->next != NULL ) 
		prev_ideal->next = ideal->next;
	/* if at head */
	else if ( prev_ideal == NULL )
		disk_q_head = ideal->next;
	/* if at tail */
	else if ( ideal->next == NULL ) {
		disk_q_tail = prev_ideal;
		prev_ideal->next = NULL;
	}

	free(ideal);

	return;
} /* vscan_schedule() */

/*
 *	vscan_request():
 *		calls fcfs_request() to implement queue
 */

void
vscan_request(int proc, int track, ReadOrWrite type)
{
	fcfs_request( proc, track, type );
	return;
} /* vscan_request() */
