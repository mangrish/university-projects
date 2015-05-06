%------------------------------------------------------------------------------
%/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
%------------------------------------------------------------------------------
%
%       program:        Project, Semester 2,2000:      PUZZLE SOLVER 
%
%       for:                    433-255, logic and computation;
%
%       written:        Mark Angrish, Student number 74592
%                                               Project, september 2000.
%
%
%
%------------------------------------------------------------------------------
%/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
%------------------------------------------------------------------------------


%------------------------------------------------------------------------------

test_puzzle(Name, Solution) :-
	structure(Name, Structure),
	clues(Name, Structure, Clues),
	queries(Name, Structure, Queries, Solution),
	solve_puzzle(puzzle(Clues, Queries, Solution), Solution).


solve_puzzle(puzzle(Clues,Queries,Solution), Solution):-
		solve(Clues),
		solve(Queries).

solve([Clue|Clues]):-
	Clue, solve(Clues).
	solve([ ]).
%==============structures===========================

%------zebra---------
structure( zebra, [man(H1, N1, P1, D1, C1), man(H2, N2, P2, D2, C2), man(H3, N3, P3, D3, C3), man(H4, N4, P4, D4, C4), man(H5, N5, P5, D5, C5)] ).


%------olympic-------
structure(olympic, [ day( A1, T1, B1), day( A2, T2, B2), day( A3, T3, B3), day( A4, T4, B4), day(A5, T5, B5)]).


%-------frogs--------
structure(frogs, [ friend(N1, T1), friend(N2, T2), friend(N3, T3), friend(N4, T4), friend(N5, T5) ] ).


%-------policeman-----
%structure(policeman, [ event(T1, L1, N1), event(T2, L2, N2), event(T3, L3, N3), event(T3, L3, N3) event(T5, L5, N5)] ).

%------spelling-------


%------flatfoot----


%==============clues================================

%------zebra--------

clues(zebra, Men,
	[	(member(Man1Clue1, Men), 
		 nationality(Man1Clue1, englishman),			% Clue 1
		 house(Man1Clue1, red) ),
		(member(Man1Clue2, Men),
		 nationality(Man1Clue2, spaniard),				% Clue 2
		 pet(Man1Clue2, dog) ),
		(member(Man1Clue3, Men),
		 house(Man1Clue3, green),									% Clue 3
		 drink(Man1Clue3, coffee) ),
		(member(Man1Clue4, Men),
		 nationality(Man1Clue4, ukranian),				% Clue 4
		 drink(Man1Clue4, tea) ),
		(on_right(Man1Clue5, Man2Clue5, Men),			% Clue 5
		 house(Man1Clue5, green),
		 house(Man2Clue5, ivory) ),
		(member(Man1Clue6, Men),
		 cigarette(Man1Clue6, winston),						% Clue 6
		 pet(Man1Clue6, snails) ),
		(member(Man1Clue7, Men),
		 house(Man1Clue7, yellow),								% Clue 7
		 cigarette(Man1Clue7, kools) ),
		(middle(Man1Clue8, Men),									% Clue 8
		 drink(Man1Clue8, milk) ),
		(first(Man1Clue9, Men),										% Clue 9
		 nationality(Man1Clue9, norwegian) ),
		(next_to(Man1Clue10, Man2Clue10, Men),		% Clue 10
		 cigarette(Man1Clue10, chesterfields),
		 pet(Man2Clue10, fox) ),
		(next_to(Man1Clue11, Man2Clue11, Men),		% Clue 11
		 cigarette(Man1Clue11, kools),
		 pet(Man2Clue11, horse) ),
		(member(Man1Clue12, Men),
		 cigarette(Man1Clue12, lucky_strike),			% Clue 12
		 drink(Man1Clue12, orange_juice) ),
		(member(Man1Clue13, Men),
		 nationality(Man1Clue13, japanese),				% Clue 13
		 cigarette(Man1Clue13, parliaments) ),
		(next_to(Man1Clue14, Man2Clue14, Men),		% Clue 14
		 nationality(Man1Clue14, norwegian),
		 house(Man2Clue14, blue) )
	] ).


%-------olympic--------

clues(olympic,Week,
		[	(not_friday(Day1Clue1, Week),				%clue 1
			 time(Day1Clue1, 3),
			 activity(Day1Clue1, stepAerobics) ),
			(tuesday(Day1Clue2, Week),
			 time(Day1Clue2, 4) ),
			(wednesday(Day1Clue3, Week),				%clue 3
			 activity(Day1Clue3, exerciseBike),
			 breakfast(Day1Clue3, muesli); breakfast(Day1Clue3, lowCalorieFlakes),
			 thursday(Day2Clue3, Week),
			 breakfast(Day2Clue3, grapeFruit),
			 not_aerobics(Day2Clue3)),
			(not_friday(Day1Clue4, Week),						%clue 4
			 activity(Day1Clue4, weights) ),
			(member(Day1Clue5, Week),
			 activity(Day1Clue5, pressUps),			%clue 5
			 breakfast(Day1Clue5, grapeFruit),
			 member(Day2Clue5, Week),
			 breakfast(Day2Clue5, muesli),
			 three_less(Day1Clue5, Day2Clue5)),
			(member(Day1Clue6, Week),
			 time(Day1Clue6, 2),
			 breakfast(Day1Clue6, poachedEgg)),
			(member(Day1Clue7, Week),
			 breakfast(Day1Clue7, orangeJuice)),
			(member(Day1Clue8, Week),
			 activity(Day1Clue8, chestExpander)),
			(member(Day1Clue9, Week),
			 time(Day1Clue9, 5))
		]).


%-------frogs----------
clues(frogs, Friends,
		[(member(Friend1Clue2, Friends),
		  member(Friend2Clue2, Friends),	
			whois(Friend1Clue2, albert),
			whois(Friend2Clue2, bernard),
			(type(Friend1Clue2, kangaroo),
			type(Friend2Clue2, kangaroo));
			(type(Friend1Clue2, frog),
			type(Friend2Clue2, frog))),
			
		 (member(Friend1Clue3, Friends),
		  member(Friend2Clue3, Friends),
			whois(Friend1Clue3, charles),
			whois(Friend2Clue3, daniel),
			(type(Friend1Clue3, kangaroo),
			type(Friend2Clue3, frog));
		  (type(Friend1Clue3, frog),
			type(Friend2Clue3, kangaroo))),

		 (member(Friend1Clue4, Friends),
		  member(Friend2Clue4, Friends),
			whois(Friend1Clue4, ellie),
			whois(Friend2Clue4, albert),
			(type(Friend1Clue4, kangaroo),
			type(Friend2Clue4, kangaroo));
			(type(Friend1Clue4, frog),
			type(Friend2Clue4, frog))),

		 (member(Friend1Clue5, Friends),
		  member(Friend2Clue5, Friends),
			whois(Friend1Clue5, bernard),
			whois(Friend2Clue5, charles),
			(type(Friend1Clue5, kangaroo),
			type(Friend2Clue5, frog));
			(type(Friend1Clue5, frog),
			type(Friend2Clue5, kangaroo))),

		 (member(Friend1Clue6, Friends),
		  member(Friend2Clue6, Friends),
			member(Friend3Clue6, Friends),
			whois(Friend1Clue6, daniel),
			whois(Friend2Clue6, ellie),
			whois(Friend3Clue6, albert),
			(type(Friend1Clue6, kangaroo),
			((type(Friend2Clue6, kangaroo) , type(Friend3Clue6, frog));
			(type(Friend2Clue6, frog) , type(Friend3Clue6, kangaroo))));
			(type(Friend1Clue6, frog),
			((type(Friend2Clue6, frog), type(Friend3Clue6, frog));
			(type(Friend2Clue6, kangaroo), type(Friend3Clue6, kangaroo)))))

		]).


%-------policeman------

%-------spelling-------


%-------inspector flatfoot-----



%=========queries===========

%---------zebra----------

queries(zebra, Men, 
	[	member(Q1, Men),					% Query 1
		nationality(Q1, Name1),
		pet(Q1, zebra),
		member(Q2, Men),					% Query 2
		nationality(Q2, Name2),
		drink(Q2, water)
	],
	[['the zebra owner is' , Name1], ['the water drinker is' , Name2]]
).


%---------olympic---------

queries(olympic, Week,
		[ monday(Q1, Week),
			activity(Q1, Activity1),
			time(Q1, Time1),
			breakfast(Q1, Breakfast1),
			tuesday(Q2, Week),
			activity(Q2, Activity2),
			time(Q2, Time2),
			breakfast(Q2, Breakfast2),
			wednesday(Q3, Week),	
			activity(Q3, Activity3),
			time(Q3, Time3),
			breakfast(Q3, Breakfast3),
			thursday(Q4, Week),
			activity(Q4, Activity4),
			time(Q4, Time4),
			breakfast(Q4, Breakfast4),
			friday(Q5, Week),
			activity(Q5, Activity5),
			time(Q5, Time5),
			breakfast(Q5, Breakfast5)
		],
		['Activities Monday to Friday are:' , Activity1, Activity2, Activity3, Activity4, Activity5,
		 'Times Monday to Friday are:' , Time1, Time2, Time3, Time4, Time5, 'Breakfasts Monday to Friday are:'
		 , Breakfast1, Breakfast2, Breakfast3, Breakfast4, Breakfast5 ]
).


%--------frogs------------

queries(frogs, Friends,
		[	Total is 0,
			add_frogs(Friends, Total, Final) 

		],
		['The number of frogs is', Final]
).

%--------policeman--------
queries(policeman, Events,
		[

		],
		['Incidents Monday to Friday were:', Incident1, Incident2, Incident3, Incident4,  Incident5, 
'Times  Monday to Friday  were:', Time1, Time2, Time3, Time4, Time5, 'Locations Monday to Friday 
were:', Location1, Location2, Location3, Location4, Location5]
).
%--------spelling---------


%--------inspector flatfoot----


%======additional predicates=======

%-----zebra--------

house(man(A, B, C, D, E), A).
nationality(man(A, B, C, D, E), B).
pet(man(A, B, C, D, E), C).
drink(man(A, B, C, D, E), D).
cigarette(man(A, B, C, D, E), E).

next_to(X,Y,L) :- on_right(X,Y,L).
next_to(X,Y,L) :- on_right(Y,X,L).

on_right(X,Y,[X,Y|_]).
on_right(X,Y,[_|T]):- on_right(X,Y,T).

middle( C, [_, _, C, _, _] ).

first( A, [A | As]).


%---------olympic------------

activity(day(A, B, C), A).
time(day(A, B, C), B).
breakfast(day(A, B, C), C).

monday( A, [A, B, C, D, E]).
tuesday( B,[A, B, C, D, E]).
wednesday( C, [A, B, C, D, E]).
thursday( D, [A, B, C, D, E]).
friday( E, [A, B, C, D, E]).

not_friday( X ,Xs):-
	monday(X, Xs);
	tuesday(X, Xs);
	wednesday(X, Xs);
	thursday(X, Xs).

not_aerobics(X):-
	activity(X, chestExpander);
	activity(X, exerciseBike);
	activity(X, pressUps);
	activity(X, weights).


three_less(X, Y):-
	time(X, 2) , time(Y, 5);
	time(X, 1) , time(Y, 4).
%--------frogs---------

whois(friend(A, B ), A).
type(friend(A, B), B).


add_frogs( [], Y, S).
add_frogs( [X | Xs] , Y, S):-
	type(X,frog),
	plus(1, Y, R),
	S is R,
  add_frogs(Xs, Y, S);
	type(X, kangaroo),
	add_frogs(Xs, Y, S).
%--------policeman------
timeof(event(A, B, C), A).
location(event(A, B, C), B).
nature(event(A, B, C), C).



%--------spelling-------



%-------flatfoot--------

