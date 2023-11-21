all: seqlock

seqlock: seqlock.o
	gcc -pthread -o seqlock seqlock.o

seqlock.o: seqlock.c seqlock.h
	gcc -pthread -c seqlock.c

clean:
	rm -f seqlock.o seqlock
