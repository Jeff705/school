solution_test(What) :-
	resources2(Resources),
	course_solution([ece2090,1,m,13,16],Resources,What).
	

assign_test(What) :-
	assign([ece2010,2,tu,13,16],[joe, [ece2090,ece2010,ece3520,ece4420],[[m,13,16]]],What).	

overall_test(What) :-
	resources2(R),
	needs4(N),
	overall_solution(N,R,What).
