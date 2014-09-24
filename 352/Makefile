CC=gcc

CFLAGS=-lfl

TESTFILE = needs

all:
	make depend 
	make scanner

depend: schedule.in
	flex schedule.in

scanner: lex.yy.c
	$(CC) -o scanner lex.yy.c $(CFLAGS)

clean:
	rm scanner lex.yy.c

test:
	./scanner < $(TESTFILE)
