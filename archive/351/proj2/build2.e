#include "owners.h"

Output *make_output_node(char *, char *);
Output *add_owners(Output *, char *, char *, float);
Output *insert_output(Output *, Output *);

Output *build(	Owners_record *owners_head, Hotels_record *hotel_head )
{
	Hotels_record *current_hotel= NULL;
	Owners_record *current_owner = NULL;
	Output *output_head = NULL;
	Output *output_curr = NULL;

	current_hotel = hotel_head;

	while (current_hotel != NULL) {

		output_curr = make_output_node(current_hotel->hotel_name,current_hotel->hotel_city);

		current_owner = owners_head;
		while (current_owner != NULL) {
			if (strcmp(current_hotel->hotel_name, current_owner->child_name) == 0 &&
					strcmp(current_hotel->hotel_city, current_owner->child_city) == 0 ){

				output_curr = add_owners(output_curr, current_owner->parent_name, 
																current_owner->parent_city, current_owner->fraction);
				break;
			}
			current_owner = current_owner->next;	
		}	
		
		output_head = insert_output(output_head, output_curr);
		current_hotel = current_hotel->next;
	}

	return output_head;
}

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

Output *add_owners(Output *node, char *parent_name, char *parent_city, float fraction)
{
	Company *owner;

	owner = malloc(sizeof(Company));

	strcpy(owner->company_name, parent_name);
	strcpy(owner->company_city, parent_city);
	owner->percentage =(fraction * 100);
	owner->next = NULL;

	node->company_head = owner;


	return node;

}

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
