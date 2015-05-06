/*=========================*/
/*-------------------------*/
/*---- Question 5 ---------*/
/*-------------------------*/
/*=========================*/
/*
**File: q5.sql
**Coder: Mark Angrish
**Login: mangr
**Date: 11th April 2001
*/

/* question 5: this query lists the client names and their suburbs who have double bookings 
	 							(as defined in the specifications)) along with the hotels and start and end date
								of each booking */

/* select */
select A.cname , A.csuburb , A.hname , A.startdate , A.enddate , B.hname , B.startdate , B.enddate
from BOOKING  A , BOOKING  B
where A.cname = B.cname and
			A.csuburb = B.csuburb and
			((A.enddate < B.enddate and
				A.enddate > B.startdate) or
			 (A.hname <> B.hname and
				A.hname < B.hname and
			  (A.startdate = B.startdate or
				 A.enddate = B.enddate)
			 )
			)
\g
