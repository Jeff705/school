CC=clang
CFLAGS=-Wall -g


	
object: mythreads.c mythreads.h
	$(CC) -c mythreads.c

library: mythreads.c mythreads.h
	$(CC) -g -c mythreads.c
	ar -cvq libmythreads.a mythreads.o

test: cooperative_test.c
	make library
	$(CC) $(CFLAGS) -o test cooperative_test.c libmythreads.a
	./test

testgcc: cooperative_test.c
	make library
	gcc $(CFLAGS) -o test cooperative_test.c libmythreads.a
	./test

clean:
	-rm *.a *.o test
