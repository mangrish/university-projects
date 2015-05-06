/*
 *------------------------------------------------------------
 *
 *	File..........:	disk.c
 *	Author........:	Gary Eddy (gary@cs.mu.OZ.AU)
 *	Date..........: March 27 1996
 *	Purpose.......:	Disk scheduling algorithms.
 *	Revision History: Paolo Busetta, March 1997 - Minor changes
 *	                  Peter Eckersley, March 2000 - Added cache code
 *										Mark Angrish, April 2001 - Added sstf, cscan & vscan code
 *			  
 *	Description:
 *		implements 4 disk scheduling algorithms; sstf, fcfs, cscan
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

typedef	enum {UP, DOWN}	Direction; /* direction variable */
typedef struct disk_rec disk;

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
static 	disk	*ahead_q_head,  *ahead_q_curr; /*pointers for procs ahead of disk head*/
static	disk	*behind_q_head, *behind_q_curr;/*pointers for procs behind disk head*/
static	int	last_busy;
static int serviced_flag;/*flag for vscan, to know when last back req was serviced*/

static Direction direction; /*direction variable can be up or down*/
static double vscan_penalty;/* penalty for vscan*/
static int vdisk_size;/*disk size variable for vscan*/
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
 */

void
sstf_init(int startup, int pertrack, int disk_size, int cachesize, 
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
} /* sstf_init() */

/*
 *	sstf_schedule():
 */

void
sstf_request(int proc, int track, ReadOrWrite type)
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

	return;

} /* sstf_schedule() */


/*
 *	sstf_schedule():
 *		This algorithm finds the shortest seek-time by going through
 *		the request queue and comparing each seek-time with the shortest
 *		seek-time so far.
 */


void
sstf_schedule(int current_time, int *delay, int *nextproc)
{
	disk	*this , *prev , *shortest_seek , *before_ss = NULL;
	int closest_track;	
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

	closest_track = abs(curr_track - this->track);
	shortest_seek = this;
	prev = this;
	this = this->next;

	while (this !=NULL ) {
		if ( abs(curr_track - this->track) < closest_track ) {
			closest_track = abs(curr_track - this->track);
			shortest_seek = this;
			before_ss = prev;
		}
		prev = this;
		this = this->next;
	}


	/* is the data in the cache ? */
	if ((in_cache = is_in_cache(shortest_seek->track))) {
		if (shortest_seek->type == Read) 
			num_cache_hits++;
	} else {
		add_to_cache(shortest_seek->track);
		if (shortest_seek->type == Read)
			num_cache_misses++;
	}

	/* decide if we have to move the disk head */
	/* note that we are not doing write-behind caching */
	if ((shortest_seek->type == Write) || !in_cache) 
		mustmove = abs(curr_track-shortest_seek->track);
	num_tracks += mustmove;
	if(mustmove > 0) {
		num_seeks++;
		*delay = mustmove*track_cost + start_cost;
		curr_track = shortest_seek->track;
	} else 
		*delay = 0;
	/* (Getting data from the cache or the current track takes "no time") */

	*nextproc = shortest_seek->proc;

	if ( before_ss == NULL && shortest_seek->next == NULL )/* if only one in list */
		disk_q_head = disk_q_tail = NULL;
	else if ( before_ss != NULL && shortest_seek->next != NULL ) /* if in middle */
		before_ss->next = shortest_seek->next;
	else if ( before_ss == NULL )/* if at head */
		disk_q_head = shortest_seek->next;
	else if ( shortest_seek->next == NULL ) {	/* if at tail */
		disk_q_tail = before_ss;
		before_ss->next = NULL;
	}

	free(shortest_seek);

} /* sstf_request() */


/********************************************************************
 * Code for the `cscan' algorithm.
 ********************************************************************/

/*
 *      cscan_init():
 *              initialise the state of the disk.
 */

void
cscan_init(int startup, int pertrack, int disk_size, int cachesize,
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
	ahead_q_head = ahead_q_curr = NULL;
	behind_q_head = behind_q_curr = NULL;
	return;

} /* cscan_init() */

/*
 *      cscan_schedule():
 */

void
cscan_schedule(int current_time, int *delay, int *nextproc)
{
	disk	*this;
	int	mustmove = 0;
	int	in_cache = FALSE;

	this = ahead_q_head;

	if (this == NULL){
		ahead_q_head = behind_q_head;
		behind_q_head = NULL;
		this = ahead_q_head;
	}

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
	idle =0;

	ahead_q_head = ahead_q_head->next;

	/* is the data in the cache ? */
	if ((in_cache = is_in_cache(this->track))) {
		fprintf(stderr, "hi me %i \n" , in_cache);
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
	/* (Getting data from the cache or the current track takes "no time")*/ 

	*nextproc = this->proc;
	fprintf(stderr, "track %i serviced\n",this->track);

	free(this);

} /* cscan_schedule() */

/*
 *      cscan_request():
 */

void
cscan_request(int proc, int track, ReadOrWrite type)
{
	disk	*new, *prev;

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
	
	if (idle == 1) {
		fprintf(stderr,"idle head\n");
		if (new->track > curr_track ) {
			direction = UP;
		}
		else  {
			direction = DOWN;
		}
	}

	ahead_q_curr = ahead_q_head;
	behind_q_curr=behind_q_head;
	prev = NULL;
	fprintf(stderr,"new->track = %i  this track = %i dir = %i \n",new->track, curr_track, direction);

	if((ahead_q_head == NULL && direction == UP && new->track >= curr_track) ||
		(ahead_q_head == NULL && direction == DOWN && new->track <= curr_track)) {
		fprintf(stderr,"added track to front of ahead_q & dir is %i\n", direction);
		ahead_q_head = new;
		new->next = NULL;
		return;
	}

	else if ((behind_q_head == NULL && direction == UP && new->track < curr_track) ||
					 (behind_q_head ==NULL &&direction ==DOWN && new->track >curr_track)){
		fprintf(stderr,"added track to front of behind_q & dir is %i\n", direction);
		behind_q_head = new;
		new->next = NULL;
		return;
	}

	else if (ahead_q_head != NULL && direction == UP && curr_track <= new->track){
		fprintf(stderr,"added track to sumwhere of ahead_q & dir is %i\n", direction);
		while (new->track >= ahead_q_curr->track ) {
				prev = ahead_q_curr;
				ahead_q_curr = ahead_q_curr->next;
				if (ahead_q_curr == NULL) {break;}
		}
		if (prev == NULL){
			ahead_q_head = new;
			new->next = ahead_q_curr;
		}
		else {
			prev->next = new;
			new->next = ahead_q_curr;
		}
		return;
	}

	else if (ahead_q_head != NULL && direction == DOWN && new->track <= curr_track ) {
		fprintf(stderr,"added track to sumwhere of ahead_q & dir is %i\n", direction);
		while (new->track <= ahead_q_curr->track) {
			prev = ahead_q_curr;
			ahead_q_curr = ahead_q_curr->next;
			if (ahead_q_curr == NULL) {break;}
		}
		if (prev ==NULL) {
			ahead_q_head = new;
			new->next = ahead_q_curr;
		}
		else {
			prev->next = new;
			new->next = ahead_q_curr;
		}
		return;
	}	

	else if (behind_q_head !=NULL && direction == UP && curr_track > new->track){
		fprintf(stderr,"added track to sumwhere of behind_q & dir is %i\n", direction);
	  while (new->track  >= behind_q_curr->track)  {
			prev = behind_q_curr;
	    behind_q_curr = behind_q_curr->next;
			if (behind_q_curr == NULL) {break;}
	  }
		if (prev == NULL ) {
			behind_q_head = new;
			new->next = behind_q_curr;
		}
		else {
			prev->next = new;
	  	new->next = behind_q_curr;
		}
		return;
	}

	else if (behind_q_head !=NULL && direction == DOWN && new->track > curr_track) {
		fprintf(stderr,"added track to sumwhere of behind_q & dir is %i\n", direction);
		 while (new->track <= behind_q_curr->track) {
			 prev = behind_q_curr;
			 behind_q_curr = behind_q_curr->next;
			 if (behind_q_curr == NULL) {break;}
		  }
			if (prev == NULL ) { 
				behind_q_head = new;
				new->next = behind_q_curr;
			}
			else {
		  	prev->next = new;
		  	new->next = behind_q_curr;
			}
			return;
	 }

} /* 

*******************************************************************
 * Code for vscan.
 ********************************************************************/
/*
 *	vscan goes through the request queue and finds the closest 
 *	requests in front and behind the disk head. After that it compares
 *	the two requests and chooses the request with to lowest seek 'cost'.
 */


/*
 *	vscan_init():
 *		initialise the state of the disk.
 */

void
vscan_init(int startup, int pertrack, int disk_size, int cachesize, 
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
  ahead_q_head = ahead_q_curr = NULL;
	behind_q_head = behind_q_curr = NULL;
	vscan_penalty = penalty;
	vdisk_size = disk_size;
	serviced_flag = 0;

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
  disk  *this, *behind;
	int mustmove = 0;
	int in_cache = FALSE;
	double front_cost;
	double behind_cost;

	this = ahead_q_head;
	behind = behind_q_head;

	if (serviced_flag == 1) { serviced_flag =2;}
	if (serviced_flag == 2) { serviced_flag = 0;}

	if (this == NULL && behind != NULL){
		ahead_q_head = behind_q_head;
		behind_q_head = NULL;
		this = ahead_q_head;
		if (direction == UP) {
			direction = DOWN;
		}
		else {
			direction = UP;
		}
	}


	/* There are no requests left and nothing for the disk to do */
	if(this == NULL && behind == NULL) {
		idle = 1;
	  last_busy = current_time;
	  *nextproc = -1;
	  return;
	}
  /* How long has the disk been idle? */
  if(idle == 1) {
    time_waiting += (current_time-last_busy);
  }
	idle =0;

	if ( behind != NULL && serviced_flag == 0) {

		front_cost = abs(this->track - curr_track);
		behind_cost = abs(behind->track - curr_track) + vscan_penalty*vdisk_size;

		if ( front_cost <= behind_cost) { 
			ahead_q_head = ahead_q_head->next;
		}
		else {
			this = behind;
			behind_q_head = behind_q_head->next;
			serviced_flag = 1;
		}
	}
	else  {
		ahead_q_head = ahead_q_head->next;
	}

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

} /* vscan_schedule() */

/*
 *	vscan_request():
 */

void
vscan_request(int proc, int track, ReadOrWrite type)
{
	disk  *new, *prev;

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

  if (idle == 1) {
	  if (new->track >= curr_track ) {
	    direction = UP;
    }
	  else {
		  direction = DOWN;
		}
	}
  ahead_q_curr = ahead_q_head;
	behind_q_curr=behind_q_head;
	prev = NULL;

	if(ahead_q_head == NULL && ((direction == UP && new->track >= curr_track) ||
	                            (direction == DOWN && new->track <= curr_track))){
		ahead_q_head = new;
	  new->next = NULL;
	  return;
	}

	else if ((behind_q_head == NULL && direction == UP && new->track < curr_track)
					||(behind_q_head ==NULL &&direction ==DOWN && new->track >curr_track)){
		behind_q_head = new;
	  new->next = NULL;
	  return;
	}

	else if (direction == UP && curr_track <= new->track){
	  while (new->track >= ahead_q_curr->track ) {
	  	prev = ahead_q_curr;
      ahead_q_curr = ahead_q_curr->next;
			if (ahead_q_curr == NULL) {break;}
		}
		if (prev == NULL){
		  ahead_q_head = new;
		  new->next = ahead_q_curr;
		}
		else {
		  prev->next = new;
		  new->next = ahead_q_curr;
	  }
	  return;
	}

	else if (direction == DOWN && new->track <= curr_track ) {
		while (new->track <= ahead_q_curr->track) {
		  prev = ahead_q_curr;
	    ahead_q_curr = ahead_q_curr->next;
	    if (ahead_q_curr == NULL) {break;}
	  }
	  if (prev ==NULL) {
	    ahead_q_head = new;
	    new->next = ahead_q_curr;
		}  
	  else {
		  prev->next = new;
		  new->next = behind_q_curr;
		}
		return;
	}

	else if (direction == UP && curr_track > new->track){
	  while (new->track  < behind_q_curr->track)  {
			prev = behind_q_curr;
		  behind_q_curr = behind_q_curr->next;
		  if (behind_q_curr == NULL) {break;}
		}
		if (prev == NULL ) {
		  behind_q_head = new;
		  new->next = behind_q_curr;
		}
		else {
		   prev->next = new;
		 	 new->next = behind_q_curr;
		}
		return;
	}

	else if (direction == DOWN && new->track >curr_track) {
	  while (new->track > behind_q_curr->track) {
       prev = behind_q_curr;
			 behind_q_curr = behind_q_curr->next;
			 if (behind_q_curr == NULL) {break;}
		}
		if (prev == NULL ) {
		   behind_q_head = new;
		   new->next = behind_q_curr;
	  }
	  else {
		   prev->next = new;
		   new->next = behind_q_curr;
	  }
	  return;
	}

  return;
} /* vscan_request() */
