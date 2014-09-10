CC=gcc
CFLAGS=-Wall -g

BINS=leakcount strace memory_shim

all: $(BINS)
	
leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c

strace: strace.c
	$(CC) $(CFLAGS) -o strace strace.c

memory_shim: memory_shim.c
	$(CC) $(CFLAGS) -ldl -o memory_shim.so memory_shim.c

clean:
	rm $(BINS)