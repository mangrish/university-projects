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
structure( zebra, [man(_H1, _N1, _P1, _D1, _C1), man(_H2, _N2, _P2, _D2, _C2), man(_H3, _N3, _P3, _D3, _C3), man(_H4, _N4, _P4, _D4, _C4), man(_H5, _N5, _P5, _D5, _C5)] ).


%------olympic-------
structure(olympic, [ day( _A1, _T1, _B1), day( _A2, _T2, _B2), day( _A3, _T3, _B3), day( _A4, _T4, _B4), day(_A5, _T5, _B5)]).


%-------frogs--------
structure(frogs, [ friend(_T1), friend(_T2), friend(_T3), friend(_T4), friend(_T5) ] ).


%-------policeman-----
structure(policeman, [ event(_T1, _L1, _N1), event(_T2, _L2, _N2), event(_T3, _L3, _N3), event(_T4, _L4, _N4), event(_T5, _L5, _N5)] ).

%------spelling-------
structure(spelling, [ pupil(_SIX1, _SEVEN1, _EIGHT1), pupil(_SIX2, _SEVEN2, _EIGHT2), pupil(_SIX3, _SEVEN3, _EIGHT3), pupil(_SIX4, _SEVEN4, _EIGHT4), pupil(_SIX5, _SEVEN5, _EIGHT5)]).

%------flatfoot----
structure(flatfoot, [scene(_M1, _W1, _R1),scene(_M2, _W2, _R2),scene(_M3, _W3, _R3),scene(_M4, _W4, _R4),scene(_M5, _W5, _R5),scene(_M6, _W6, _R6)]). 

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
			 cereal(Day1Clue3),
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
		[((albert(Friend1Clue2, Friends),
		   bernard(Friend2Clue2, Friends)),	
			((type(Friend1Clue2, kangaroo), type(Friend2Clue2, kangaroo));
			 (type(Friend1Clue2, frog), type(Friend2Clue2, frog)))),
			
		 ((charles(Friend1Clue3, Friends),
		   daniel(Friend2Clue3, Friends)),
			((type(Friend1Clue3, kangaroo), type(Friend2Clue3, frog));
		   (type(Friend1Clue3, frog), type(Friend2Clue3, kangaroo)))),

		 ((ellie(Friend1Clue4, Friends),
		   albert(Friend2Clue4, Friends)),
			((type(Friend1Clue4, kangaroo), type(Friend2Clue4, kangaroo));
			 (type(Friend1Clue4, frog), type(Friend2Clue4, frog)))),

		 ((bernard(Friend1Clue5, Friends),
		   charles(Friend2Clue5, Friends)),
			((type(Friend1Clue5, kangaroo), type(Friend2Clue5, frog));
			 (type(Friend1Clue5, frog), type(Friend2Clue5, kangaroo)))),

		 ((daniel(Friend1Clue6, Friends),
		   ellie(Friend2Clue6, Friends),
			 albert(Friend3Clue6, Friends)),
			((type(Friend1Clue6, kangaroo),
			 ((type(Friend2Clue6, kangaroo) , type(Friend3Clue6, frog));
			  (type(Friend2Clue6, frog) , type(Friend3Clue6, kangaroo))));
			(type(Friend1Clue6, frog),
			 ((type(Friend2Clue6, frog), type(Friend3Clue6, frog));
			  (type(Friend2Clue6, kangaroo), type(Friend3Clue6, kangaroo))))))

		]).


%-------policeman------
clues(policeman, Events,
		[(day_after(Event1Clue1, Event2Clue1, Events),
			timeof(Event1Clue1, 8),
			nature(Event1Clue1, cyclingWithoutLights),
			location(Event2Clue1, melbourneCentral)),

		 (not_friday(Event1Clue2, Events),
			timeof(Event1Clue2, 10),
			location(Event1Clue2, tinAlley),
			friday(Event2Clue2, Events),
			nature(Event2Clue2, missingTortoise)),

		 (later_in_day(Event1Clue3, Event2Clue3),
			monday(Event1Clue3, Events),
			member(Event2Clue3, Events),
			location(Event2Clue3, bouverieStreet)),

		 (wednesday(Event1Clue4, Events),
			timeof(Event1Clue4, 12)),

		 (tuesday(Event1Clue5, Events),
		  location(Event1Clue5, victoriaMarket)),

			(not_thursday(Event1Clue6, Events),
			nature(Event1Clue6, scooterAccident)),

		 (member(Event1Clue7, Events),
			nature(Event1Clue7, brokenWindow),
			location(Event1Clue7, royalParade)),

		 (member(Event1Clue8, Events),
			nature(Event1Clue8, drunkAndIncapable)),

		 (member(Event1Clue9, Events),
			timeof(Event1Clue9, 2)),

		 (member(Event1Clue10, Events),
			timeof(Event1Clue10, 4))
		]).
	
	
%-------spelling-------

clues( spelling, Pupils,
		[	(alfie(Pupil1Clue1, Pupils),
			 got_detail_rite(Pupil1Clue1),
			 got_chimney_rite(Pupil1Clue1),
			 got_building_rite(Pupil1Clue1),
			 may(Pupil2Clue1, Pupils),
			 got_unique_rite(Pupil2Clue1),
			 got_scandal_rite(Pupil2Clue1),
			 got_medicine_rite(Pupil2Clue1)),

			(ivor(Pupil1Clue2, Pupils),
			 wrong_8(Pupil1Clue2, delicacy)),

			(on_right(Pupil2Clue3, Pupil1Clue3, Pupils),
			 wrong_6(Pupil1Clue3, detail),
			 wrong_7(Pupil1Clue3, mystery),
			 wrong_8(Pupil2Clue3, crescent),
			 wrong_7(Pupil2Clue3, sphere)),

			(alfie(Pupil1Clue4, Pupils),
			 ida(Pupil2Clue4, Pupils),
			 ivor(Pupil3Clue4, Pupils),
			 kenny(Pupil4Clue4, Pupils),
			 may(Pupil5Clue4, Pupils),
			 got_display_rite(Pupil1Clue4),
			 got_display_rite(Pupil3Clue4),
			 got_display_rite(Pupil4Clue4),
			 got_chimney_rite(Pupil2Clue4),
			 got_chimney_rite(Pupil5Clue4)),
			 
			(member(Pupil1Clue5, Pupils),
			 wrong_8(Pupil1Clue5, building),
			 wrong_7(Pupil1Clue5, scandal)),

			(ida(Pupil1Clue6, Pupils),
			 wrong_6(Pupil1Clue6, plague)),

			(member(Pupil1Clue7, Pupils),
			 wrong_7(Pupil1Clue7, routine),
			 got_magazine_rite(Pupil1Clue7),
			 got_medicine_rite(Pupil1Clue7),
			 got_unique_rite(Pupil1Clue7))
	]).

%------- flatfoot-----
%clues(flatfoot, Scenarios,

		

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
		[	
			add_frogs(Friends, Final)
		],
		['The number of frogs is', Final]
).

%--------policeman--------
queries(policeman, Events,
		[ monday(Q1, Events),
			nature(Q1, Incident1),
			timeof(Q1, Time1),
			location(Q1, Location1),
			tuesday(Q2, Events),
			nature(Q2, Incident2),
			timeof(Q2, Time2),
			location(Q2, Location2),
			wednesday(Q3, Events),	
			nature(Q3, Incident3),
			timeof(Q3, Time3),
			location(Q3, Location3),
			thursday(Q4,Events ),
			nature(Q4, Incident4),
			timeof(Q4, Time4),
			location(Q4, Location4),
			friday(Q5, Events),
			nature(Q5, Incident5),
			timeof(Q5, Time5),
			location(Q5, Location5)
		],
		['Incidents Monday to Friday were:', Incident1, Incident2, Incident3, Incident4,  Incident5, 
'Times  Monday to Friday  were:', Time1, Time2, Time3, Time4, Time5, 'Locations Monday to Friday were:',
Location1, Location2, Location3, Location4, Location5]
).


%--------spelling---------
queries( spelling, Pupils,
		[alfie(Q1, Pupils),
		 wrong_6(Q1, Six1),
		 wrong_7(Q1, Seven1),
		 wrong_8(Q1, Eight1),
		 ida(Q2, Pupils),
		 wrong_6(Q2, Six2),
		 wrong_7(Q2, Seven2),
		 wrong_8(Q2, Eight2),
		 ivor(Q3, Pupils),
		 wrong_6(Q3, Six3),
		 wrong_7(Q3, Seven3),
		 wrong_8(Q3, Eight3),
		 kenny(Q4, Pupils),
		 wrong_6(Q4, Six4),
		 wrong_7(Q4, Seven4),
		 wrong_8(Q4, Eight4),
		 may(Q5, Pupils),
		 wrong_6(Q5, Six5),
		 wrong_7(Q5, Seven5),
		 wrong_8(Q5, Eight5)
		],
		S = ['Alfie Bett misspelt:', Six1, Seven1, Eight1, 'Ida Guest misspelt:',Six2, Seven2, Eight2,'Ivor Notion misspelt:', Six3, Seven3, Eight3,'Kenny Spellett misspelt:', Six4, Seven4, Eight4,'May Beewright misspelt:',Six5, Seven5, Eight5]
		).

%-------- flatfoot----
%queries(flatfoot, Scenarios,



%======additional predicates=======

%-----zebra--------

house(man(A, _B, _C, _D, _E), A).
nationality(man(_A, B, _C, _D, _E), B).
pet(man(_A, _B, C, _D, _E), C).
drink(man(_A, _B, _C, D, _E), D).
cigarette(man(_A, _B, _C, _D, E), E).

next_to(X,Y,L) :- 
	on_right(X,Y,L);
	on_right(Y,X,L).

on_right(X,Y,[X,Y|_Xs]).
on_right(X,Y,[_Xs|T]):- on_right(X,Y,T).

middle( C, [_A, _B, C, _D, _E] ).

first( A, [A | _As]).


%---------olympic------------

activity(day(A, _B, _C), A).
time(day(_A, B, _C), B).
breakfast(day(_A, _B, C), C).

cereal(A):-
	breakfast(A, lowCalorieFlakes);
	breakfast(A, muesli).

monday( A, [A, _B, _C, _D, _E]).
tuesday( B,[_A, B, _C, _D, _E]).
wednesday( C, [_A, _B, C, _D, _E]).
thursday( D, [_A, _B, _C, D, _E]).
friday( E, [_A, _B, _C, _D, E]).

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

type(friend(A), A).

albert( A, [A, _B, _C, _D, _E]).
bernard( B, [_A, B, _C, _D, _E]).
charles( C, [_A, _B, C, _D, _E]).
daniel( D, [_A, _B, _C, D, _E]).
ellie( E, [_A, _B, _C, _D, E]).

add_frogs( [],  0).
add_frogs( [X | Xs] , S):-
	add_frogs(Xs, S2),
	((type(X,frog),
	S is S2+1);
	(type(X, kangaroo),
	S is S2)). 

%--------policeman------
timeof(event(A, _B, _C), A).
location(event(_A, B, _C), B).
nature(event(_A, _B, C), C).

not_thursday( X , Xs):-
	monday(X, Xs);
	tuesday(X, Xs);
	wednesday(X, Xs);
	friday(X, Xs).

day_after(X,Y,[X,Y|_Xs]).
day_after(X,Y,[_Xs|T]):- day_after(X,Y,T).

later_in_day(X, Y):-
	((timeof(Y,10),(timeof(X,12); timeof(X,2); timeof(X,4); timeof(X,8)));
	(timeof(Y,12),(timeof(X,2); timeof(X,4); timeof(X,8)));
	(timeof(Y,2), (timeof(X,4); timeof(X,8)));
	(timeof(Y,4), timeof(X,8))).
%--------spelling-------

wrong_6(pupil(A,_B,_C),A).
wrong_7(pupil(_A,B,_C),B).
wrong_8(pupil(_A,_B,C),C).

alfie( A, [A,_B,_C,_D,_E]).
ida( B, [_A,B,_C,_D,_E]).
ivor( C, [_A,_B,C,_D,_E]).
kenny( D, [_A,_B,_C,D,_E]).
may( E, [_A,_B,_C,_D,E]).

got_chimney_rite(X):-
	wrong_7(X,display);
	wrong_7(X,mystery);
	wrong_7(X,routine);
	wrong_7(X,scandal).

got_scandal_rite(X):-
	wrong_7(X,building);
	wrong_7(X,display);
	wrong_7(X,mystery);
	wrong_7(X,routine).

got_display_rite(X):-
	wrong_7(X,building);
	wrong_7(X,mystery);
	wrong_7(X,routine);
	wrong_7(X,scandal).

got_unique_rite(X):-
	wrong_6(X,detail);
	wrong_6(X,league);
	wrong_6(X,plague);
	wrong_6(X,sphere).

got_detail_rite(X):-
	wrong_6(X,league);
	wrong_6(X,plague);
	wrong_6(X,sphere);
	wrong_6(X,unique).

got_medicine_rite(X):-
	wrong_8(X, building);
	wrong_8(X, crescent);
	wrong_8(X, delicacy);
	wrong_8(X, magazine).

got_building_rite(X):-
	wrong_8(X, crescent);
	wrong_8(X, delicacy);
	wrong_8(X, magazine);
	wrong_8(X, medicine).


got_magazine_rite(X):-
	wrong_8(X, building);
	wrong_8(X, crescent);
	wrong_8(X, delicacy);
	wrong_8(X, medicine).


%-------flatfoot--------

