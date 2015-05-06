/*=========================*/
/*-------------------------*/
/*---- Question 4 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q4.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/

/* question 4: this query lists all the agents who work for more than one
	 						 agency, and their registration numbers in alphabetical order
							 with a secondary ordering on registration number */

select distinct agname , AGENT.regonumber /* select the agent name and the registration number*/
																				  /* from agent */
from AGENT, WORKS 	/* from agent and works */
where AGENT.regonumber = WORKS.regonumber /* where the agent regonumber equals the works regonumber */
group by agname, AGENT.regonumber  /* group that result by agent name and agent regonumber */
having  count(WORKS.regonumber) > 1 /* having the condition that the number of regonumbers is more */
																		/* than one, indicating that the agent works for more than 1 agency */
order by agname , AGENT.regonumber asc /* order alphabetically by agent name then , the regonumber ascending*/
\g
