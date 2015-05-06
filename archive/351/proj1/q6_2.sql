select cname as client, aname as agency, ((numberofpeople*regprice*(interval('days', (enddate - startdate)))
) - amtpaid) as amountoutstanding, (int4(interval('weeks', startdate -  'today'))) as weekstoarr
from HOTEL h, BOOKING b
where h.hname = b.hname and
      h.hcity = b.hcity and
      h.hname = 'Ritz Hotel' and
      (roomtype = 'regular' and
       amtpaid < (numberofpeople * regprice * int4(interval('days', (enddate - startdate))))/2)

union

select cname as client, aname as agency, ((numberofpeople*luxprice*(interval('days', (enddate - startdate)))
) - amtpaid) as amountoutstanding, (int4(interval('weeks', startdate -  'today'))) as weekstoarr
from HOTEL h, BOOKING b
where h.hname = b.hname and
      h.hcity = b.hcity and
      h.hname = 'Ritz Hotel' and
       (roomtype = 'luxury' and
        amtpaid < (numberofpeople * luxprice * int4(interval('days', (enddate - startdate))))/2)
\g

