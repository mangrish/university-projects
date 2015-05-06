/*=========================*/
/*-------------------------*/
/*---- Question 6 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q6.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/

/* question 6: This query lists all the unconfimed bookings (as stated on the spec sheet) for the
	 Ritz Hotel by client name, agency , amount outstanding, and weeks till arrival */

/* select the client name, the agency used, the amount paid - the no. of people times by the price 
	 of a regular room times by the number of days stayed, and the remaining interval from 'today' till 
	 the date specified */
(select distinct cname as Client, BOOKING.aname as Agent, ((numberofpeople*regprice*interval('days', (enddate - startdate))) - amtpaid) as Outstanding , int2(interval('week' , startdate - date('today'))) as WeeksToArr
from BOOKING , HOTEL  /* from the Booking and Hotel relation */
where BOOKING.hname = 'Ritz Hotel' and /* where the Booking hotel name is the Ritz hotel*/
			HOTEL.hname = 'Ritz Hotel' and /* where the Hotel hotel name is also the Ritz Hotel */
			(roomtype='regular' and /* and where for a regular room, and the amount due is more than half the required bill */
			 (numberofpeople*regprice*int4(interval('days' ,(enddate - startdate)))) / 2 >= amtpaid)
)

union  /* this adds all the luxury rooms with the regular rooms, and removes the duplicates */

/* all the same as above, except now we are using luxury rooms */
(select distinct cname as Cliet, BOOKING.aname as Agent, ((numberofpeople*luxprice * interval('days', (enddate - startdate))) - amtpaid) as Outstanding , int2(interval('week' , startdate - date('today'))) as WeeksToArr
 from BOOKING , HOTEL
 where BOOKING.hname = 'Ritz Hotel' and
 			 HOTEL.hname= 'Ritz Hotel' and
			 (roomtype='luxury' and
			  (numberofpeople*luxprice*int4(interval('days' , (enddate - startdate)))) / 2 >= amtpaid)
)
\g
