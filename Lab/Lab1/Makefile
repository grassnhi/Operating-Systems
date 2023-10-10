CC = gcc

all: calc

calc: calc.o
	$(CC) calc.o -o calc

calc.o: calc.c
	$(CC) -c calc.c

clean:
	rm -f *.o calc
