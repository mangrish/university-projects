select distinct  R.cname
from AGENT T, WORKS S, BOOKING R
where T.agname = 'Minnie Mouse' and
			T.regonumber = S.regonumber and
			S.aname = R.aname
group by R.cname
having count(distinct R.aname) = (select count(R.aname)
																  from T , S , R 
																	where T.agname='Minnie Mouse' and
																	T.regonumber= S.regonumber and
																	S.aname = R.aname)
\g
