/*=========================*/
/*-------------------------*/
/*---- Question 3 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q3.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/
/* question 3: this query lists each client name and the toal amount of money paid
	 					   to all agencys so far. */

select distinct cname , sum(amtpaid) as Total /* select the client name , and the */
																							/* sum of the amounts paid and call */ 
																							/* the group Total. */
from BOOKING	/* from the Booking relation */
group by cname /* and group the result by client name */
\g
