CC=gcc
CFLAGS=-Wall -g

BINS=passwordBreaker passwordServer

all: $(BINS)
	
passwordBreaker: passwordBreaker.c includes.c includes.h
	$(CC) $(CFLAGS) -o passwordBreaker passwordBreaker.c includes.c -lrt

passwordServer: passwordServer.c includes.c includes.h
	$(CC) $(CFLAGS) -o passwordServer passwordServer.c includes.c

clean:
	rm $(BINS)
