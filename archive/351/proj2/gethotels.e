/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : gethotels.e:
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

Hotels_record *add_hrecord(Hotels_record *, char *, char *);

/*---------------------------*/


Hotels_record *gethotels(void)
{
	Hotels_record *head  = NULL;/* our C defined data structure ptr to its head*/

	/* 
  * All variables (and their corresponding structures) that will be
  * used by SQL. 
  */

	EXEC SQL BEGIN DECLARE SECTION;

	struct {
		char hname[HNAME_LENGTH];
		char hcity[COCITY_LENGTH];
	} Hotels;

	short null_indicator[2];

	EXEC SQL END DECLARE SECTION;

	/* execute the SQL statement by declaring a cursor so we can step through the tuples
		 one at a time */
	EXEC SQL DECLARE cursor2 CURSOR FOR
	SELECT hname, hcity
	FROM hotel;

	EXEC SQL CONNECT DBNAME;/*connect to the database */

	/*as long as there is no error code (ie more tuples available for reading ).. */
	EXEC SQL OPEN cursor2 for readonly;
	while (sqlca.sqlcode == 0) {
		EXEC SQL FETCH cursor2 INTO :Hotels INDICATOR :null_indicator;
		if (sqlca.sqlcode == 0) {
			/*copy the record to the hotels structure defined by us (ie c data structure)*/
			head = add_hrecord(head, Hotels.hname, Hotels.hcity);
		}
	}
	EXEC SQL CLOSE cursor2;/*close the cursor */
	EXEC SQL commit;/* Commit releases the read lock on the table */
	EXEC SQL DISCONNECT;/* disconnect from the database before finishing.*/

	return head;/*return the head of the C data structure for hotels table */
}


/*==============================================================================
** add_hrecord:adds a new Hotels_reecord node to the end of the C linked list 
**  input- ptr to head of the hotels table, the hotel name,and  hotel city .
**  output- the head of the owners linked list, with the new node added to the end.
**============================================================================*/

Hotels_record *add_hrecord(Hotels_record *head, char *hotel_name, char *hotel_city)
{
	Hotels_record *new_record;
	Hotels_record *curr_ptr = head;
	Hotels_record *prv_ptr = NULL;

	new_record = malloc(sizeof(Hotels_record ));
	
	strcpy(new_record->hotel_name, hotel_name); 
	strcpy(new_record->hotel_city, hotel_city);
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
