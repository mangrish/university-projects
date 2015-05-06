/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : getowners.e:
**  For     : 433 -351: Databases;
**            PROJECT 2
**  Written : Mark Angrish, Student number: 74592
**            May 2001;
**  Purpose: this module opens the database and copies the tuple from the database
**					 to a C defined data structure.
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include "owners.h"

/*---------------------------*/
/* LOCAL FUNCTION PROTOTYPES */
/*---------------------------*/

Owners_record *add_orecord(Owners_record *, char *, char *, char *, char *, float);

/*---------------------------*/


Owners_record *getowners(void)
{
	Owners_record *head = NULL;/* our C defined data structure ptr o its head*/

	/* 
  * All variables (and their corresponding structures) that will be
  * used by SQL. 
  */

	EXEC SQL BEGIN DECLARE SECTION;

	struct {
		char parentname[PARENT_NAME_LENGTH];
		char parentcity[PARENT_CITY_LENGTH];
		char childname[CHILD_NAME_LENGTH];
		char childcity[CHILD_CITY_LENGTH];
		float fraction;
	} Owners;

	short null_indicator[5];

	EXEC SQL END DECLARE SECTION;

	/* execute the SQL statement by declaring a cursor so we can step through the tuples
		 one at a time */
	EXEC SQL DECLARE cursor1 CURSOR FOR
	SELECT *
	FROM owners;

	EXEC SQL CONNECT DBNAME;/*connect to the database */

	/*as long as there is no error code (ie more tuples available for reading ).. */
	EXEC SQL OPEN cursor1 for readonly;
	while (sqlca.sqlcode == 0) {
		EXEC SQL FETCH cursor1 INTO :Owners INDICATOR :null_indicator;
		if (sqlca.sqlcode == 0) {
			/*copy the record to the owners structure defined by us (ie c data structure)*/	
			head = add_orecord(head, Owners.parentname, Owners.parentcity, Owners.childname,
												Owners.childcity, Owners.fraction);
		}
	}
	EXEC SQL CLOSE cursor1;/*close the cursor */
	EXEC SQL commit;/* Commit releases the read lock on the table */
	EXEC SQL DISCONNECT;/* disconnect from the database before finishing.*/

	return head;/*return the head of the C data structure for owners table */
}


/*==============================================================================
** add_orecord:adds a new Owners_reecord node to the end of the C linked list 
**  input- ptr to head of the owners table, the parent name, parent city, child 
**				 name and chid city, and the fration of ownership.
**  output- the head of the owners linked list, with the new node added to the end.
**============================================================================*/

Owners_record *add_orecord(Owners_record *head, char *parent_name, char *parent_city,
													char *child_name, char *child_city, float fraction)
{

	Owners_record *new_record;
	Owners_record *curr_ptr = head;
	Owners_record *prv_ptr = NULL;

	new_record = malloc(sizeof(Owners_record ));
	
	strcpy(new_record->parent_name, parent_name);
	strcpy(new_record->parent_city, parent_city);
	strcpy(new_record->child_name, child_name);
	strcpy(new_record->child_city, child_city);
	new_record->fraction = fraction;
	new_record->next = NULL;

	/*if the head of the linked list is NULL, then add the node there, otherwise put it
		at the end of the linked list*/
	if (head == NULL)
		head = new_record;

	else {
		while (curr_ptr != NULL) {
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
		prv_ptr->next = new_record;
	}

	return head;
}
