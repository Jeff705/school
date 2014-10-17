CC=clang
CFLAGS=-Wall -g


	
object: mythreads.c mythreads.h
	$(CC) -c mythreads.c

library: mythreads.c mythreads.h
	$(CC) -g -c mythreads.c
	ar -cvq libmythreads.a mythreads.o

test: libmythreads.a cooperative_test.c
	$(CC) $(CFLAGS) -o test cooperative_test.c libmythreads.a

clean:
	rm *.a *.o test
