CC=gcc
CFLAGS=-Wall -g

BINS=leakcount memory_shim.so

all: $(BINS)
	
leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c

#strace: strace.c
#	$(CC) $(CFLAGS) -o strace strace.c

memory_shim.so: memory_shim.c
	$(CC) $(CFLAGS)  -fPIC -shared -o memory_shim.so memory_shim.c -ldl

clean:
	rm $(BINS)
