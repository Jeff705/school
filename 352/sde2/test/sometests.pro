/*
 * PREDICATE TESTS
 */

print_test :-
	print_assignment([ece3520,3,joe]),
	print_assignment([ece2090,1,randi]).

n_c_a_a_test1 :-
	no_conflict_all_unavailable(m,12,14,[])
	,nl,write('no_conflict_all_available first test passed'),nl;
	nl,write('no_conflict_all_available first test failed'),nl.

n_c_a_a_test2 :-
	no_conflict_all_unavailable(m,12,14,[[tu,15,17],[m,11,13]])
	,nl,write('no_conflict_all_available second test passed'),nl;
	nl,write('no_conflict_all_available second test failed'),nl.

n_c_test1 :-
	no_conflict(m,12,14,tu,13,17)
	,nl,write('no_conflict first test passed'),nl;
	nl,write('no_conflict first test failed'),nl.

n_c_test2 :-
	no_conflict(m,12,14,m,13,17)
	,nl,write('no_conflict second test passed'),nl;
	nl,write('no_conflict second test failed'),nl.

overlap_test1 :-
	no_overlap(1,2,3,4)
	,nl,write('no_overlap first test passed'),nl;
	nl,write('no_overlap first test failed'),nl.

overlap_test2 :-
	no_overlap(3,4,1,4)
	,nl,write('no_overlap second test passed'),nl;
	nl,write('no_overlap second test failed'),nl.

remove_test1 :-
	resources(R),
	nl,write('resources before: '),write(R),
	remove_student(joe,R,R_new),
	nl,write('resources after: '),write(R_new),nl.

remove_test2 :-
	resources(R),
	nl,write('resources before: '),write(R),
	remove_student(none,R,R_new),
	nl,write('resources after: '),write(R_new),nl.

solution_test1(Solution) :-
	resources2(Resources),
	course_solution([ece2090,1,m,13,16],Resources,Solution),
	nl,write(Solution),nl.

solution_test2(Solution) :-
	resources6(Resources),
	course_solution([ece2060,2,w,13,16],Resources,Solution),
	nl,write(Solution),nl.	

assign_test1(Assign) :-
	assign([ece2010,2,tu,13,16],[joe, [ece2090,ece2010,ece3520,ece4420],[[m,13,16]]],Assign),
	nl,write('result of assignment test 1: '),nl,print_assignment(Assign).	

assign_test2(Assign) :-
	assign([ece2010,2,m,13,16],[joe, [ece2090,ece2010,ece3520,ece4420],[[m,13,16]]],Assign),
	nl,write('result of assignment test 2: '),nl,print_assignment(Assign).

overall_test1(Overall) :-
	resources2(R),
	needs4(N),
	overall_solution(N,R,Overall),!,
	nl,write('overall solution 1: '),write(Overall),nl.

overall_test2(Overall) :-
	resources6(R),
	needs6(N),
	overall_solution(N,R,Overall),!,
	nl,write('overall solution 2: '),write(Overall),nl.
