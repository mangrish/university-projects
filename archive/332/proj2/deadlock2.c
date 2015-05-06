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

#define MAX_RESOURCE_SIZE 20

typedef struct process Process;
typedef struct transaction_edge Edge;

struct process {
	int		PID;
	int		priority;
	int		holding_num;
	int 	holding[MAX_RESOURCE_SIZE];
	int		requests_num;
	int		requests[MAX_RESOURCE_SIZE];
	Process *next;
};

struct transaction_edge {
	int PID;
	int priority;
	Edge *next;
};

int deadlock = 0;

/*=============================================================================
**===== FUNCTION PROTOTYPES. 
**=============================================================================*/
void usage( char *);
void print_results( void );
FILE *open_file(char *, char *, const char *);
Process *make_process_node(FILE*);
Process *make_system_state(FILE*);
void wait_for_graph(Process *, Process *);
Edge *locate_deadlock(Process **, Process **, Edge *);
Edge *make_edge_node(int  , int);
Process *delete(Process *, int);
Edge *deletePID(Edge *, int);
/*===========================================================================*/


int main( int argc, char *argv[])
{
	char *prog_name = argv[0];
	FILE *ssf_fptr= NULL;
	Process *head;

	if (argc != 2) 
		usage(prog_name);
	
	ssf_fptr = open_file(prog_name, argv[1], "r");
	head = make_system_state(ssf_fptr);
	wait_for_graph(head, head);
	print_results();

	fclose(ssf_fptr);

	return 0;
}


/*=============================================================================
**===== usage: displays usage message to user, if program incorrectly invoked.
**=============================================================================*/

void usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s [filename]\n", prog_name);
	exit( EXIT_FAILURE );
}


/*==============================================================================
**===== open_file: opens a file in read mode. If the file cannot be opened, it
**=====						 reports this to the user
**==============================================================================*/

FILE *open_file( char *prog_name, char *file_name, const char *mode)
{
	FILE *file_ptr = NULL;

	file_ptr = fopen(file_name, mode);

	if (file_ptr == NULL) {
		fprintf(stderr, "%s: ERROR: File \"%s\" not opened; "
					  "program execution terminated.\n", prog_name, file_name);
		exit(EXIT_FAILURE);
	}

	return file_ptr;
}

/*==============================================================================
**===== print_results: 
**==============================================================================*/

void print_results( void)
{
	if (deadlock ==1)
		printf("Deadlock was detected");
	else
		printf("Deadlock was not detected");
	
	return;
}


/*==============================================================================
**===== make_system_state: 
**=====	
**==============================================================================*/

Process *make_system_state(FILE *ssf_fptr) 
{
	Process *process = NULL;
	Process *head = NULL;
	Process *tail = NULL;

	while ((process = make_process_node(ssf_fptr)) != NULL) {
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
	int i =0, j=0;
	int c;
	Process *new;

	new = malloc(sizeof (Process));

	if ((c = fscanf(file_ptr,"%*s%i%*s%i%*s",&new->PID, &new->priority)) == 0 || c == EOF)
		return NULL;

	while (fscanf(file_ptr,"%i",&new->holding[i]) != 0)
		i++;
	new->holding_num = i;	

	if (i == 0)
		fscanf(file_ptr,"%*s");
	fscanf(file_ptr,"%*s");

	while (fscanf(file_ptr,"%i",&new->requests[j]) != 0) 
		j++;
	new->requests_num = j;	
	new->next = NULL;

	if (j == 0)
		fscanf(file_ptr,"%*s");

	return new;

}

/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

void wait_for_graph(Process *head, Process *current_process)
{
	Edge *headPID_ptr = make_edge_node(head->PID, head->priority);
	Edge *lowest_priority;
	Edge *curr_ptr;

	while ((headPID_ptr = locate_deadlock(&head, &current_process, headPID_ptr)) != NULL){
		if (deadlock == 1) {
			curr_ptr = headPID_ptr;
			lowest_priority = curr_ptr;
			while (curr_ptr != NULL) {
				if (curr_ptr->priority < lowest_priority->priority) 
					lowest_priority =  curr_ptr;
				curr_ptr = curr_ptr->next;
			}
			printf("%i", lowest_priority->PID);
			head = delete(head, lowest_priority->PID);
			current_process = head;
			headPID_ptr = make_edge_node(head->PID, head->priority);
			deadlock = 0;
		}
	}

	return;
}



/*==============================================================================
**===== make_process_node: 
**=====	
**==============================================================================*/

Edge *locate_deadlock(Process **head, Process **current_process, Edge *edgePID_head)
{

	Process *curr_ptr;
	Process *prv_ptr = NULL;
	Edge *currPID_ptr = edgePID_head;
	Edge *prevPID_ptr = NULL;
	int request;
	int i,j;

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
		if (prv_ptr == NULL)
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


Edge *make_edge_node(int PID, int priority)
{
	Edge *edge_new;
	
	edge_new= malloc(sizeof(Edge));
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


Edge *deletePID(Edge *head, int PID)
{
	Edge *curr = head, *prev = NULL;

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
