CC=gcc
CFLAGS=-Wall -g

BINS= libmyalloc.so

all: $(BINS)
	
libmyalloc.so: allocator.c
	$(CC) $(CFLAGS)  -fPIC -shared -o libmyalloc.so allocator.c

clean:
	rm $(BINS)
