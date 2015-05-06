/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : build.e:
**  For     : 433 -351: Databases;
**            PROJECT 2
**  Written : Mark Angrish, Student number: 74592
**            May 2001;
**  Input   : the head of the owners list and the head of the hotels list.  
**  Output  : the head of the ouput linked list. 
**  
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include "owners.h"

/*---------------------------*/
/* LOCAL FUNCTION PROTOTYPES */
/*---------------------------*/

Output *make_output_node(char *, char *);
Output *add_owners(Owners_record *, Output *, char *, char *, float);
Output *insert_output(Output *, Output *);

/*---------------------------*/


Output *build(	Owners_record *owners_head, Hotels_record *hotel_head )
{
	Hotels_record *current_hotel= NULL;
	Owners_record *current_owner = NULL;
	Output *output_head = NULL;
	Output *output_curr = NULL;

	current_hotel = hotel_head;

	/*while there are hotels ...*/
	while (current_hotel != NULL) {

		/*make an output node for it..*/
		output_curr = make_output_node(current_hotel->hotel_name,current_hotel->hotel_city);

		/*then look through the owners structure one by one, and if the current hotel matches
			the current owner then add that owner to the ouput node for that hotel..*/
		current_owner = owners_head;
		while (current_owner != NULL) {
			if (strcmp(current_hotel->hotel_name, current_owner->child_name) == 0 &&
					strcmp(current_hotel->hotel_city, current_owner->child_city) == 0 ){

				output_curr = add_owners(owners_head, output_curr, current_owner->parent_name, 
																current_owner->parent_city, current_owner->fraction);
				break;
			}
			current_owner = current_owner->next;	
		}	
		
		/*then place the node into the output list*/
		output_head = insert_output(output_head, output_curr);
		current_hotel = current_hotel->next;
	}

	return output_head;/*return the head of the output list */
}


/*==============================================================================
** make_output_node: creates a new output node with only the hotel name and city
**									 filled in.
**	input- string of the hotel name, and its city 
**	output- an output node for the hotel name and its city.
**============================================================================*/


Output *make_output_node(char *hotel_name, char *hotel_city)
{
	Output *new_node;

	new_node = malloc(sizeof(Output));
	
	strcpy(new_node->hotel_name, hotel_name);
	strcpy(new_node->hotel_city, hotel_city);
	new_node->company_head = NULL;
	new_node->next = NULL;

	return new_node;
}

/*==============================================================================
** add_owners: adds all the owners of a hotel into the ouput data structure for
**						 a particular hotel
**	input- the head of the owners list, the ouput node itself, the company name
**				 and city that was matched to that hotel and the fraction of ownership
**	output- an output node with all its owners in it.
**============================================================================*/


Output *add_owners(Owners_record *head, Output *node, char *company_name, char *company_city, float fraction)
{
	Company *owner;
	Company *curr_ptr;
	Company *prv_ptr;
	Owners_record *curr_owner;

	curr_owner = head;

	owner = malloc(sizeof(Company));

	/*add the matched company name and city to the list of companys for ownership of
		that hotel */
	strcpy(owner->company_name, company_name);
	strcpy(owner->company_city, company_city);
	owner->percentage =(fraction * 100);
	owner->next = NULL;

	curr_ptr = node->company_head;

	/*..and place it at the end of the list for the compnays that own the hotel.*/
	if (node->company_head == NULL)
		node->company_head = owner;
	else {
		while (curr_ptr != NULL) {
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
		prv_ptr->next = owner;
	}

	/* for a company, if another company owns it in someway, then recurse to find the 
		 owner of that company and add it to the company list for that hotel.*/
	while (curr_owner != NULL) {
		if (strcmp(curr_owner->child_name, company_name) == 0 &&	
			  strcmp(curr_owner->child_city, company_city) == 0 ) {
			return add_owners(head, node, curr_owner->parent_name, curr_owner->parent_city, curr_owner->fraction * fraction);	
		}
		curr_owner = curr_owner->next;
	}

	return node;

}

/*==============================================================================
** insert_output: adds the node given to the list of output nodes.
**	input- the ptr to the head of the output list, and the node to be added.
**	output- the output list head, with the new node added.
**============================================================================*/

Output *insert_output(Output *head, Output *node)
{
	Output *curr_ptr = NULL;
	Output *prv_ptr = NULL;

	curr_ptr = head;

	if (head == NULL)
		head = node;

	else {
		while (curr_ptr !=NULL){
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}
		prv_ptr->next = node;
	}

	return head;
}
