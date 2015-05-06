/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : owners.h:
**  For     : 433 -351: Databases;
**            PROJECT 2
**  Written : Mark Angrish, Student number: 74592
**            May 2001;
**  Input   : No input 
**  Output  : No output 
** 
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "/usr/include/sys/socket.h"  /* for lcompat */
#include "dbname.h"

/*
 * These lengths are the length of the string defined in the
 * database schema + 1 for the null (string terminating) character.
 */
#define PARENT_NAME_LENGTH 12
#define PARENT_CITY_LENGTH 11
#define CHILD_NAME_LENGTH 22 
#define CHILD_CITY_LENGTH 11
#define CONAME_LENGTH 12
#define COCITY_LENGTH 11
#define HNAME_LENGTH 22
#define HCITY_LENGTH 10

/*=====================*/
/*typedefs for structs */
/*=====================*/

typedef struct owners Owners_record;/*a struct for the owners table in the DB*/
typedef struct hotels Hotels_record;/* a struct for the hotels table in the DB*/
typedef struct company Company; /* a struct for the output struct with company
																	 names and cities */
typedef struct output Output; /*a struct for the output */


/*=====================*/
/*     structs         */
/*=====================*/

struct owners {
		char parent_name[PARENT_NAME_LENGTH];
		char parent_city[PARENT_CITY_LENGTH];
		char child_name[CHILD_NAME_LENGTH];
		char child_city[CHILD_CITY_LENGTH];
		float fraction;
		Owners_record *next;
	};

struct hotels {
	char hotel_name[HNAME_LENGTH];
	char hotel_city[HCITY_LENGTH];
	Hotels_record *next;
	};

 /*company name and city and %ownership*/
struct company {
	char company_name[CONAME_LENGTH]; 
	char company_city[COCITY_LENGTH];
	double percentage;
	Company *next;
	};

/* made for the output with a linked list of companies a hotel is owned by*/
struct output {
	char hotel_name[HNAME_LENGTH];
	char hotel_city[HCITY_LENGTH];
	Company *company_head;
	Output *next;
	};
	
/* 
 * Some instructions to SQL about error messages and error conditions.
*/
EXEC SQL INCLUDE SQLCA;
EXEC SQL WHENEVER SQLERROR STOP;

/* GLOBAL FUNCTION PROTOTYPES */
Owners_record *getowners( void);
Hotels_record *gethotels( void);
Output *build(Owners_record *, Hotels_record *);
