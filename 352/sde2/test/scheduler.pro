/*
 * overall_solution/3
 * 
 * prototype:
 * overall_solution(+Needs, +Resources, -Solution)
 *
 * Solution is a list of Partial_Assign lists, which are the form [Course, Section, TA]
 *
 */

/*overall_solution([Needs_Head|Needs_Tail],Resources,[[Course,Section,Sname]|Sol_Tail]) :-
	course_solution(Needs_Head,Resources,[Course,Section,Sname]),
	remove_student(Sname,Resources,New_Resources),
	((Needs_Tail \= [],New_Resources \= []),overall_solution(Needs_Tail,New_Resources,Sol_Tail));
	Needs_Tail = [];New_Resources = [].
*/

overall_solution([Needs_Head|Needs_Tail],Resources,[[Course,Section,Sname]|Sol_Tail]) :-
	course_solution(Needs_Head,Resources,[Course,Section,Sname]),
	remove_student(Sname,Resources,New_Resources),
	(New_Resources \= [],overall_solution(Needs_Tail,New_Resources,Sol_Tail);Needs_Tail == []).

/*
 * qualified/2
 * 
 * prototype:
 * overall_solution(+Course,+Qualified_List)
 *
 * a helper predicate that evaluates to true if Course is in Qualified_List
 *
 */

qualified(Acourse, [Acourse|_]).
qualified(Acourse, [_|OtherCourses]) :-
	qualified(Acourse, OtherCourses).

/*
 * assign/3
 * 
 * prototype:
 * assign(+Aneed,+Aresource,-Partial_Assign)
 * 
 * true when Need can be matched with Resource, binding student's name to Partial_Assign
 */
assign([Course, Section, Aday, Astart, Astop],[Sname,Qualified_List,Unavailable_List],[Course,Section,Sname]) :-
	qualified(Course,Qualified_List),!,no_conflict_all_unavailable(Aday,Astart,Astop,Unavailable_List).

/*
 * course_solution/3
 * 
 * prototype:
 * course_solution(+Aneed,+Resources,-Assignment)
 *
 * satisfies Aneed with the first available student in Resources, or
 * assignes [none,none,none] to Assignment.
 */

course_solution(Aneed,[Resource_Head|Resource_Tail],[Course,Section,Sname]) :-		
	(assign(Aneed,Resource_Head,[Course,Section,Sname]);
	course_solution(Aneed,Resource_Tail,[Course,Section,Sname]));
	(Course = none,Section = none, Sname = none).

/*
 * remove_student/3
 * 
 * prototype:
 * remove_student(+StudentName,+Resources,-Revised_Resources)
 * 
 * removes student with StudentName from Resources. if 'none' is entered, list is not altered.
 * student must be in the list, and only once, for this predicate to succeed.
 */

remove_student(none,Resources,Resources).
remove_student(StudentName,[[StudentName|_]|Resources_Tail],Resources_Tail).
remove_student(StudentName,[Resources_Head|Resources_Tail],[Resources_Head|Modified_R_Tail]) :-
	remove_student(StudentName,Resources_Tail,Modified_R_Tail).

/*
 * no_overlap/4
 * 
 * prototype: 
 * no_overlap(+A,+B,+C,+D)
 *
 * succeeds if the time frame defined by A and B does not interfere with
 * that of C and D.
 *
 */

no_overlap(A,B,C,D) :- 
	B=<C ; A>=D.

/*
 * 
 * no_conflict/6
 * 
 * prototype:
 * no_conflict(+Aday,+Astart,+Astop,+Sday,+Sstart,+Sstop)
 * 
 * succeeds if no conflict exists between the student/needed day and time
 *
 */

no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop) :- 
	Aday \= Sday ; no_overlap(Astart,Astop,Sstart,Sstop).

/*
 * no_conflict_all_unavailable/4
 * 
 * prototype:
 * no_conflict_all_unavailable(+Aday,+Astart,+Astop,+Unavailable_List)
 * 
 * iterates through Unavailable_List recursively, succeeds if no conflict
 * exists with ALL items on Unavailable_List, or if Unavailable_List is empty.
 */

no_conflict_all_unavailable(_,_,_,[]).
no_conflict_all_unavailable(Aday,Astart,Astop,[[Sday,Sstart,Sstop]|Unavailable_Tail]) :-
	no_conflict(Aday,Astart,Astop,Sday,Sstart,Sstop),
	(no_conflict_all_unavailable(Aday,Astart,Astop,Unavailable_Tail);Unavailable_Tail = []).

/*
 * print_assignment/1
 * 
 * prototype:
 * print_assignment([+Course,+Section,+TA])
 *
 * prints the list passed by caller.
 *
 */

print_assignment([Course,Section,TA]) :-
	nl,write(TA),write(' is assigned to '),
	write(Course),write(', Section '),write(Section),nl.

