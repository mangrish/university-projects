/*=========================*/
/*-------------------------*/
/*---- Question 7 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q7.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/

/* question 7: this query lists all the clients who have bookings with all agencies that
	 						 Minnie Mouse works for */

select distinct  BOOKING.cname /* select the client name */
from AGENT , WORKS , BOOKING /* from the relations agent booking and works */
where AGENT.agname = 'Minnie Mouse' and /*where... the agent name is minnie mouse and..*/
			AGENT.regonumber = WORKS.regonumber and /* her agent regonumber is the same as her works regonumber*/
			WORKS.aname = BOOKING.aname /*and .. her agency name , is the same as the booking name */
group by BOOKING.cname /* group this result by the client name */

/* having the condition on that group that the number of distinct booking agency names is the same as
	 the number (count) of agency Minnie mouse works for */
having count(distinct BOOKING.aname) = (select count(distinct WORKS.aname)
																				from AGENT, WORKS
																				where AGENT.agname = 'Minnie Mouse' and
																					    AGENT.regonumber = WORKS.regonumber
																				)
\g
