%{
	
	#include <stdio.h>
	int yylex (void);
	int yyerror(char *s);

%}

%token DAY
%token STUDENT
%token NUM
%token COURSE
%token LBRACKET
%token RBRACKET
%token COMMA

%%

production: needs_list {
	printf("found valid 'needs' list\n\n");
};

production: unavailable_list {
	printf("found valid 'unavailable_list'\n\n");
};

production: course_capabilities_list {
	printf("found valid 'course_capabilities' list\n\n");
};

production: resources_list {
	printf("found valid 'resources list'\n\n");
};

production: resources_entry {
	printf("found valid 'resources_entry' list\n\n");
};

needs_list: LBRACKET course_list RBRACKET
;

course_list: single_course | single_course COMMA course_list
;

single_course: LBRACKET COURSE COMMA NUM COMMA DAY COMMA NUM COMMA NUM RBRACKET
;

unavailable_list: LBRACKET RBRACKET | LBRACKET unavail_items RBRACKET
;

unavail_items: unavail | unavail COMMA unavail_items
;

unavail: LBRACKET DAY COMMA NUM COMMA NUM RBRACKET
;

course_capabilities_list: LBRACKET courses RBRACKET
;

courses: COURSE | COURSE COMMA courses
;

resources_entry: LBRACKET STUDENT COMMA course_capabilities_list COMMA unavailable_list RBRACKET
;

multi_resources: resources_entry | resources_entry COMMA multi_resources
;

resources_list: LBRACKET multi_resources RBRACKET
;

%%
