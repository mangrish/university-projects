/*=========================*/
/*-------------------------*/
/*---- Question 2 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q2.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/

/* Question 2: this query lists each agent whose year to date bookings are greater than the 
	 						 average year to date bookings for all agents in the registry */

select	distinct agname /* select the agent...*/
from		AGENT		/* from agent...*/
where 	ytd_bookings > (select avg(ytd_bookings) /*where the year to date bookings */
											from AGENT)								 /*for that agent is greater than the */
																							 /* average year to date bookings for all */
																								 /* agents.*/
/g
