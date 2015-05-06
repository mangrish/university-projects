%------------------------------------------------------------------------------
%/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
%------------------------------------------------------------------------------
%
%	program:	Project, Semester 2,1999:	COMPARING 2 POKER HANDS.
%
%	for:			433-255, Models of computation;
%
%	written:	Mark Angrish, Student number 74592
%						Project, september 1999.
%
%
%
%------------------------------------------------------------------------------
%/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
%------------------------------------------------------------------------------


%------------------------------------------------------------------------------ 
%compare_poker_hands/3: given two valid poker hands the resulting winner is 
%												determined by the value S, where S is 1 if H1 is better
%												than poker hand H2, 2 if H2 is better than H1, and
%												0, if both H1 and H2 are equal according to the rules
%												of poker.
%------------------------------------------------------------------------------ 

compare_poker_hands(hand(H1),hand(H2),S):-
	swap_kind(H1, R1),
	swap_kind(H2,R2),
	sort(R1,S1),
	sort(R2,S2),
	valid_hands(S1, S2),
 	result(S1, S2, S).	


%------------------------------------------------------------------------------ 
%swap_kind/2:	takes a list of poker kinds and another list of poker kinds and
%							swaps the poker kind with its numerical equivalent poker kind.	
%------------------------------------------------------------------------------ 

swap_kind([],[]).
swap_kind([card(K1,S1)|Cs],[card(SK1,S1)|Rs]):-
	swap(K1,SK1),
	swap_kind(Cs,Rs).


%------------------------------------------------------------------------------ 
%swap/2:	provides values for poker kinds of cards before they are swaped.	
%------------------------------------------------------------------------------ 

swap(X,X):-
	member(X, [2,3,4,5,6,7,8,9,10]).
swap(jack, 11).
swap(queen,12).
swap(king,13).
swap(ace,14).


%------------------------------------------------------------------------------
%valid_hands/2:	2 given hands are considered valid if the length of both hands 
%								5, and the intersection of these hands is 0.
%------------------------------------------------------------------------------ 

valid_hands(H1, H2):-
	length(H1,5),
	length(H2,5),
	intersection(H1,H2,T),
	length(T,0).

%------------------------------------------------------------------------------ 
%result/3:	the result takes a pokeer hands H1, and H2  and returns an integer 
%						value, 1 if hand H1 is better then hand H2 and 2, if H2 is better
%						than H1.  If H1 and H2 are equal it is tie broken.  If after a tie
%						break the hands are still equal the number 0, is returned.
%------------------------------------------------------------------------------ 

result(H1, H2, 1):-
	rank(H1, T1),
  rank(H2,T2),
	T1>T2.
result(H1,H2,2):-
	rank(H1, T1),
  rank(H2,T2),
	T1<T2.
result(H1,H2,R):- 
	rank(H1, T1),
	rank(H2,T2),
	tie_break(H1,H2,R).	


%------------------------------------------------------------------------------
%tie_break/2:	takes hands H1 and H2, and reverses them to get the higher cards
%							at the front of the list, and does a comparison on the hands to 
%							see which is better.
%------------------------------------------------------------------------------

tie_break(H1, H2, R):-
	reverse(H1, R1),
	reverse(H2, R2),
	comp(R1, R2, R).


%------------------------------------------------------------------------------
%comp/2: given a list of poker hands, and another list of poker hands, comp will
%					return the winner determined by which cards have a high value, where
%					0, is a drawn hand, 1 is where H1 is better than H2, and 2 is where
%					H2 is better than H1.
%------------------------------------------------------------------------------

comp([],[],R).
comp([card(K1,S)|Rs],[card(K2,T)|Cs], R):-
	K1>K2,
	comp(Rs,Cs,1).
comp([card(K1,S)|Rs],[card(K2,T)|Cs], R):-
  K1<K2,
  comp(Rs,Cs,2).
comp([card(K1,S)|Rs],[card(K2,T)|Cs], R):-
  K1=:=K2,
  comp(Rs,Cs,0).


%------------------------------------------------------------------------------
%rank/2:	given a hand, a numerical value is assigned corresponding to the rank 
%					of the hand, according to the rules of poker
%------------------------------------------------------------------------------

rank(H,1):-
	high_card(H).
rank(H,2):-
	pair(H).
rank(H,3):-
	two_pair(H).
rank(H,4):-
	three_of_a_kind(H).
rank(H,5):-
	straight(H).
rank(H,6):-
	flush(H).
rank(H,7):-
	full_house(H).
rank(H,8):-
	four_of_a_kind(H).
rank(H,9):-
	straight_flush(H).


%------------------------------------------------------------------------------ 
%high_card/1:	a hand is considered a high card if all cards in a hand are 
%							different.
%------------------------------------------------------------------------------ 

high_card(H):-
	all_different(H).


%------------------------------------------------------------------------------
%pair/1:	a hand is onsidered a pair if there are only two cards that have the 
%					same kind and when these cards are removed from the hand, there are 
%					3 cards left that are all differnet.
%------------------------------------------------------------------------------ 

pair(H):-
	cards_same(H,K),
	subtract(H, K, R),
	length(R, 3),
	all_different(R).


%------------------------------------------------------------------------------
%two_pair/1:	a hand is consididered a two pair if there are only two cards that  
%							have the same kind and when these cards are removed from the hand,
%							there are 3 cards left, and of these cards there is another pair 
%							of a different kind to the first pair that are all differnet 
%------------------------------------------------------------------------------ 

two_pair(H):-
	cards_same(H,K1),
	subtract(H,K1,R),
	length(R, 3),
	cards_same(R,K2),
	subract(R, K2, S),
	length(S, 1).


%------------------------------------------------------------------------------
%three_of_a_kind/1:	a hand is onsidered a three of a kind if there are only 
%										three cards that have the same kind and when these cards are 
%										removed from the hand, there are 2 cards left that are all  
%										differnet.
%------------------------------------------------------------------------------ 

three_of_a_kind(H):-
	cards_same(H,K),
	subtract(H, K, R),
	length(R, 2),
	all_different(R).


%------------------------------------------------------------------------------ 
%straight/1:	a hand is considered a straight if the hand is in order 
%							(eg 4-5-6-7-8), and reguardless of the suit of each kind(ie not
%							the same suit).
%------------------------------------------------------------------------------ 

straight(H):-
	in_order(H),
	not(same_suit(H)).


%------------------------------------------------------------------------------ 
%flush/1:	a hand is considered a flush if all all the cards in the hand are the 
%					same suit (eg J-8-5-3-2, of spades), and all the poker kinds are 
%					all different.
%------------------------------------------------------------------------------ 

flush(H):-
	same_suit(H),
	all_different(H).


%------------------------------------------------------------------------------ 
%full_house/1:	a hand is considered a full house if there is a 3-of-a-kind and
%								a pair (eg K-K-K-5-5), and wen both these are removed from a  
%								hand the number of cards left is 0.
%------------------------------------------------------------------------------ 

full_house(H):-
	cards_same(H,K1),
	subtract(H, K1, R),
	length(R, 2),
	cards_same(R,K2),
	subtract(R,K2,S),
	length(S, 0).
full_house(H):-
	cards_same(H,K1),
	subtract(H, K1, R),
	length(R, 3),
	cards_same(R,K2),
	subtract(R,K2,S),
	length(S, 0).


%------------------------------------------------------------------------------
%four_of_a_kind/1:	a hand is onsidered a four of a kind if there are only 
%										three cards that have the same kind and when these cards are 
%										removed from the hand, there is 1 card left (eg K-K-K-K-2). 
%------------------------------------------------------------------------------ 

four_of_a_kind(H):-
	cards_same(H,K),
	subtract(H, K, R),
	length(R, 1).


%------------------------------------------------------------------------------ 
%straight_flush/1:	a hand is considered a straight flush is all cards in the 
%										hand are of the same suit, and they are all in order
%										(eg 5-6-7-8-9), and do not wrap around.
%------------------------------------------------------------------------------ 

straight_flush(H):-
	same_suit(H),
	in_order(H).


%------------------------------------------------------------------------------ 
%cards_same/2:	cards are considered the same if subsequent kinds of cards in  
%								the list of tuples is the same.
%------------------------------------------------------------------------------ 

cards_same([card(K1,S), card(K1,T) ,card(K1,U)|Cs],[card(K1,S), card(K1,T) ,card(K1,U)] ).
cards_same([card(K1,S), card(K1,T) ,card(K1,U),card(K1,V)|Cs],[card(K1,S), card(K1,T) ,card(K1,U),card(K1,V)] ).
cards_same( [card(K1,X), card(K1,Y) |Cs], [card(K1,X),card(K1,Y)]).
cards_same( [card(K1,X),card(K2,Y) |Cs ], Rs):-
	not(K1 = K2),
	cards_same([ card(K2, Y) |Cs], Rs).


%------------------------------------------------------------------------------ 
%all_different/1:	takes a hand and the cards are considered all different if  
%									subsequent cards are of a different poker kind in the list of 
%									tuples.
%------------------------------------------------------------------------------ 

all_diffrent([]).
all_different( [card(K1,S),card(K2,T)]):-
	K1 =\= K2.
all_different( [card(K1,S),card(K2,T) |Cs ]):-
  not(K1 = K2),
	all_different([ card(K2, T) |Cs]).


%------------------------------------------------------------------------------ 
%same_suit/1:	takes a hand and the cards have the same suit if all cards have 
%							the same suit in thelist of tuples.
%------------------------------------------------------------------------------ 

same_suit([card(K,S1), card(L,S1), card(M,S1), card(N,S1), card(O,S1) ]).


%------------------------------------------------------------------------------ 
%in_order/1:	takes a hand of cards and the hand are  considered in order if 
%							the difference between each subsequent kinds of cards in a list 
%							of tuples is 1.
%------------------------------------------------------------------------------ 

in_order([card(K1,U),card(K2,V),card(K3,W),card(K4,X),card(K5,Y)]):-
	minus(K2,K1,1),
	minus(K3,K2,1),
	minus(K4,K3,1),
	minus(K5,K4,1).


%------------------------------------------------------------------------------ 
%minus/3:	takes 2 numbers and uses the inbuilt plus predicate to perform negation.	
%------------------------------------------------------------------------------ 

minus(X,Y,R):-
	Z is -Y,
	plus(X,Z,R).
