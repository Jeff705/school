overall_solution(Needs,Resources,Solution)

course_solution(Aneed,Resources,Assignment)

assign(Aneed,Aresource,Partial_Assign)

remove_student(StudentName,Resources,Revised_Resources)

no_overlap(A,B,C,D) :- 
	B=<C; A>=D.

no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop) :- 
	Aday \= Sday ; no_overlap(Astart,Astop,Sstart,Sstop).

no_conflict_all_unavailable(Aday,Astart,Astop,[[Sday,Sstart,Sstop]|Unavailable_Tail]) :-
	no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop),
	(no_conflict_all_unavailable(Aday,Astart,Astop,Unavailable_Tail);Unavailable_Tail = []).
	

print_assignment([Course,Section,TA]) :-
	nl,write(TA),write(' is assigned to '),
	write(Course),write(', Section '),write(Section),nl.
