/* Program : relations.nl -- data clauses and rules for family relations.
** For     : M.U. C.S.Dept: NU-Prolog programming exercise.
** Written : VRJ, Mar 1990, Mar 1992.
** 433-255 Lab material
*/

/*************************/
/***   Data  Clauses   ***/
/*************************/

        % female(F) -- F is a female.
female(alice).               
female(mary).
female(linda).
female(sweeching).

        % male(M) -- M is a male.
male(peter).                 
male(paul).
male(john).
male(arthur).
male(yanjun).

        % parent(P, C) -- P is a parent of C.
parent(john, peter).         
parent(john, paul).
parent(linda, peter).
parent(linda, mary).
parent(john, mary).
parent(arthur, john).
parent(alice, paul).
parent(christos, markos).


/********************************/
/***   Relationships  Rules   ***/
/********************************/

        % mother(M, C) - M is the mother of C.
mother(M, C) :-
				female(M),
				parent(M, C).

        % father(F, C) - F is a father of C.
father(F, C) :- 
        male(F),
        parent(F, C).

        % father(F)    - F is a father.
father(F) :-
				male(F),
				parent(F, X).

        % son(S)       - S is a son (of some parent).
son(S) :-
				male(S),
				parent(P, S).

        % daughter(D, P)     - D is a daughter of P.
daughter(D) :-
				female(D),
				parent(P, D).

        % siblings(A, B)     - A and B are siblings.
        %                      i.e., have at least 1 parent in common.
siblings(A, B) :-
        parent(P, A),
        parent(P, B),
        not(A = B). 


        % brothers(A, B)     - A and B are brothers.
        %                      i.e., have at least 1 parent in common.
brothers(A, B) :-
				male(A),
				male(B),
				parent(P, A),
				parent(P, B),
				not(A = B).

        % grandchild(C, G)   - C is a grandchild of G.
grandchild(C, G) :-
				parent(P, C),
				parent(G, P).

        % cousins(C, D)      - C and D are cousins.
cousins(C, D) :-
				parent(P, C),
				parent(Q, D),
				siblings(P, Q).

        % full_brothers(A, B) - A is a full brother of B.
        %                      i.e., they have both parents in common.
full_brothers(A, B) :-
				male(A),
				male(B),
				mother(P, A),
				father(Q, B),
				father(P, B),
				mother(Q, A),
				not(A = B).

        % half_brothers(A, B) - A is a half brother of B.
        %                      i.e., have exactly 1 parent in common.
half_brothers(A, B) :-
				male(A),
				male(B),
				parent(P, A),

        % ancestor(An, P)    - An is an ancestor of P.
/** Code to be written **/

