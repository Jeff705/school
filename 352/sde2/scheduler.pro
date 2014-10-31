overall_solution([Needs_Head|Needs_Tail],Resources,[[Course,Section,Sname]|Sol_Tail]) :-
	course_solution(Needs_Head,Resources,[Course,Section,Sname]),remove_student(Sname,Resources,New_Resources),!,
	(New_Resources \= [],overall_solution(Needs_Tail,New_Resources,Sol_Tail);Needs_Tail = []).

qualified(Acourse, [Acourse|_]).
qualified(Acourse, [_|OtherCourses]) :-
	qualified(Acourse, OtherCourses).

assign([Course, Section, Aday, Astart, Astop],[Sname,Qualified_List,Unavailable_List],[Course,Section,Sname]) :-
	qualified(Course,Qualified_List),!,no_conflict_all_unavailable(Aday,Astart,Astop,Unavailable_List),!,
	print_assignment([Course,Section,Sname]).

course_solution(Aneed,[Resource_Head|Resource_Tail],[Course,Section,Sname]) :-		
	(assign(Aneed,Resource_Head,[Course,Section,Sname]);
	course_solution(Aneed,Resource_Tail,[Course,Section,Sname]));
	(Course = none,Section = none, Sname = none).

remove_student(none,Resources,Resources).
remove_student(StudentName,[[StudentName|_]|Resources_Tail],Resources_Tail).
remove_student(StudentName,[Resources_Head|Resources_Tail],[Resources_Head|Modified_R_Tail]) :-
	remove_student(StudentName,Resources_Tail,Modified_R_Tail).

no_overlap(A,B,C,D) :- 
	B=<C ; A>=D.

no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop) :- 
	Aday \= Sday ; no_overlap(Astart,Astop,Sstart,Sstop).

no_conflict_all_unavailable(_,_,_,[]).
no_conflict_all_unavailable(Aday,Astart,Astop,[[Sday,Sstart,Sstop]|Unavailable_Tail]) :-
	no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop),
	(no_conflict_all_unavailable(Aday,Astart,Astop,Unavailable_Tail);Unavailable_Tail = []).

print_assignment([Course,Section,TA]) :-
	nl,write(TA),write(' is assigned to '),
	write(Course),write(', Section '),write(Section),nl.

