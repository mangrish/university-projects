# include "/local/ingres/files/eqdef.h"
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
# include "/local/ingres/files/eqsqlca.h"
    extern IISQLCA sqlca;   /* SQL Communications Area */
/* # line 32 "account.sc" */	/* host code */
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
  struct {
    char account_number[ACCOUNT_NUMBER_LENGTH];
    char branch_name[BRANCH_NAME_LENGTH];
    int balance;
  } account_record;
  int minimum_balance = 0;
  short null_indicator[3];
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
/* # line 89 "account.sc" */	/* host code */
    /*
     * We must connect to the database that we will be accessing.
     * Remember to DISCONNECT at the end of the program.
     */
/* # line 93 "account.sc" */	/* connect */
  {
    IIsqInit(&sqlca);
    IIsqConnect(0,"banking",(char *)0, (char *)0, (char *)0, (char *)0, 
    (char *)0, (char *)0, (char *)0, (char *)0, (char *)0, (char *)0, 
    (char *)0, (char *)0, (char *)0);
    if (sqlca.sqlcode < 0) 
      IIsqStop(&sqlca);
  }
/* # line 96 "account.sc" */	/* host code */
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
/* # line 106 "account.sc" */	/* open */
  {
    IIsqInit(&sqlca);
    IIcsOpen("cursor1",13381,4331);
    IIwritio(0,(short *)0,1,32,0,"select account_number, branch_name, \
balance from account where balance>");
    IIputdomio((short *)0,1,30,4,&minimum_balance);
    IIcsQuery("cursor1",13381,4331);
    if (sqlca.sqlcode < 0) 
      IIsqStop(&sqlca);
  }
/* # line 107 "account.sc" */	/* host code */
    while (sqlca.sqlcode == 0)
    {
/* # line 109 "account.sc" */	/* fetch */
  {
    IIsqInit(&sqlca);
    if (IIcsRetrieve("cursor1",13381,4331) != 0) {
      IIcsGetio(&null_indicator[0],1,32,ACCOUNT_NUMBER_LENGTH-1,
      (account_record).account_number);
      IIcsGetio(&null_indicator[1],1,32,BRANCH_NAME_LENGTH-1,
      (account_record).branch_name);
      IIcsGetio(&null_indicator[2],1,30,4,&(account_record).balance);
      IIcsERetrieve();
    } /* IIcsRetrieve */
    if (sqlca.sqlcode < 0) 
      IIsqStop(&sqlca);
  }
/* # line 110 "account.sc" */	/* host code */
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
/* # line 138 "account.sc" */	/* close */
  {
    IIsqInit(&sqlca);
    IIcsClose("cursor1",13381,4331);
    if (sqlca.sqlcode < 0) 
      IIsqStop(&sqlca);
  }
/* # line 140 "account.sc" */	/* host code */
    /* Commit releases the read lock on the table */
/* # line 141 "account.sc" */	/* commit */
  {
    IIsqInit(&sqlca);
    IIxact(3);
    if (sqlca.sqlcode < 0) 
      IIsqStop(&sqlca);
  }
/* # line 143 "account.sc" */	/* host code */
    /*
     * If we wished we could now open the cursor again.  If we did so
     * after changing the value of the "minimum_balance" variable, we'd
     * get a different set of tuples back.
     */
    /*
     * We must disconnect from the database before finishing.
     */
/* # line 152 "account.sc" */	/* disconnect */
  {
    IIsqInit(&sqlca);
    IIsqDisconnect();
    if (sqlca.sqlcode < 0) 
      IIsqStop(&sqlca);
  }
/* # line 154 "account.sc" */	/* host code */
    exit(EXIT_SUCCESS);
}
