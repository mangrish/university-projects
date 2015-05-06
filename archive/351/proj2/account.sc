/*
 * A simple Embedded SQL example using the banking_example database
 * described in Database System Concepts by Silberschatz, Korth and
 * Sudarshan.
 *
 * Written by:  Evan Harris <evan@cs.mu.oz.au>
 * Modified by Linda Stern
 * Last update: 2 May 2001
 */

#include <stdlib.h>
#include <stdio.h>

#include "/usr/include/sys/socket.h"  /* for lcompat */


/*
 * These lengths must be the length of the string defined in the
 * database schema + 1 for the null (string terminating) character.
 */
#define ACCOUNT_NUMBER_LENGTH 16
#define BRANCH_NAME_LENGTH 31

/* 
 * Some instructions to SQL about error messages and error conditions.
 * Be sure to include this in your own programs.
 */
EXEC SQL INCLUDE SQLCA;
EXEC SQL WHENEVER SQLERROR STOP;


int
main(int argc, char** argv)
{
    /* 
     * All variables (and their corresponding structures) that will be
     * used by SQL. 
     *
     * If a variable is to be used in an SQL statement (preceded by a colon), 
       it must be declared here.
     */
    EXEC SQL BEGIN DECLARE SECTION;

    struct
    {
	char account_number[ACCOUNT_NUMBER_LENGTH];
	char branch_name[BRANCH_NAME_LENGTH];
	int balance;
    }
    account_record;

    int minimum_balance = 0;

    /*
     * This indicates whether any of the returned structure values are NULL.
     * There must be one element for each field.
     */
    short null_indicator[3];

    EXEC SQL END DECLARE SECTION;

    /*
     * Parse any program arguments.
     */
    if (argc == 2)
    {
	minimum_balance = atoi(argv[1]);
    }
    else if (argc > 2)
    {
	fprintf(stderr, "Usage: %s [minimum_balance]", argv[0]);
	exit(EXIT_FAILURE);
    }
    
    /*
     * Declaring a cursor allows us to step through the result tuples
     * of a query one at a time.  The query may also have GROUP BY,
     * HAVING, etc., clauses.
     *
     * Note that the cursor variable can not be named "cursor" because
     * the embedded SQL compiler confuses it with the CURSOR keyword.
     */
    EXEC SQL DECLARE cursor1 CURSOR FOR
	SELECT account_number, branch_name, balance
	FROM account
	WHERE balance > :minimum_balance;


    /*
     * We must connect to the database that we will be accessing.
     * Remember to DISCONNECT at the end of the program.
     */
    EXEC SQL CONNECT banking;


    /* 
     * This is a trivial example of how to use the cursor facility.
     *
     * sqlca.sqlcode, the error code, is 0 providing an error
     * (including EOF) has not occurred.  That is, more tuples are
     * available for reading from the cursor.
     *
     * We assume that the meaning of sqlca.sqlcode != 0 is similar to
     * "EOF" of C.
     */
    EXEC SQL OPEN cursor1;
    while (sqlca.sqlcode == 0)
    {
	EXEC SQL FETCH cursor1 INTO :account_record INDICATOR :null_indicator;
	if (sqlca.sqlcode == 0)
	{
	    /*
	     * The account number can't be NULL because it is declared
	     * as NOT NULL in the schema.
	     */
	    printf("%s ", account_record.account_number);

	    /*
	     * The branch name can be NULL, so we must deal with that
	     * eventuality.
	     */
	    if (null_indicator[1])
	    {
		printf("NULL ");
	    }
	    else
	    {
		printf("%s ", account_record.branch_name);
	    }

	    /*
	     * The balance can't be NULL because we've compared it above.
	     */
	    printf("%d\n", account_record.balance);
	}
    }
    /*  You must close one cursor before opening another */
    EXEC SQL CLOSE cursor1;  
    
    /* Commit releases the read lock on the table */
    EXEC SQL commit;

    /*
     * If we wished we could now open the cursor again.  If we did so
     * after changing the value of the "minimum_balance" variable, we'd
     * get a different set of tuples back.
     */

    /*
     * We must disconnect from the database before finishing.
     */
    EXEC SQL DISCONNECT;
    
    exit(EXIT_SUCCESS);
}

