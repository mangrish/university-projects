/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : deadlock.c: 
**  For     : 433 - 332: Operating Systems;
**            PROJECT 2
**  Written : Mark Angrish, Student number: 74592
**            May 2001;
**  Input   : FILE containing the system state as a command line argument;
**  Output  : To stdout: 1) report on whether or not the system is initally 
**													deadlocked; and,.
**											 2) if deadlock exists, describe which process(es) 
**													should be aborted to resolve the deadlock, according
**													to the specifications.
**											 e.g. > deadlock systemsatefile
**										
**														Deadlock was detected
**														Abort processes 2,5 to resolve deadlock
**
**														>
**	Questions: Refer to the file "comments".
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct process	Process;
typedef struct resource	Resource;

struct process {
	int			PID;
	int			priority;
	int			holding_num;
	int 		*holding;
	int			requests_num;
	int			*requests;
	Process *next;
};

struct resource {
	int				PID;
	int				priority;
	Resource	*next;
};

int deadlock = 0;

/*=============================================================================
**===== FUNCTION PROTOTYPES. 
**=============================================================================*/

void				usage( char * );
void				print_results( int *, int );
FILE				*open_file( char *, char *, const char * );
Process			*make_process_node( FILE* );
Process			*create_system_state_list( FILE*, int * );
int					*wait_for_graph( Process *, Process *, int, int * );
Resource		*locate_deadlock( Process **, Process **, Resource * );
Resource		*make_edge_node( int  , int );
Process			*delete( Process *, int );
Resource		*deletePID( Resource *, int );
int 				cmp(int *,int *);
/*int *resolve_deadlock(Resource **, Process **, Process **, int *, int *);*/
int *resolve_deadlock(Resource *headPID_ptr, Process *head, Process *current_process, int *killed_array, int *killed_size);

/*===========================================================================*/

int main( int argc, char *argv[] )
{
	char		*prog_name = argv[0]; /*the name of the executable*/
	FILE		*ssf_fptr= NULL;			/*the system state file pointer*/
	Process *process_head;				/*pointer to head of the process list*/
	int			*killed_array = NULL; /*the process IDs of all killed process*/
	int			num_processes = 0;		/*the total number of processes */
	int			killed_size = 0;			/*the total number of killed processes */

	if (argc != 2) 
		usage(prog_name);
	
	ssf_fptr = open_file( prog_name, argv[1], "r" ); /*open the system state file*/
	process_head = create_system_state_list( ssf_fptr, &num_processes );/*create the system state*/
	killed_array = wait_for_graph(process_head, process_head, num_processes, &killed_size );
	print_results( killed_array, killed_size ); /*output the results to the screen*/

	fclose( ssf_fptr ); 
	return 0;
}


/*=============================================================================
**===== usage: displays usage message to user, if program incorrectly invoked.
**=============================================================================*/

void usage( char *prog_name )
{
	fprintf( stderr, "Usage: %s [filename]\n", prog_name );
	exit( EXIT_FAILURE );
}


/*==============================================================================
**===== open_file: opens a file in read mode. If the file cannot be opened, it
**=====						 reports this to the user
**==============================================================================*/

FILE *open_file( char *prog_name, char *file_name, const char *mode )
{
	FILE *file_ptr = NULL;

	file_ptr = fopen(file_name, mode);

	if (file_ptr == NULL) {
		fprintf( stderr, "%s: ERROR: File \"%s\" not opened; "
					   "program execution terminated.\n", prog_name, file_name );
		exit( EXIT_FAILURE );
	}

	return file_ptr;
}


/*==============================================================================
**===== print_results: 
**==============================================================================*/

void print_results( int *killed_array, int killed_size )
{
	int i;
	
	/*sort the output array of killed processes into ascending order*/
	qsort( killed_array, killed_size, sizeof(int), (int (*)(const void *,const void *))cmp );

	if (killed_size != 0) { /*While there is a killed process, print them out.. */
		printf( "\nDeadlock was detected.\n" );
		printf( "Abort processes : " );
		for ( i = 0; i< killed_size; i++ ) {
			if ( (i+1) == killed_size )
				printf( "%i ", killed_array[i] );
			else
				printf( "%i, ", killed_array[i] );
		}
		printf( "to resolve deadlock\n\n" );
	}
	else										/*..else print there was no deadlock*/
		printf( "\nDeadlock was not detected.\n\n" );
	
	return;
}


/*==============================================================================
**===== make_system_state: 
**=====	
**==============================================================================*/

Process *create_system_state_list(FILE *ssf_fptr, int *num_processes) 
{
	Process *process = NULL;
	Process *head = NULL;
	Process *tail = NULL;

	while ((process = make_process_node(ssf_fptr)) != NULL) {
		(*num_processes)++;
		if (head == NULL) 
			head = tail= process;
		else {
			tail->next = process;
			tail = process;
		}
	}	
	return head;
}


/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

Process *make_process_node(FILE *file_ptr) 
{
	int 		i = 0;
	int 		j = 0;
	int 		c;
	Process	*new;
	int			*requests;
	int			*holding;

	new = malloc(sizeof (Process));

	if ((c = fscanf( file_ptr, "%*s%i%*s%i%*s", &new->PID, &new->priority)) == 0 || c == EOF)
		return NULL;

	holding = malloc( (i+1) * sizeof(int) );
	while (fscanf( file_ptr, "%i", &holding[i]) != 0) {
		i++;
		holding = realloc( holding, ( (i+1) * sizeof(int)  )); 
	}
	new->holding_num = i;	
	new->holding = holding;

	if (i == 0)
		fscanf(file_ptr,"%*s");
	fscanf(file_ptr,"%*s");

	requests = malloc( (j+1) * sizeof(int) );
	while ( fscanf( file_ptr, "%i", &requests[j]) != 0) { 
		j++;
		requests = realloc( requests, ( (j+1) * sizeof(int) ));
	}
	new->requests_num = j;	
	new->requests = requests;
	new->next = NULL;

	if (j == 0)
		fscanf(file_ptr,"%*s");

	return new;

}

/*==============================================================================
**===== wait_for_graph: 
**=====	
**==============================================================================*/

int *wait_for_graph(Process *head, Process *current_process, int num_processes, int *killed_size)
{
	Resource	*headPID_ptr;
	int				*killed_array;

	headPID_ptr = make_edge_node(head->PID, head->priority);
	killed_array = malloc(num_processes * sizeof(int));

	while ((headPID_ptr = locate_deadlock(&head, &current_process, headPID_ptr)) != NULL){
		if (deadlock == 1) 
			killed_array = resolve_deadlock(&(*headPID_ptr), &(*head), &(*current_process), killed_array, &(*killed_size));
	}

	return killed_array;
}


int *resolve_deadlock(Resource *headPID_ptr, Process *head, Process *current_process, int *killed_array, int *killed_size)
{
	Resource	*lowest_priority;
	Resource	*curr_ptr;

	curr_ptr = headPID_ptr;
	lowest_priority = curr_ptr;
	while (curr_ptr != NULL) {
		if (curr_ptr->priority < lowest_priority->priority) 
			lowest_priority =  curr_ptr;
		curr_ptr = curr_ptr->next;
	}
	killed_array[*killed_size] = lowest_priority->PID;
	(*killed_size)++;
	head = delete(head, lowest_priority->PID);
	*current_process = *head;
	headPID_ptr = make_edge_node((head)->PID, (head)->priority);
	deadlock = 0;

	return killed_array;
}
/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

Resource *locate_deadlock(Process **head, Process **current_process, Resource *edgePID_head)
{

	Process		*curr_ptr;
	Process		*prv_ptr = NULL;
	Resource	*currPID_ptr = edgePID_head;
	Resource	*prevPID_ptr = NULL;
	int				request;
	int				i;
	int				j;

	for (i = 0; i < (*current_process)->requests_num; i++) {
		request = (*current_process)->requests[i];
		curr_ptr = *head;

		if (curr_ptr->PID == (*current_process)->PID) {
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
		while (curr_ptr != NULL) {
			for (j = 0; j< curr_ptr->holding_num; j++) {
				if (request == curr_ptr->holding[j]) {
					while (currPID_ptr != NULL ) {
						if (currPID_ptr->PID == curr_ptr->PID) {
							deadlock=1;
							return edgePID_head;
						}
						prevPID_ptr = currPID_ptr;
						currPID_ptr = currPID_ptr->next;
					}
					currPID_ptr = make_edge_node(curr_ptr->PID, curr_ptr->priority);
					prevPID_ptr->next = currPID_ptr;
					return locate_deadlock(&*head, &curr_ptr, edgePID_head);
				}
			}
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
	}
	if (i == (*current_process)->requests_num) {
		*head = delete(*head, (*current_process)->PID);
		edgePID_head = deletePID(edgePID_head, (*current_process)->PID);
		if (*head == NULL)
			return NULL;
		edgePID_head = make_edge_node((*head)->PID, (*head)->priority);
		if (prv_ptr == NULL || edgePID_head->next == NULL)
			*current_process = *head;
		else
			*current_process = prv_ptr;
		}

	return edgePID_head;
}


/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

Resource *make_edge_node(int PID, int priority)
{
	Resource *edge_new;
	
	edge_new= malloc(sizeof(Resource));
	edge_new->PID = PID;
	edge_new->priority = priority;
	edge_new->next = NULL;

	return edge_new;
}

/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

Process *delete(Process *head, int PID)
{
	Process *curr = head, *prev = NULL;

	while (curr != NULL && curr->PID != PID) {
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL )
		return head;

	if (curr == head) 
		head = head->next;
	else
		prev->next = curr->next;
	free(curr);

	return head;
}


/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

Resource *deletePID(Resource *head, int PID)
{
	Resource *curr = head, *prev = NULL;

	while (curr != NULL && curr->PID != PID) {
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL )
		return head;

	if (curr == head) 
		head = head->next;
	else
		prev->next = curr->next;
	free(curr);

	return head;
}


/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

int cmp(int *x,int *y)
{
   if(*x < *y) return (-1);
   if(*x > *y) return ( 1);
   return (0);
}
