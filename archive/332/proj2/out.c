============================================ vis/COMPILE
20:03:10_Thursday_24_May_2001
============================================ Compiling with gcc -Wall
-o deadlock *.c ...  deadlock3.c: In function `main':  deadlock3.c:84:
warning: passing arg 4 of `qsort' from incompatible pointer type
============================================ vis/case1.out
20:03:10_Thursday_24_May_2001
============================================

Deadlock was detected.  Abort processes : 2, 5 to resolve deadlock

============================================ src/deadlock3.c
20:03:02_Thursday_24_May_2001
============================================
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/* **  Program : deadlock.c:  **  For     : 433 - 332: Operating
Systems; **            PROJECT 2 **  Written : Mark Angrish, Student
number: 74592 **            May 2001; **  Input   : FILE containing the
system state as a command line argument; **  Output  : To stdout: 1)
report on whether or not the system is initally
**
deadlocked; and,.
**
2) if deadlock exists, describe which process(es)
**
should be aborted to resolve the deadlock, according
**
to the specifications.
**
e.g. > deadlock systemsatefile **
**
Deadlock was detected
**
Abort processes 2,5 to resolve deadlock **
**
> **      Questions: Refer to the file "comments".  */
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include <stdio.h> #include <stdlib.h> #include <ctype.h>

typedef struct process Process; typedef struct transaction_edge Edge;

struct process {
	int             PID; int             priority; int
	holding_num; int     *holding; int             requests_num;
	int             *requests; Process *next; };

struct transaction_edge {
	int PID; int priority; Edge *next; };

int deadlock = 0;

/*=============================================================================
**===== FUNCTION PROTOTYPES.
**=============================================================================*/

void            usage( char * ); void            print_results( int *,
int ); FILE            *open_file( char *, char *, const char * );
Process *make_process_node( FILE* ); Process *make_system_state( FILE*,
int * ); int                     *wait_for_graph(Process *, Process *,
int, int *); Edge            *locate_deadlock(Process **, Process **,
Edge *); Edge            *make_edge_node(int  , int); Process
*delete(Process *, int); Edge            *deletePID(Edge *, int);
static  int intcompare(int *, int *);

/*===========================================================================*/

int main( int argc, char *argv[] ) {
	char            *prog_name = argv[0]; FILE
	*ssf_fptr= NULL; Process *head; int
	*killed_array = NULL; int                     num_processes =
	0; int                     killed_size = 0;

	if (argc != 2)
		usage(prog_name);

	ssf_fptr = open_file(prog_name, argv[1], "r"); head =
	make_system_state(ssf_fptr, &num_processes); killed_array =
	wait_for_graph(head, head, num_processes, &killed_size);
	qsort((char *) killed_array,
	killed_size,sizeof(int),intcompare);
	print_results(killed_array, killed_size);

	fclose(ssf_fptr);

	return 0; }


/*=============================================================================
**===== usage: displays usage message to user, if program incorrectly
invoked.
**=============================================================================*/

void usage(char *prog_name) {
	fprintf(stderr, "Usage: %s [filename]\n", prog_name); exit(
	EXIT_FAILURE ); }


/*==============================================================================
**===== open_file: opens a file in read mode. If the file cannot be
opened, it **=====                                          reports
this to the user
**==============================================================================*/

FILE *open_file( char *prog_name, char *file_name, const char *mode) {
	FILE *file_ptr = NULL;

	file_ptr = fopen(file_name, mode);

	if (file_ptr == NULL) {
		fprintf(stderr, "%s: ERROR: File \"%s\" not opened; "
					  "program execution
					  terminated.\n", prog_name,
					  file_name);
		exit(EXIT_FAILURE); }

	return file_ptr; }

/*==============================================================================
**===== print_results:
**==============================================================================*/

void print_results( int *killed_array, int killed_size) {
	int i;

	if (killed_size != 0) {
		printf("\nDeadlock was detected.\n"); printf("Abort
		processes : "); for (i = 0; i< killed_size; i++) {
			if ((i+1) == killed_size)
				printf("%i ", killed_array[i]); else
				printf("%i, ", killed_array[i]);
		} printf("to resolve deadlock\n\n"); } else
		printf("\nDeadlock was not detected.\n\n");

	return; }


/*==============================================================================
**===== make_system_state:  **=====
**==============================================================================*/

Process *make_system_state(FILE *ssf_fptr, int *num_processes) {
	Process *process = NULL; Process *head = NULL; Process *tail =
	NULL;

	while ((process = make_process_node(ssf_fptr)) != NULL) {
		(*num_processes)++; if (head == NULL)
			head = tail= process; else {
			tail->next = process; tail = process; } }
	return head; }



/*==============================================================================
**===== make_process_node:  **=====
**==============================================================================*/

Process *make_process_node(FILE *file_ptr) {
	int i =0, j=0; int c; Process *new; int *requests; int
	*holding;

	new = malloc(sizeof (Process));

	if ((c = fscanf(file_ptr,"%*s%i%*s%i%*s",&new->PID,
	&new->priority)) == 0 || c == EOF)
		return NULL;

	holding = malloc((i+1) * sizeof(int)); while
	(fscanf(file_ptr,"%i",&holding[i]) != 0) {
		i++; holding = realloc(holding,((i+1) * sizeof(int)));
	} new->holding_num = i; new->holding = holding;

	if (i == 0)
		fscanf(file_ptr,"%*s"); fscanf(file_ptr,"%*s");

	requests = malloc((j+1) * sizeof(int)); while
	(fscanf(file_ptr,"%i",&requests[j]) != 0) {
		j++; requests = realloc(requests,((j+1) *
		sizeof(int))); } new->requests_num = j; new->requests =
	requests; new->next = NULL;

	if (j == 0)
		fscanf(file_ptr,"%*s");

	return new;

}

/*==============================================================================
**===== wait_for_graph:  **=====
**==============================================================================*/

int *wait_for_graph(Process *head, Process *current_process, int
num_processes, int *killed_size) {
	Edge *headPID_ptr = make_edge_node(head->PID, head->priority);
	Edge *lowest_priority; Edge *curr_ptr; int *killed_array;

	killed_array = malloc(num_processes * sizeof(int));

	while ((headPID_ptr = locate_deadlock(&head, &current_process,
	headPID_ptr)) != NULL){
		if (deadlock == 1) {
			curr_ptr = headPID_ptr; lowest_priority =
			curr_ptr; while (curr_ptr != NULL) {
				if (curr_ptr->priority <
				lowest_priority->priority)
					lowest_priority =  curr_ptr;
				curr_ptr = curr_ptr->next; }
			killed_array[*killed_size] =
			lowest_priority->PID; (*killed_size)++; head =
			delete(head, lowest_priority->PID);
			current_process = head; headPID_ptr =
			make_edge_node(head->PID, head->priority);
			deadlock = 0; } }

	return killed_array; }



/*==============================================================================
**===== make_process_node:  **=====
**==============================================================================*/

Edge *locate_deadlock(Process **head, Process **current_process, Edge
*edgePID_head) {

	Process *curr_ptr; Process *prv_ptr = NULL; Edge *currPID_ptr =
	edgePID_head; Edge *prevPID_ptr = NULL; int request; int i,j;

	for (i = 0; i < (*current_process)->requests_num; i++) {
		request = (*current_process)->requests[i]; curr_ptr =
		*head;

		if (curr_ptr->PID == (*current_process)->PID) {
			prv_ptr = curr_ptr; curr_ptr = curr_ptr->next;
		} while (curr_ptr != NULL) {
			for (j = 0; j< curr_ptr->holding_num; j++) {
				if (request == curr_ptr->holding[j]) {
					while (currPID_ptr != NULL ) {
						if (currPID_ptr->PID ==
						curr_ptr->PID) {
							deadlock=1;
							return
							edgePID_head; }
						prevPID_ptr =
						currPID_ptr;
						currPID_ptr =
						currPID_ptr->next; }
					currPID_ptr =
					make_edge_node(curr_ptr->PID,
					curr_ptr->priority);
					prevPID_ptr->next =
					currPID_ptr; return
					locate_deadlock(&*head,
					&curr_ptr, edgePID_head); } }
			prv_ptr = curr_ptr; curr_ptr = curr_ptr->next;
		} } if (i == (*current_process)->requests_num) {
		*head = delete(*head, (*current_process)->PID);
		edgePID_head = deletePID(edgePID_head,
		(*current_process)->PID); if (*head == NULL)
			return NULL; edgePID_head =
		make_edge_node((*head)->PID, (*head)->priority); if
		(prv_ptr == NULL || edgePID_head->next == NULL)
			*current_process = *head; else
			*current_process = prv_ptr; }

	return edgePID_head; }



/*==============================================================================
**===== make_process_node:  **=====
**==============================================================================*/


Edge *make_edge_node(int PID, int priority) {
	Edge *edge_new;

	edge_new= malloc(sizeof(Edge)); edge_new->PID = PID;
	edge_new->priority = priority; edge_new->next = NULL;

	return edge_new; }

/*==============================================================================
**===== make_process_node:  **=====
**==============================================================================*/


Process *delete(Process *head, int PID) {
	Process *curr = head, *prev = NULL;

	while (curr != NULL && curr->PID != PID) {
		prev = curr; curr = curr->next; }

	if (curr == NULL )
		return head;

	if (curr == head)
		head = head->next; else
		prev->next = curr->next; free(curr);

	return head; }


/*==============================================================================
**===== make_process_node:  **=====
**==============================================================================*/


Edge *deletePID(Edge *head, int PID) {
	Edge *curr = head, *prev = NULL;

	while (curr != NULL && curr->PID != PID) {
		prev = curr; curr = curr->next; }

	if (curr == NULL )
		return head;

	if (curr == head)
		head = head->next; else
		prev->next = curr->next; free(curr);

	return head; }

static  int intcompare(int *i, int *j) {
 if (*i > *j)
	return (1); if (*i < *j)
  return (-1); return (0); }

