all: mmap.o
	gcc mmap.o -o mmap -lrt
mmap.o: mmap.c
	gcc -c mmap.c
clean:
	rm -f *.o mmap