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

typedef struct process	Process; /*struct of processes for process list*/
typedef struct resource	Resource;/*struct of resources for resouce list*/

struct process {
	int			PID;					/*the process ID number */
	int			priority;			/*its priority */
	int			holding_num;	/*the number or resources its holding */
	int 		*holding;			/*dynamic array of int of the resources its holding*/
	int			requests_num;	/*the number of resources its requesting*/
	int			*requests;		/*dynamic array of int of the resources its requesting*/
	Process *next;				/*next process in list */
};

struct resource {
	int				PID;			/*the process ID of the wait for resource process*/
	int				priority;	/* its priority */
	Resource	*next;		/*next in the wait for resource graph */
};

/*GLOBAL VARIABLES*/
int deadlock = 0; /* trigger is deadlock detected.*/

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
	killed_array = wait_for_graph(process_head, process_head, num_processes, &killed_size );/*create the wait for resource graph and return what PIDs were killed*/
	print_results( killed_array, killed_size ); /*output the results to the screen*/

	fclose( ssf_fptr );  /*close the file*/
	return 0;
}


/*=============================================================================
**===== usage: displays usage message to user, if program incorrectly invoked.
**=====	INPUT: the program executables name
**===== OUTPUT: the usage statement to stderr.
**=============================================================================*/

void usage( char *prog_name )
{
	fprintf( stderr, "Usage: %s [filename]\n", prog_name );
	exit( EXIT_FAILURE );
}


/*==============================================================================
**===== open_file: opens a file in read mode. 
**===== INPUT: program name, name of the file, and the mode to open file in.
**===== OUTPUT: returns the file pointer to the file given in the input. 
**==============================================================================*/

FILE *open_file( char *prog_name, char *file_name, const char *mode )
{
	FILE *file_ptr = NULL;

	file_ptr = fopen(file_name, mode); /*open the file...*/

	/* if the file entered is wrong or doesnt exit failure....*/
	if (file_ptr == NULL) {
		fprintf( stderr, "%s: ERROR: File \"%s\" not opened; "
					   "program execution terminated.\n", prog_name, file_name );
		exit( EXIT_FAILURE );
	}

	/*...else return the file ptr*/
	return file_ptr;
}


/*==============================================================================
**===== print_results: prints the results of the killed process in ascending order, and if no deadlock exits, a report saying that no deadlock existed.
**===== INPUT: the array or PIDs that have been killed, and the size of the array
**===== OUTPUT: the output as specified at the top of this file.
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
**===== create_system_state: creates a list of all the system states in the input file.
**=====	INPUT: the file pointer, and a pointer to the number of process in the file.
**===== OUTPUT: A linked list of all the processes in the system state.
**==============================================================================*/

Process *create_system_state_list(FILE *ssf_fptr, int *num_processes) 
{
	Process *process = NULL;
	Process *head = NULL;
	Process *tail = NULL;

	/*while there are processes....*/
	while ((process = make_process_node(ssf_fptr)) != NULL) {
		(*num_processes)++;	/*count them ...*/
		if (head == NULL) 	/* and add them to the end of the linked list */
			head = tail= process;
		else {
			tail->next = process;
			tail = process;
		}
	}	
	return head; /* return the head of the linked list */
}


/*==============================================================================
**===== make_process_node: creates a new node to insert in the list of processes.
**===== INPUT: the file pointer	
**===== OUTPUT: the corresponding linked list node for the line in the file ptr.
**==============================================================================*/

Process *make_process_node(FILE *file_ptr) 
{
	int 		i = 0;
	int 		j = 0;
	int 		c;
	Process	*new;
	int			*requests;
	int			*holding;

	new = malloc(sizeof (Process)); /*create the space for the new node */

	/* scan for the process ID, and priority number, and skip any chars..*/
	if ((c = fscanf( file_ptr, "%*s%i%*s%i%*s", &new->PID, &new->priority)) == 0 || c == EOF)
		return NULL; /*if EOF is reached, its the end of the file so return NULL*/

	holding = malloc( (i+1) * sizeof(int) );/*make the space for the held resources*/
	while (fscanf( file_ptr, "%i", &holding[i]) != 0) {
		i++;
		/*while there are still numbers, make the array of held resources bigger!*/
		holding = realloc( holding, ( (i+1) * sizeof(int)  )); 
	}
	new->holding_num = i;	
	new->holding = holding;

	/*skip over any chars (ie "*" or "r")*/
	if (i == 0)
		fscanf(file_ptr,"%*s");
	fscanf(file_ptr,"%*s");

	requests = malloc( (j+1) * sizeof(int) );/*make the space for the requested resources*/
	while ( fscanf( file_ptr, "%i", &requests[j]) != 0) { 
		j++;
		/*while there are still numbers, make the array or requested resources bigger!*/
		requests = realloc( requests, ( (j+1) * sizeof(int) ));
	}
	new->requests_num = j;	
	new->requests = requests;
	new->next = NULL;

	if (j == 0) /*skip over a "*" if it was there */
		fscanf(file_ptr,"%*s");

	return new; /* return the new node */

}

/*==============================================================================
**===== wait_for_graph: creates the wait for resource graph, and kills any 
**=====                 deadlock if its detected.
**=====  INPUT: the process list head, the current process being looked at, and
**=====         the number of processes in the list, and a pointer to the number
**=====         of killed resources.
**===== OUTPUT: the array of killed resources PIDs	
**==============================================================================*/

int *wait_for_graph(Process *head, Process *current_process, int num_processes, int *killed_size)
{
	Resource	*headPID_ptr;
	Resource	*lowest_priority; /* lowest priority of a deadlocked chain */
	Resource	*curr_ptr;
	int				*killed_array; /* the killed processes ID array*/

	/*create the first PID in the wait for resource list, and create the killed
		PID array */
	headPID_ptr = make_edge_node(head->PID, head->priority);
	killed_array = malloc(num_processes * sizeof(int));

	/*while a process chain exists..*/
	while ((headPID_ptr = locate_deadlock(&head, &current_process, headPID_ptr)) != NULL){
		if (deadlock == 1) { /*if that chain is deadlocked...*/
			curr_ptr = headPID_ptr;
			lowest_priority = curr_ptr;
			while (curr_ptr != NULL) {
				if (curr_ptr->priority < lowest_priority->priority) 
					lowest_priority =  curr_ptr; /*.. find the lowest priority in it.. */
				curr_ptr = curr_ptr->next;
			}
			killed_array[*killed_size] = lowest_priority->PID; /*.. add the PID to the killed array.. */
			(*killed_size)++;
			head = delete(head, lowest_priority->PID); /*kill that PID*/
			current_process = head; /*.. and continue from the head of the chain */
			headPID_ptr = make_edge_node(head->PID, head->priority);
			deadlock = 0; 
		}
	}

	return killed_array; /*return the killed array of PIDs*/
}


/*==============================================================================
**===== locate_deadlock: recusivly finds if deadlock exits.  If deadlock is 
**=====                  detected, the resource graph is returned, otherwise 
**=====	                 it recurses to the end of the chain and removes processes that can be accomodated.
**=====	INPUT: a pointer to the head of the process list, a pointer to 
**=====        the current process being checked against, and the head of the Resource graph.
**===== OUTPUT: 1) the head of the resource graph, if deadlock is detected.
**=====         2) recurses until the end of a chain is found.
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

	/* for all the processes requests..... */
	for (i = 0; i < (*current_process)->requests_num; i++) {
		request = (*current_process)->requests[i];
		curr_ptr = *head; 

		/*skip the process, if its looking at its own contents....*/
		if (curr_ptr->PID == (*current_process)->PID) {
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
		/*for all the processes in the system state...*/
		while (curr_ptr != NULL) {
			/*for all the current nodes held resources ....*/
			for (j = 0; j< curr_ptr->holding_num; j++) {
				if (request == curr_ptr->holding[j]) {/*..if the request is in the held resources.. */
					while (currPID_ptr != NULL ) { /* then for all the PIDs in the resouce chain*/
						if (currPID_ptr->PID == curr_ptr->PID) {/*if the PID has been seen before*/
							deadlock=1; /*deadlock has been reached..*/
							return edgePID_head;
						}
						prevPID_ptr = currPID_ptr;
						currPID_ptr = currPID_ptr->next;
					}
					/*otherwise, add the PID to the resource chain...*/
					currPID_ptr = make_edge_node(curr_ptr->PID, curr_ptr->priority);
					prevPID_ptr->next = currPID_ptr;
					/*and recurse for the current PIDS requests */
					return locate_deadlock(&*head, &curr_ptr, edgePID_head);
				}
			}
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
	}
	/*if a PIDs requests have been fulfilled, finish the process*/
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

	return edgePID_head; /* return the head of the process list*/
}


/*==============================================================================
**===== make_process_node:  makes a new node to insert into the resource list.
**===== INPUT: the PID to assign the new node with, and the priority of it.	
**===== OUTPUT: A new node initialised for the first two values.
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
**===== delete: deletes the PID given, from the Process list.
**===== INPUT: The process list head, and the PID of the item to be deleted.
**===== OUTPUT: The process list minus the PID numbers node.
**==============================================================================
*/

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
**===== deletePID: deletes the PID given, from the Resource list.
**=====	INPUT: The resource list head, and the PID of the item to be deleted.
**===== OUTPUT: The resource list minus the PID numbers node.
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
	free(curr); /*free the node */

	return head; /*return the head of the process list minus the deleted node*/
}


/*==============================================================================
**===== cmp: Compares two numbers and indicates through a number which number is bigger.
**===== INPUT: two pointers to ints	
**===== OUTPUT: a number to indicate which number was bigger for the qsort function.
**==============================================================================*/

int cmp(int *x,int *y)
{
   if(*x < *y) return (-1);
   if(*x > *y) return ( 1);
   return (0);
}
