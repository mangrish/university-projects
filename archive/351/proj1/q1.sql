/*=========================*/
/*-------------------------*/
/*---- Question 1 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q1.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/

/* question 1: This query lists all agents alphebetically who are registered, 
	 but not currently with an agency*/

select distinct agname /*select the agent name...*/
 from AGENT		/* from the agent relation... */
 where regonumber not in (select distinct regonumber /*where the agents registration number*/
		 												from WORKS)							 /*is not in all the registration numbers*/ 
																										 /*in the WORKS relation..*/
 order by agname	/*and order alphebetically by agent name.*/
\g
