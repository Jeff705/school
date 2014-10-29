/*I'm a comment!
 *on multiple lines!
 */

no_overlap(A,B,C,D) :- 
	B=<C ; A>=D.

no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop) :- 
	Aday \= Sday ; no_overlap(Astart,Astop,Sstart,Sstop).

print_assignment([Course,Section,TA]) :-
	nl,write(TA),write(' is assigned to '),
	write(Course),write(', Section '),write(Section),nl.

no_conflict_all_unavailable(Aday,Astart,Astop,[[Sday,Sstart,Sstop]|Unavailable_Tail]) :-
	no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop),
	(no_conflict_all_unavailable(Aday,Astart,Astop,Unavailable_Tail);Unavailable_Tail = []).
